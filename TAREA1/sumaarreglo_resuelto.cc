extern "C" {
#include <bsp.h>
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bsp_main() {
    bsp_begin(bsp_nprocs());

    int pid = bsp_pid();
    int nprocs = bsp_nprocs();

    int tag = 0;
    int sztag = sizeof(int);
    bsp_set_tagsize(&sztag);

    int N = 128;
    int *arreglo = new int[N];

    // Cada procesador inicializa su arreglo local
    srand(pid * 111 + 111 + time(NULL));
    for (int i = 0; i < N; i++) {
        arreglo[i] = rand() % 1024;
    }

    // Cada procesador calcula su suma parcial
    long long suma_parcial = 0;
    for (int i = 0; i < N; i++) {
        suma_parcial += arreglo[i];
    }
    delete[] arreglo;

    // SUPERPASO 1: Envío de sumas parciales a todos
    // Cada procesador envía su suma parcial a todos los demás.
    for (int i = 0; i < nprocs; i++) {
        bsp_send(i, &tag, &suma_parcial, sizeof(suma_parcial));
    }

    bsp_sync(); // Sincronización para asegurar que todos los envíos se completen

    // SUPERPASO 2: Recepción y cálculo de la suma global
    int nm, nb;
    long long temp_suma;
    long long suma_global = 0;

    bsp_qsize(&nm, &nb); // Consultamos cuántos mensajes hemos recibido

    // Cada procesador sabe que debe recibir 'nprocs' mensajes
    for (int i = 0; i < nm; i++) {
        // En este enfoque simple, no necesitamos bsp_get_tag porque todos los
        // mensajes son del mismo tipo y tamaño. bsp_move es suficiente.
        bsp_move(&temp_suma, sizeof(temp_suma));
        suma_global += temp_suma; // Acumulamos las sumas parciales
    }

    // Todos los procesadores imprimen el resultado final para verificar
    if (pid == 0) {
        printf("PID %d: La suma total (calculada en 2 superpasos) es = %lld\n", pid, suma_global);
    }
    
    bsp_sync(); // Sincronización final antes de terminar

    bsp_end();
}

int main(int argc, char *argv[]) {
    bsp_init(bsp_main, argc, argv);
    bsp_main();
    return 0;
} 