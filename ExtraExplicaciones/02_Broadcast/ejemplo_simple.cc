extern "C" {
#include <bsp.h>
}
#include <stdio.h>

void spmd_main() {
    bsp_begin(bsp_nprocs());

    int nprocs = bsp_nprocs();
    int pid = bsp_pid();

    // Configurar el tamaño de la etiqueta (tag)
    int tag_data = 0;
    bsp_set_tagsize(&tag_data);
    bsp_sync();

    int dato_a_difundir;

    // ================== SUPERPASO 0 ==================
    // El procesador 0 (raíz) envía el mismo dato a todos los procesadores.
    if (pid == 0) {
        dato_a_difundir = 77;
        printf("Procesador Raíz (PID %d): Difundiendo el número %d a %d procesadores.\n", pid, dato_a_difundir, nprocs);

        // Bucle para enviar el dato a cada procesador, incluido él mismo.
        for (int i = 0; i < nprocs; ++i) {
            bsp_send(i, &tag_data, &dato_a_difundir, sizeof(dato_a_difundir));
        }
    }
    
    // Sincronización: asegura que el envío se complete antes de que alguien intente recibir.
    bsp_sync();
    // ================== SUPERPASO 1 ==================
    
    // Todos los procesadores reciben el dato.
    int n_mensajes, tam_total_bytes;
    bsp_qsize(&n_mensajes, &tam_total_bytes);

    if (n_mensajes > 0) {
        int dato_recibido;
        bsp_move(&dato_recibido, sizeof(dato_recibido));
        printf("Procesador %d: He recibido el número %d.\n", pid, dato_recibido);
    }

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 