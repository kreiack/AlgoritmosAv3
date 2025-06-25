extern "C" {
#include <bsp.h>
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int   nm,    // numero de mensajes que recibe cada procesador.
      nb,    // total de bytes en la cola de mensajes.
      tag,   // identificador (header) de mensajes (no lo usamos).
      sztag, // tama o del indentificador.
      total; // tama o en bytes del siguiente mensaje a ser sacado
             // de la cola de nuevos mensajes en el procesador.



void bsp_main()
{

  bsp_begin( bsp_nprocs() );

  int pid= bsp_pid();
  int nprocs= bsp_nprocs();

  tag = 0;
  sztag= sizeof(int);

  bsp_set_tagsize(&sztag);

  int N=128;
  int *arreglo=new int[N];

  // inicializacion del arreglo.
  srand(pid*111+111+time(NULL));

  for(int i=0; i<N; i++)
    arreglo[i]= rand()%1024;


  // Cada procesador calcula la suma.

  int suma=0;
  for(int i=0; i<N; i++)
    suma += arreglo[i];

// Superstep 1:

  // cada procesador le envia al procesador 0
  // el resultado parcial de la suma global.

  bsp_send(0, &tag, &suma, sizeof(int));

  bsp_sync(); // debe ir aqui para producir el envio
              // efectivo de los mensajes.

// Superstep 2:

  // lee cuantos mensajes llegaron (nm)
  // y cual es la cantidad total de bytes recibidos (nb).

  if (pid==0)
  {
     bsp_qsize(&nm, &nb); // numero de mensajes recibidos.

     // extrae los mensajes uno por uno desde la cola
     // de mensajes.

     int suma_parcial, suma_global=0;

     for(int i=0; i<nm; i++)
     {
       // solo procesador 0 aqui.
       //If total has  the  value  -1 then the buffer is empty
       //therwise it becomes the length of the payload of the  first  message  in the buffer.
       bsp_get_tag(&total,&tag);
       //moves the first mgs of size total
       bsp_move(&suma_parcial,total);
       suma_global += suma_parcial;
     }

     for(int i=0; i<nprocs; i++)
        bsp_send(i, &tag, &suma_global, sizeof(int));
  }

  bsp_sync();

// Superstep 3:

  int resultado;

  bsp_get_tag(&total,&tag);
  bsp_move(&resultado,total);

 // if (pid==1)
  //{
     printf("PID %d El resultado es = %d\n",pid,resultado);
  //}

  bsp_sync();
  bsp_end();

}


int main (int argc, char *argv[])
{
   void bsp_main();

   bsp_init(bsp_main, argc, argv);

   bsp_main();

   return 0;
} 