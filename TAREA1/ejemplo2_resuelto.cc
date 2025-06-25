extern "C" {
#include <bsp.h>
}

#include <stdio.h>

void bsp_main() {
    bsp_begin(bsp_nprocs());

    int pid = bsp_pid();
    int nprocs = bsp_nprocs();

    const int N = 10;
    int arreglo[N];

    int tag = 0;
    int sztag = sizeof(int);
    bsp_set_tagsize(&sztag);

    // SUPERPASO 1: Creación y envío del arreglo
    if (pid == 0) {
        for (int i = 0; i < N; i++) {
            arreglo[i] = (i + 1) * 10;
        }
        
        if (nprocs > 1) {
            printf("PID %d: Enviando arreglo al PID 1...\n", pid);
            bsp_send(1, &tag, arreglo, N * sizeof(int));
        } else {
            printf("Se necesitan al menos 2 procesadores para este ejemplo.\n");
        }
    }

    bsp_sync();

    // SUPERPASO 2: Recepción e impresión del arreglo
    if (pid == 1) {
        int nm, nb, total;
        bsp_qsize(&nm, &nb);

        if (nm > 0) {
            int arreglo_recibido[N];
            bsp_get_tag(&total, &tag);

            if (total == N * sizeof(int)) {
                bsp_move(arreglo_recibido, total);
                printf("PID %d: Arreglo recibido:\n", pid);
                for (int i = 0; i < N; i++) {
                    printf("  arreglo[%d] = %d\n", i, arreglo_recibido[i]);
                }
            }
        }
    }

    bsp_end();
}

int main(int argc, char *argv[]) {
    bsp_init(bsp_main, argc, argv);
    bsp_main();
    return 0;
} 