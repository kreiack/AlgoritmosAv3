extern "C" {
#include <bsp.h>
}

#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_NAME_LEN 20
#define NUM_PERSONAS 3

// Estructura para enviar los datos de forma contigua en memoria
struct DatoSerializado {
    int edad;
    char nombre[MAX_NAME_LEN];
    char apellido[MAX_NAME_LEN];
};

void bsp_main() {
    bsp_begin(bsp_nprocs());

    int pid = bsp_pid();
    int nprocs = bsp_nprocs();

    int tag = 0;
    int sztag = sizeof(int);
    bsp_set_tagsize(&sztag);

    DatoSerializado buffer_envio[NUM_PERSONAS];

    // SUPERPASO 1: Serialización y envío de objetos
    if (pid == 0) {
        // Creamos y serializamos 3 objetos Dato
        buffer_envio[0] = {25, "Juan", "Perez"};
        buffer_envio[1] = {18, "Ana", "Gomez"};
        buffer_envio[2] = {32, "Luis", "Vega"};

        printf("PID 0: Enviando 3 objetos a todos los procesadores...\n");
        // El procesador 0 envía el buffer a todos
        for (int i = 0; i < nprocs; i++) {
            bsp_send(i, &tag, buffer_envio, sizeof(buffer_envio));
        }
    }

    bsp_sync(); // Sincronización para asegurar que los envíos se completen

    // SUPERPASO 2: Deserialización, cálculo y presentación de resultados
    int nm, nb, total;
    bsp_qsize(&nm, &nb);

    if (nm > 0) {
        DatoSerializado buffer_recibido[NUM_PERSONAS];
        bsp_get_tag(&total, &tag);

        if (total == sizeof(buffer_recibido)) {
            bsp_move(buffer_recibido, total);

            // Calculamos la edad máxima y mínima
            int edad_max = INT_MIN;
            int edad_min = INT_MAX;

            for (int i = 0; i < NUM_PERSONAS; i++) {
                if (buffer_recibido[i].edad > edad_max) {
                    edad_max = buffer_recibido[i].edad;
                }
                if (buffer_recibido[i].edad < edad_min) {
                    edad_min = buffer_recibido[i].edad;
                }
            }
            
            printf("PID %d: Edad máxima = %d, Edad mínima = %d\n", pid, edad_max, edad_min);

        } else {
            printf("PID %d: Error, el tamaño del mensaje recibido no es el esperado.\n", pid);
        }
    }

    bsp_end();
}

int main(int argc, char *argv[]) {
    bsp_init(bsp_main, argc, argv);
    bsp_main();
    return 0;
} 