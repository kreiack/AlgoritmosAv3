# Concepto: Scatter (Dispersión)

El patrón **Scatter** es una operación de comunicación colectiva en la que un proceso raíz divide un conjunto de datos (normalmente un arreglo) en `P` trozos y envía un trozo único a cada uno de los `P` procesadores. El procesador `i` recibe el `i`-ésimo trozo del arreglo.

A diferencia del **Broadcast** (donde todos reciben los *mismos* datos), en un **Scatter**, cada procesador recibe una porción *diferente* y exclusiva de los datos originales.

Este patrón es fundamental para el paradigma de "dividir y vencer" en computación paralela, donde un problema grande se descompone en subproblemas más pequeños que se resuelven en paralelo.

## Implementación en BSP

Al igual que el Broadcast, BSPlib no suele tener una primitiva `bsp_scatter()`. Se implementa manualmente. El proceso raíz calcula los desplazamientos (offsets) dentro del arreglo de datos y envía la porción correspondiente a cada procesador mediante `bsp_send()`.

## Análisis del Ejemplo (`ejemplo_simple.cc`)

El código de ejemplo muestra cómo un proceso raíz (pid 0) "esparce" un arreglo de enteros entre todos los procesadores.

### Flujo del Programa

1.  **Inicialización:**
    - Se define un `DATOS_POR_PROCESADOR`.
    - El número total de datos será `nprocs * DATOS_POR_PROCESADOR`.
2.  **Superpaso 0: Creación de Datos y Dispersión (solo en Raíz)**
    -   El **Procesador 0** itera, calcula un puntero a cada trozo del arreglo y lo envía al procesador correspondiente.

    ```cpp
    // Solo en el Procesador 0
    if (pid == 0) {
        std::vector<int> arreglo_global(N_GLOBAL);
        // ... (se llena el arreglo) ...

        for (int i = 0; i < nprocs; ++i) {
            int* puntero_trozo = &arreglo_global[i * DATOS_POR_PROCESADOR];
            bsp_send(i, &tag_data, puntero_trozo, DATOS_POR_PROCESADOR * sizeof(int));
        }
    }
    ```
3.  **Sincronización:**
    -   `bsp_sync()` asegura que todos los trozos de datos se entreguen.
4.  **Superpaso 1: Recepción**
    -   **Cada procesador** recibe su trozo único en un arreglo local.

    ```cpp
    // Todos los procesadores ejecutan esto
    std::vector<int> arreglo_local(DATOS_POR_PROCESADOR);
    bsp_move(arreglo_local.data(), DATOS_POR_PROCESADOR * sizeof(int));

    printf("Procesador %d: He recibido los datos: [...", pid);
    // ... (se imprimen los datos) ...
    ```

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_simple ejemplo_simple.cc

# Ejecutar con varios procesadores
bsprun -n 4 ./ejemplo_simple
``` 