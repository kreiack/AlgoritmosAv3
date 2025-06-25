extern "C" {
#include <bsp.h>
}
#include <stdio.h>

void spmd_main() {
    bsp_begin(bsp_nprocs());

    int nprocs = bsp_nprocs();
    int pid = bsp_pid();

    int tag_data = 0;
    bsp_set_tagsize(&tag_data);
    bsp_sync();

    // ================== SUPERPASO 0 ==================
    // Cada procesador genera un valor local y lo envía a la raíz (pid 0).
    int valor_local = pid + 1; // p0=1, p1=2, p2=3, ...
    printf("Procesador %d: Mi valor local es %d. Enviando a la raíz.\n", pid, valor_local);
    bsp_send(0, &tag_data, &valor_local, sizeof(valor_local));
    
    bsp_sync();
    // ================== SUPERPASO 1 ==================
    
    // El procesador 0 (raíz) recolecta los valores y los reduce (suma).
    if (pid == 0) {
        printf("Procesador Raíz (PID %d): Reduciendo valores...\n", pid);
        long long suma_global = 0; // Usamos long long para evitar desbordes.

        int n_mensajes, tam_total_bytes;
        bsp_qsize(&n_mensajes, &tam_total_bytes);

        for(int i = 0; i < n_mensajes; ++i) {
            int valor_recibido;
            bsp_move(&valor_recibido, sizeof(valor_recibido));
            
            // Operación de reducción
            suma_global += valor_recibido;
        }
        
        printf("Procesador Raíz (PID %d): La suma total (reducción) es %lld.\n", pid, suma_global);
    }

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 