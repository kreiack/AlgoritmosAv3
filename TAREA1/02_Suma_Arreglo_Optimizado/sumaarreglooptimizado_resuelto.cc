#include <bsp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void bsp_main() {
    bsp_begin(bsp_nprocs());

    int pid = bsp_pid();
    int nprocs = bsp_nprocs();

    // Configuración del tag para mensajes
    int tag = 0;
    int sztag = sizeof(int);
    bsp_set_tagsize(&sztag);
    bsp_sync(); 

    // 1. CÁLCULO DE LA SUMA LOCAL
    // ============================
    long long suma_parcial = 0;
    srand(pid + time(NULL));
    int N = 128;
    for (int i = 0; i < N; i++) {
        suma_parcial += rand() % 100;
    }

    bsp_sync(); // Sincronización después de la suma local

    // 2. REDUCCIÓN EN ÁRBOL
    int n_superpasos = (nprocs > 1) ? floor(log2(nprocs - 1)) + 1 : 0;
    for (int k = 0; k < n_superpasos; ++k) {
        int mask = 1 << k;
        if ((pid & mask) != 0) {
            int dest_pid = pid & ~mask;
            bsp_send(dest_pid, &tag, &suma_parcial, sizeof(suma_parcial));
        } else {
            int src_pid = pid | mask;
            if (src_pid < nprocs) {
                long long suma_recibida;
                int status, received_tag;
                bsp_get_tag(&status, &received_tag);
                if (status > 0) {
                    bsp_move(&suma_recibida, status);
                    suma_parcial += suma_recibida;
                }
            }
        }
        bsp_sync();
    }

    // 3. BROADCAST EN ÁRBOL
    long long suma_global = suma_parcial; 
    bsp_push_reg(&suma_global, sizeof(long long));
    bsp_sync();

    if (pid == 0) {
        for (int i = 1; i < nprocs; i++) {
            bsp_put(i, &suma_global, &suma_global, 0, sizeof(long long));
        }
    }
    bsp_sync();
    bsp_pop_reg(&suma_global);

    // 4. VERIFICACIÓN
    // ===============
    printf("PID %d: La suma global final (optimizada) es = %lld\n", pid, suma_global);
    
    bsp_end();
}

int main(int argc, char *argv[]) {
    bsp_init(bsp_main, argc, argv);
    bsp_main();
    return 0;
} 