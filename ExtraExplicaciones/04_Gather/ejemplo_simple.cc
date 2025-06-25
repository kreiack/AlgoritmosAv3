extern "C" {
#include <bsp.h>
}
#include <stdio.h>
#include <vector>

void spmd_main() {
    bsp_begin(bsp_nprocs());

    int nprocs = bsp_nprocs();
    int pid = bsp_pid();

    // El tag se usará para enviar el PID del remitente.
    bsp_set_tagsize(&pid); 
    bsp_sync();

    // ================== SUPERPASO 0 ==================
    // Cada procesador genera un dato local y lo envía al procesador 0 (raíz).
    int dato_local = 100 * (pid + 1);
    printf("Procesador %d: Enviando mi dato (%d) a la raíz.\n", pid, dato_local);
    
    // El PID del remitente se envía como tag para que la raíz sepa de quién es el dato.
    bsp_send(0, &pid, &dato_local, sizeof(dato_local));
    
    bsp_sync();
    // ================== SUPERPASO 1 ==================
    
    // El procesador 0 (raíz) recolecta los datos de todos.
    if (pid == 0) {
        printf("Procesador Raíz (PID %d): Recolectando datos...\n", pid);
        std::vector<int> arreglo_recolectado(nprocs);

        int n_mensajes, tam_total_bytes;
        bsp_qsize(&n_mensajes, &tam_total_bytes);

        if (n_mensajes > 0) {
            for(int i = 0; i < n_mensajes; ++i) {
                int pid_origen;
                int tam_mensaje;
                int dato_recibido;

                // bsp_get_tag() recupera el tag del siguiente mensaje sin moverlo.
                // Lo usamos para saber de quién es el mensaje.
                bsp_get_tag(&tam_mensaje, &pid_origen);
                
                // bsp_move() mueve el payload del mensaje.
                bsp_move(&dato_recibido, sizeof(dato_recibido));

                // Colocamos el dato en la posición correcta del arreglo.
                arreglo_recolectado[pid_origen] = dato_recibido;
            }
        }
        
        printf("Procesador Raíz (PID %d): Arreglo final recolectado: [", pid);
        for (int i = 0; i < nprocs; ++i) {
            printf("%d%s", arreglo_recolectado[i], i == nprocs - 1 ? "" : ", ");
        }
        printf("]\n");
    }

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 