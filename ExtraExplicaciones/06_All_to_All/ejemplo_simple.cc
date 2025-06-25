extern "C" {
#include <bsp.h>
}
#include <stdio.h>
#include <vector>
#include <numeric>

void spmd_main() {
    bsp_begin(bsp_nprocs());

    int nprocs = bsp_nprocs();
    int pid = bsp_pid();

    int tag_data = 0;
    bsp_set_tagsize(&tag_data);
    bsp_sync();

    // ================== SUPERPASO 0 ==================
    // Cada procesador envía su PID a todos los demás procesadores.
    printf("Procesador %d: Enviando mi PID a todos.\n", pid);
    for (int destino_pid = 0; destino_pid < nprocs; ++destino_pid) {
        bsp_send(destino_pid, &tag_data, &pid, sizeof(pid));
    }
    
    bsp_sync();
    // ================== SUPERPASO 1 ==================
    
    // Cada procesador recolecta todos los mensajes que le llegaron.
    int n_mensajes, tam_total_bytes;
    bsp_qsize(&n_mensajes, &tam_total_bytes);
    
    std::vector<int> pids_recibidos;
    if (n_mensajes > 0) {
        pids_recibidos.resize(n_mensajes);
        for(int i = 0; i < n_mensajes; ++i) {
            bsp_move(&pids_recibidos[i], sizeof(int));
        }
    }

    printf("Procesador %d: He recibido %zu PIDs: [", pid, pids_recibidos.size());
    for (size_t i = 0; i < pids_recibidos.size(); ++i) {
        printf("%d%s", pids_recibidos[i], i == pids_recibidos.size() - 1 ? "" : ", ");
    }
    printf("]\n");

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 