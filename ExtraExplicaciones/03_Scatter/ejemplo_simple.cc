extern "C" {
#include <bsp.h>
}
#include <stdio.h>
#include <vector>

void spmd_main() {
    bsp_begin(bsp_nprocs());

    int nprocs = bsp_nprocs();
    int pid = bsp_pid();

    const int DATOS_POR_PROCESADOR = 3;
    const int N_GLOBAL = nprocs * DATOS_POR_PROCESADOR;

    int tag_data = 0;
    bsp_set_tagsize(&tag_data);
    bsp_sync();

    // ================== SUPERPASO 0 ==================
    // El procesador 0 (raíz) crea los datos y los "esparce" (scatters) a todos.
    if (pid == 0) {
        std::vector<int> arreglo_global(N_GLOBAL);
        printf("Procesador Raíz (PID %d): Creando y esparciendo datos...\n", pid);
        for (int i = 0; i < N_GLOBAL; ++i) {
            arreglo_global[i] = (i + 1) * 10;
        }

        // Bucle para enviar a cada procesador su trozo correspondiente.
        for (int i = 0; i < nprocs; ++i) {
            // Se calcula un puntero al inicio del trozo para el procesador 'i'.
            int* puntero_trozo = &arreglo_global[i * DATOS_POR_PROCESADOR];
            printf("  - Enviando datos [%d, %d, %d] a PID %d\n", puntero_trozo[0], puntero_trozo[1], puntero_trozo[2], i);
            bsp_send(i, &tag_data, puntero_trozo, DATOS_POR_PROCESADOR * sizeof(int));
        }
    }
    
    bsp_sync();
    // ================== SUPERPASO 1 ==================
    
    // Todos los procesadores reciben su trozo de datos.
    std::vector<int> arreglo_local(DATOS_POR_PROCESADOR);
    bsp_move(arreglo_local.data(), DATOS_POR_PROCESADOR * sizeof(int));

    printf("Procesador %d: He recibido los datos: [", pid);
    for (int i = 0; i < DATOS_POR_PROCESADOR; ++i) {
        printf("%d%s", arreglo_local[i], i == DATOS_POR_PROCESADOR - 1 ? "" : ", ");
    }
    printf("]\n");

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 