# Concepto: Reducción (Reduce)

Una operación de **Reducción** (o `reduce`) es una comunicación colectiva que combina datos de todos los procesos en un grupo para producir un único resultado final en un proceso raíz. Es similar a un `Gather`, pero implica realizar una operación matemática o lógica (como suma, producto, máximo, mínimo, AND, OR) sobre los datos a medida que se recolectan.

El nombre "reducción" viene del hecho de que un conjunto de `P` valores se "reduce" a un solo valor.

Este patrón es extremadamente común para agregar resultados parciales. Por ejemplo, si cada procesador calcula una suma parcial de una porción de un arreglo, una reducción con la operación `SUMA` puede calcular la suma total de forma eficiente.

## Implementación en BSP

Una reducción se puede implementar de varias maneras. La más simple (aunque no siempre la más eficiente en términos de superpasos) es una variante de `Gather`:
1.  Cada proceso envía su valor al proceso raíz.
2.  El proceso raíz recibe todos los valores y aplica la operación de reducción (suma, en este caso) de forma secuencial.

Implementaciones más avanzadas (como árboles de reducción) pueden realizar la operación en menos superpasos, pero son más complejas de codificar.

## Análisis del Ejemplo (`ejemplo_suma.cc`)

El ejemplo calcula la suma total de valores generados por cada procesador.

### Flujo del Programa

1.  **Superpaso 0: Cómputo Local y Envío a Raíz**
    -   Cada procesador genera un `valor_local` y lo envía al procesador raíz (pid 0).

    ```cpp
    // Todos los procesadores ejecutan esto
    int valor_local = pid + 1;
    bsp_send(0, &tag_data, &valor_local, sizeof(valor_local));
    ```

2.  **Sincronización:**
    -   `bsp_sync()` asegura que todos los valores locales lleguen a la raíz.
3.  **Superpaso 1: Reducción (solo en Raíz)**
    -   El **Procesador 0** recibe todos los valores y los acumula en una variable `suma_global`, aplicando la operación de reducción (suma).

    ```cpp
    // Solo en el Procesador 0
    if (pid == 0) {
        long long suma_global = 0;
        for(int i = 0; i < nprocs; ++i) {
            int valor_recibido;
            bsp_move(&valor_recibido, sizeof(valor_recibido));
            
            // Operación de reducción
            suma_global += valor_recibido;
        }
        printf("La suma total (reducción) es %lld.\n", suma_global);
    }
    ```

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_suma ejemplo_suma.cc

# Ejecutar con varios procesadores
bsprun -n 4 ./ejemplo_suma
``` 