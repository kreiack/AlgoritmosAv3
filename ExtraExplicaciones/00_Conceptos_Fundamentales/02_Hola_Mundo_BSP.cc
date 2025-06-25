#include <bsp.h>
#include <stdio.h>

void bsp_main() {
    bsp_begin(bsp_nprocs());

    int pid = bsp_pid();
    int nprocs = bsp_nprocs();

    printf("Hola desde el proceso %d de un total de %d procesos.\\n", pid, nprocs);

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(bsp_main, argc, argv);
    bsp_main();
    return 0;
} 