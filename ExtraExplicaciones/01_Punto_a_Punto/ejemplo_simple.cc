extern "C" {
#include <bsp.h>
}
#include <stdio.h>

void spmd_main() {
    bsp_begin(bsp_nprocs());

    // Se requiere al menos 2 procesadores para este ejemplo
    if (bsp_nprocs() < 2) {
        if (bsp_pid() == 0) {
            printf("Error: Se necesitan al menos 2 procesadores.\n");
        }
        bsp_end();
        return;
    }

    // Configurar el tamaño de la etiqueta (tag)
    // El tag es un metadato que acompaña al mensaje.
    int tag_data = 0;
    bsp_set_tagsize(&tag_data);
    bsp_sync(); // Sincronizar para asegurar que la configuración del tag se aplica en todos los procs

    // ================== SUPERPASO 0 ==================
    // El procesador 0 envía un dato al procesador 1
    if (bsp_pid() == 0) {
        int dato_a_enviar = 42;
        printf("Procesador %d: Enviando el número %d al procesador 1.\n", bsp_pid(), dato_a_enviar);
        
        // bsp_send(pid_destino, tag, payload, tamaño_payload)
        // - pid_destino: 1
        // - tag: puntero a la etiqueta (opcional, puede ser NULL)
        // - payload: puntero a los datos que se envían
        // - tamaño_payload: tamaño de los datos en bytes
        bsp_send(1, &tag_data, &dato_a_enviar, sizeof(dato_a_enviar));
    }

    // Barrera de sincronización: Finaliza el superpaso 0.
    // Garantiza que todos los bsp_send() se completen antes de continuar.
    bsp_sync();
    // ================== SUPERPASO 1 ==================

    // El procesador 1 recibe el dato y lo imprime
    if (bsp_pid() == 1) {
        int n_mensajes, tam_total_bytes;
        
        // bsp_qsize consulta cuántos mensajes y bytes hay en la cola de entrada.
        bsp_qsize(&n_mensajes, &tam_total_bytes);

        if (n_mensajes > 0) {
            int dato_recibido;
            
            // bsp_move(destino, tamaño_a_mover)
            // Mueve el primer mensaje de la cola a la variable `dato_recibido`.
            bsp_move(&dato_recibido, sizeof(dato_recibido));
            
            printf("Procesador %d: He recibido el número %d.\n", bsp_pid(), dato_recibido);
        }
    }

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 