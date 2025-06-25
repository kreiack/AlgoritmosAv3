# 03 - Comunicación Básica Punto a Punto

El siguiente paso después de que los procesos pueden identificarse es hacer que se comuniquen entre sí. El modelo más simple es la comunicación **punto a punto**, donde un proceso envía un mensaje directamente a otro.

Este ejemplo implementa el envío de un único número entero desde el proceso `0` al proceso `1`.

## Funciones Clave

-   `bsp_send(pid_dest, tag, payload, size)`: Envía un mensaje.
    -   `pid_dest`: El ID del proceso de destino.
    -   `tag`: Una "etiqueta" para identificar el tipo de mensaje. Se configura previamente con `bsp_set_tagsize`.
    -   `payload`: Un puntero al dato que se quiere enviar.
    -   `size`: El tamaño en bytes del dato (`sizeof(dato)`).
-   `bsp_sync()`: Una barrera de sincronización. Es el corazón del modelo BSP. Garantiza que todos los mensajes enviados con `bsp_send` en un superpaso estarán disponibles para ser leídos en el siguiente superpaso.
-   `bsp_move(dest, size)`: Mueve un mensaje recibido de la cola de entrada a una variable local.
    -   `dest`: Un puntero a la variable donde se guardará el dato.
    -   `size`: El tamaño en bytes del dato que se espera recibir.

## Código Fuente (`03_Comunicacion_Basica.cc`)

La lógica se separa por roles usando `if (pid == ...)`:
-   **Proceso 0 (Emisor):** Envía un entero al proceso 1.
-   **Proceso 1 (Receptor):** Recibe el entero y lo imprime.
-   **Otros Procesos:** No hacen nada en la comunicación.

```cpp
#include <bsp.h>
#include <stdio.h>

void bsp_main() {
    bsp_begin(bsp_nprocs());

    int pid = bsp_pid();
    int nprocs = bsp_nprocs();

    // 1. Superpaso de Envío
    if (pid == 0) {
        if (nprocs > 1) {
            int mensaje = 2024;
            printf("Proceso %d enviando el valor %d al proceso 1...\\n", pid, mensaje);
            bsp_send(1, NULL, &mensaje, sizeof(int));
        }
    }

    bsp_sync(); // Barrera: Asegura que el envío se complete antes de la recepción

    // 2. Superpaso de Recepción
    if (pid == 1) {
        int mensaje_recibido;
        bsp_move(&mensaje_recibido, sizeof(int));
        printf("Proceso %d recibió el valor: %d\\n", pid, mensaje_recibido);
    }

    bsp_end();
}

int main(int argc, char **argv) {
    bsp_init(bsp_main, argc, argv);
    bsp_main();
    return 0;
}
```

## Compilación y Ejecución

Se necesitan al menos 2 procesos para que la comunicación ocurra.

```bash
bspcc -o comunicacion_exec 03_Comunicacion_Basica.cc
bsprun -np 2 ./comunicacion_exec
```

### Salida de la Ejecución

La salida confirma que el proceso 0 envió el mensaje y el proceso 1 lo recibió después de la barrera de sincronización.

```text
Proceso 0 enviando el valor 2024 al proceso 1...
Proceso 1 recibió el valor: 2024
```
Nuevamente, el orden de las líneas puede variar, pero el contenido demuestra que la comunicación fue exitosa. 