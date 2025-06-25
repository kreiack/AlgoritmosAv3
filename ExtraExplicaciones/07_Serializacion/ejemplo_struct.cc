extern "C" {
#include <bsp.h>
}
#include <stdio.h>
#include <string.h> // Para memcpy y strlen
#include <stdlib.h> // Para exit

#define MAX_NOMBRE 50

// Un struct simple. Es importante que para este tipo de serialización
// manual, los campos sean de tamaño fijo.
struct Persona {
    int edad;
    char nombre[MAX_NOMBRE];
};

void spmd_main() {
    bsp_begin(bsp_nprocs());

    if (bsp_nprocs() < 2) {
        if(bsp_pid() == 0) printf("Se requieren al menos 2 procesadores.\n");
        bsp_end();
        exit(1);
    }

    int tag_data = 0;
    bsp_set_tagsize(&tag_data);
    bsp_sync();

    const int buffer_size = sizeof(int) + MAX_NOMBRE * sizeof(char);
    char buffer[buffer_size];

    // ================== SUPERPASO 0 ==================
    if (bsp_pid() == 0) {
        // 1. Crear el objeto
        Persona p_enviar;
        p_enviar.edad = 30;
        strncpy(p_enviar.nombre, "Juan Perez", MAX_NOMBRE);
        
        printf("Procesador 0: Serializando y enviando a Persona: {nombre: %s, edad: %d}\n", p_enviar.nombre, p_enviar.edad);

        // 2. Serializar (copiar campos al buffer)
        char* ptr = buffer; // Puntero auxiliar para movernos por el buffer
        
        // Copiar la edad
        memcpy(ptr, &p_enviar.edad, sizeof(int));
        ptr += sizeof(int); // Mover el puntero
        
        // Copiar el nombre
        memcpy(ptr, p_enviar.nombre, MAX_NOMBRE * sizeof(char));

        // 3. Enviar el buffer
        bsp_send(1, &tag_data, buffer, buffer_size);
    }
    
    bsp_sync();
    // ================== SUPERPASO 1 ==================

    if (bsp_pid() == 1) {
        // 4. Recibir el buffer
        bsp_move(buffer, buffer_size);

        // 5. Deserializar (reconstruir el objeto desde el buffer)
        Persona p_recibida;
        char* ptr = buffer;

        // Extraer la edad
        memcpy(&p_recibida.edad, ptr, sizeof(int));
        ptr += sizeof(int);

        // Extraer el nombre
        memcpy(p_recibida.nombre, ptr, MAX_NOMBRE * sizeof(char));

        printf("Procesador 1: Recibido y deserializado. Persona: {nombre: %s, edad: %d}\n", p_recibida.nombre, p_recibida.edad);
    }

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(spmd_main, argc, argv);
    spmd_main();
    return 0;
} 