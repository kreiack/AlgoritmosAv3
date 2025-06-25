#include <bsp.h>
#include <stdio.h>

void bsp_main() {
    bsp_begin(bsp_nprocs());

    int pid = bsp_pid();
    int nprocs = bsp_nprocs();

    // En este ejemplo simple, no necesitamos un tag, por lo que no lo configuramos.
    
    // SUPERPASO 1: Envío
    if (pid == 0) {
        // El proceso 0 envía un mensaje solo si hay al menos 2 procesos.
        if (nprocs > 1) {
            int mensaje = 2024;
            printf("Proceso %d enviando el valor %d al proceso 1...\\n", pid, mensaje);
            bsp_send(1, NULL, &mensaje, sizeof(int));
        }
    }

    // Barrera de sincronización:
    // Asegura que todos los `bsp_send` se hayan iniciado antes de que cualquier
    // proceso intente leer de su cola de entrada.
    bsp_sync(); 

    // SUPERPASO 2: Recepción
    if (pid == 1) {
        int mensaje_recibido;
        // Mueve el primer (y único) mensaje de la cola a la variable local.
        bsp_move(&mensaje_recibido, sizeof(int));
        printf("Proceso %d recibió el valor: %d\\n", pid, mensaje_recibido);
    }

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(bsp_main, argc, argv);
    bsp_main();
    return 0;
} 