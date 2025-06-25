# Concepto: All-to-All

La comunicación **All-to-All** (Todos a Todos) es uno de los patrones de comunicación colectiva más intensivos. En esta operación, cada procesador envía un mensaje individual a cada uno de los otros procesadores (y a menudo, a sí mismo). Si hay `P` procesadores, cada uno envía `P-1` o `P` mensajes y, a su vez, recibe `P-1` o `P` mensajes.

Este patrón se utiliza en algoritmos donde se necesita una redistribución completa de los datos, como en:
-   Transformadas de Fourier Rápidas (FFT).
-   Algunos algoritmos de ordenamiento paralelo (sort).
-   Multiplicación de matrices.
-   La solución de `sumaarreglo` en dos superpasos, donde cada procesador difunde su suma parcial a todos los demás.

## Implementación en BSP

Se implementa con un doble bucle o, más comúnmente, con un único bucle en el que cada procesador `p` itera sobre todos los posibles destinos `d` (de `0` a `nprocs-1`) y les envía un mensaje.

## Análisis del Ejemplo (`ejemplo_simple.cc`)

El ejemplo muestra a cada procesador enviando un dato único (su `pid`) a todos los demás.

### Flujo del Programa

1.  **Superpaso 0: Envío de Mensajes a Todos**
    -   **Cada procesador** itera y envía su propio `pid` a todos los procesadores (incluido él mismo).

    ```cpp
    // Todos los procesadores ejecutan esto
    for (int destino_pid = 0; destino_pid < nprocs; ++destino_pid) {
        bsp_send(destino_pid, &tag_data, &pid, sizeof(pid));
    }
    ```

2.  **Sincronización:**
    -   `bsp_sync()` asegura que los `P*P` mensajes se entreguen.
3.  **Superpaso 1: Recepción de Mensajes**
    -   **Cada procesador** sabe que debe recibir `nprocs` mensajes y los recoge en un `std::vector`.

    ```cpp
    // Todos los procesadores ejecutan esto
    std::vector<int> pids_recibidos(nprocs);
    for(int i = 0; i < nprocs; ++i) {
        bsp_move(&pids_recibidos[i], sizeof(int));
    }
    // ... (imprimir el vector) ...
    ```

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_simple ejemplo_simple.cc

# Ejecutar con varios procesadores
bsprun -n 4 ./ejemplo_simple
``` 