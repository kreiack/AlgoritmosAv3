# Concepto: Serialización

## El Problema: Enviar Datos Complejos

Las bibliotecas de comunicación paralela como BSPlib o MPI son expertas en enviar y recibir **bloques de memoria contiguos**, es decir, "buffers" de bytes. No tienen un conocimiento inherente sobre qué es un `struct`, una `class` de C++, un `std::vector` o cualquier otro tipo de dato complejo.

Intentar enviar un objeto complejo directamente (`bsp_send(..., &mi_objeto, ...)`), especialmente si contiene punteros o gestiona su propia memoria (como `std::string` o `std::vector`), resultará en un error. Solo se enviaría el contenido inmediato del objeto (por ejemplo, el puntero del vector, su tamaño y capacidad), pero no los datos a los que ese puntero apunta. El receptor recibiría una dirección de memoria que no tiene sentido en su propio contexto, causando un fallo.

## La Solución: Serialización y Deserialización

**Serialización** es el proceso de convertir una estructura de datos u objeto en un formato que se pueda transmitir: un simple buffer de bytes.
**Deserialización** es el proceso inverso: reconstruir el objeto original a partir del buffer de bytes recibido.

El programador es responsable de este proceso:
1.  **Serializar (antes de enviar):** Crear un buffer (p. ej. un `char[]` o `int[]`) y copiar manualmente los campos importantes del objeto en él en un orden bien definido.
2.  **Enviar:** Enviar el buffer serializado.
3.  **Recibir:** Recibir el buffer.
4.  **Deserializar (después de recibir):** Leer el buffer en el mismo orden en que fue escrito para reconstruir el objeto.

## Análisis del Ejemplo (`ejemplo_struct.cc`)

El código muestra cómo enviar un `struct Persona`.

```cpp
#define MAX_NOMBRE 50

struct Persona {
    int edad;
    char nombre[MAX_NOMBRE];
};
```

### Flujo del Programa

1.  **Definición:**
    - Se define un `struct Persona` simple, con campos de tamaño fijo.
2.  **Superpaso 0: Creación, Serialización y Envío (en Raíz)**
    -   Se crea un `buffer` de `char` y se copian los campos del `struct` a él usando `memcpy`.

    ```cpp
    // Solo en el Procesador 0
    Persona p_enviar;
    p_enviar.edad = 30;
    strncpy(p_enviar.nombre, "Juan Perez", MAX_NOMBRE);

    const int buffer_size = sizeof(int) + MAX_NOMBRE * sizeof(char);
    char buffer[buffer_size];
    
    char* ptr = buffer;
    memcpy(ptr, &p_enviar.edad, sizeof(int));
    ptr += sizeof(int);
    memcpy(ptr, p_enviar.nombre, MAX_NOMBRE * sizeof(char));

    bsp_send(1, &tag_data, buffer, buffer_size);
    ```

3.  **Sincronización**
4.  **Superpaso 1: Recepción y Deserialización (en Receptor)**
    -   Se recibe el `buffer` y se usa `memcpy` para extraer los bytes y reconstruir el `struct`.

    ```cpp
    // Solo en el Procesador 1
    char buffer[buffer_size];
    bsp_move(buffer, buffer_size);

    Persona p_recibida;
    char* ptr = buffer;

    memcpy(&p_recibida.edad, ptr, sizeof(int));
    ptr += sizeof(int);
    memcpy(p_recibida.nombre, ptr, MAX_NOMBRE * sizeof(char));

    printf("Recibido: {nombre: %s, edad: %d}\n", p_recibida.nombre, p_recibida.edad);
    ```

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_struct ejemplo_struct.cc

# Ejecutar con 2 o más procesadores
bsprun -n 2 ./ejemplo_struct
``` 