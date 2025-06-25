# Concepto: Broadcast (Uno a Todos)

Un **Broadcast** (o difusión) es una operación de comunicación colectiva fundamental en la que un único proceso, comúnmente denominado "raíz" (root), envía el mismo mensaje a todos los demás procesos en el grupo de cómputo (incluyéndose a sí mismo).

Este patrón es esencial cuando se necesita distribuir un dato o un conjunto de datos desde un nodo a todos los demás para que puedan iniciar un cálculo con la misma información base. Ejemplos de uso incluyen:
- Distribuir parámetros de configuración.
- Enviar una sección de una matriz o un vector a todos los nodos.
- Propagar una condición de parada.

## Implementación en BSP

A diferencia de MPI, `BSPlib` no suele proporcionar una primitiva `bsp_broadcast()` de alto nivel. En su lugar, el patrón se implementa manualmente utilizando la comunicación punto a punto.

El proceso raíz (generalmente el `pid` 0) simplemente itera a través de todos los ID de proceso, de `0` a `nprocs-1`, y realiza un `bsp_send()` a cada uno de ellos.

## Análisis del Ejemplo (`ejemplo_simple.cc`)

El código de ejemplo implementa un broadcast de un único entero.

### Flujo del Programa

1.  **Superpaso 0: Envío**
    -   El **Procesador 0** define un `dato_a_difundir` y lo envía a todos en un bucle.
    ```cpp
    // Solo en el Procesador 0
    if (pid == 0) {
        dato_a_difundir = 77;
        for (int i = 0; i < nprocs; ++i) {
            bsp_send(i, &tag_data, &dato_a_difundir, sizeof(dato_a_difundir));
        }
    }
    ```

2.  **Sincronización:**
    -   `bsp_sync()` asegura que todos los mensajes de `bsp_send` se entreguen.

3.  **Superpaso 1: Recepción**
    -   **Cada procesador** (incluido el 0) recibe el dato y lo imprime.
    ```cpp
    // Todos los procesadores ejecutan esto
    int dato_recibido;
    bsp_move(&dato_recibido, sizeof(dato_recibido));
    printf("Procesador %d: He recibido el número %d.\n", pid, dato_recibido);
    ```

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_simple ejemplo_simple.cc

# Ejecutar con varios procesadores
bsprun -n 4 ./ejemplo_simple
``` 