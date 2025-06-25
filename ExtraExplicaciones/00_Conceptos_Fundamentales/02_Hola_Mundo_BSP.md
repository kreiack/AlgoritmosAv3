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