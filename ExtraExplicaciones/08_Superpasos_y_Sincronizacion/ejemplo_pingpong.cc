extern "C" {
#include <bsp.h>
}
#include <stdio.h>
#include <stdlib.h>

void spmd_main() {
    bsp_begin(bsp_nprocs());

    if (bsp_nprocs() != 2) {
        if(bsp_pid() == 0) printf("Este ejemplo requiere exactamente 2 procesadores.\n");
        bsp_end();
        exit(1);
    }

    int tag_data = 0;
    bsp_set_tagsize(&tag_data);
    bsp_sync();

    // ================== SUPERPASO 0: PING ==================
    // El procesador 0 envía un "ping" (el número 1) al procesador 1.
    if (bsp_pid() == 0) {
        int ping_msg = 1;
        printf("PID %d (Superpaso 0): Enviando PING.\n", bsp_pid());
        bsp_send(1, &tag_data, &ping_msg, sizeof(ping_msg));
    }
    
    // La barrera asegura que el PING se entrega antes de que comience el Superpaso 1.
    bsp_sync();
    // ================== SUPERPASO 1: PONG ==================

    if (bsp_pid() == 1) {
        int received_msg;
        bsp_move(&received_msg, sizeof(received_msg));
        printf("PID %d (Superpaso 1): Recibido PING (%d).\n", bsp_pid(), received_msg);

        // En respuesta, el procesador 1 envía un "pong" (el número 2) de vuelta.
        int pong_msg = 2;
        printf("PID %d (Superpaso 1): Enviando PONG.\n", bsp_pid());
        bsp_send(0, &tag_data, &pong_msg, sizeof(pong_msg));
    }

    // La barrera asegura que el PONG se entrega antes de que comience el Superpaso 2.
    bsp_sync();
    // ================== SUPERPASO 2: RECIBIR PONG ==================

    if (bsp_pid() == 0) {
        int n_mensajes, tam_bytes;
        bsp_qsize(&n_mensajes, &tam_bytes);
        if (n_mensajes > 0) {
            int received_msg;
            bsp_move(&received_msg, sizeof(received_msg));
            printf("PID %d (Superpaso 2): Recibido PONG (%d).\n", bsp_pid(), received_msg);
        }
    }
    
    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 