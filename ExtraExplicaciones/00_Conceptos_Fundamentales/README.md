# 01 - Inspección del Entorno Computacional

Antes de escribir código paralelo, es fundamental comprender el hardware sobre el que se ejecutará. Conocer el número de procesadores y núcleos disponibles nos permite tomar decisiones informadas sobre cuántos procesos paralelos lanzar para obtener el máximo rendimiento.

## Comandos para macOS

En sistemas basados en Unix como macOS, el comando `sysctl` nos permite inspeccionar los parámetros del kernel y del hardware.

### 1. Modelo exacto de la CPU

Para saber con qué procesador estamos trabajando, podemos usar:

```bash
sysctl -n machdep.cpu.brand_string
```

### 2. Número de Núcleos Lógicos (Hilos de Ejecución)

Este es el número más relevante para `bsprun -np P`. Indica el máximo número de hilos que el sistema operativo puede ejecutar simultáneamente. Generalmente, no es recomendable usar un `P` mayor a este número.

```bash
sysctl -n hw.logicalcpu
```

### 3. Número de Núcleos Físicos

Indica cuántos núcleos de procesamiento reales tiene el chip, sin contar tecnologías como Hyper-Threading que los duplican a nivel lógico.

```bash
sysctl -n hw.physicalcpu
```

## Salida Real en su Sistema

A continuación, se muestra la salida de estos comandos ejecutados en su equipo para que sirva como referencia concreta.

```text
Modelo de CPU:
Apple M4 Max

Núcleos Lógicos:
16

Núcleos Físicos:
16
```

Estos datos nos indican que su computador cuenta con un procesador Apple M4 Max con 16 núcleos de alto rendimiento, lo que lo hace un sistema excepcionalmente potente para la experimentación en cómputo paralelo. Podríamos, por ejemplo, ejecutar hasta `bsprun -np 16` y cada proceso tendría un núcleo lógico a su disposición.

---

# 02 - Hola Mundo en Paralelo (BSP)

Este es el programa más fundamental en cualquier entorno de programación paralela. Su objetivo es simple: iniciar múltiples procesos y hacer que cada uno se identifique.

## Funciones Clave

-   `bsp_init(bsp_main, argc, argv)`: Inicializa el entorno BSP. Se le pasa la función que contiene la lógica paralela (`bsp_main`). Se llama desde `main`.
-   `bsp_begin(P)`: Marca el inicio de la sección de código paralelo. `P` es el número de procesos que participarán. `bsp_nprocs()` es la forma estándar de obtener este valor.
-   `bsp_pid()`: Devuelve el identificador (ID) del proceso que ejecuta la llamada. Es un número de `0` a `P-1`.
-   `bsp_nprocs()`: Devuelve el número total de procesos (`P`) que se lanzaron con `bsprun -np P`.
-   `bsp_end()`: Marca el fin de la sección de código paralelo.

## Código Fuente (`02_Hola_Mundo_BSP.cc`)

El código es directo: cada proceso obtiene su ID y el total de procesos, y luego imprime un mensaje formateado.

```cpp
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
```

## Compilación y Ejecución

Para compilar y ejecutar este código, usamos los comandos proporcionados por la biblioteca:

```bash
# Compilar
bspcc -o hola_mundo_exec 02_Hola_Mundo_BSP.cc

# Ejecutar con 4 procesos
bsprun -np 4 ./hola_mundo_exec
```

A continuación se muestra la salida real de la ejecución.

### Salida de la Ejecución

```text
Hola desde el proceso 0 de un total de 4 procesos.
Hola desde el proceso 3 de un total de 4 procesos.
Hola desde el proceso 1 de un total de 4 procesos.
Hola desde el proceso 2 de un total de 4 procesos.
```

**Observación Importante:** El orden en que aparecen los mensajes no está garantizado. En un sistema paralelo, el sistema operativo decide qué proceso se ejecuta en cada momento. Cada vez que se ejecute el programa, el orden de las líneas podría ser diferente. Esta es una de las características clave del no determinismo en la programación concurrente.

---

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