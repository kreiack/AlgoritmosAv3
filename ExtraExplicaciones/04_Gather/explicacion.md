# Concepto: Gather (Recolección)

La operación **Gather** es la inversa de **Scatter**. Es una comunicación colectiva donde cada procesador envía un trozo de datos a un único proceso raíz, el cual los "recolecta" y los ensambla en un conjunto de datos más grande, usualmente un arreglo.

Si `Scatter` es "repartir el trabajo", `Gather` es "recoger los resultados". Este patrón es típico al final de un cómputo paralelo, donde cada nodo ha procesado su porción de datos y necesita enviar su resultado parcial de vuelta a un nodo maestro para el ensamblaje final.

## Implementación en BSP

Como en los casos anteriores, `Gather` se implementa manualmente. Cada proceso (incluido el raíz) utiliza `bsp_send()` para enviar su dato o trozo de datos al proceso raíz (pid 0). El proceso raíz, después de la sincronización, recibe todos los mensajes y los coloca en el lugar correcto dentro de un arreglo global.

## Análisis del Ejemplo (`ejemplo_simple.cc`)

El código de ejemplo demuestra cómo cada procesador genera un dato y el procesador raíz (pid 0) los recolecta todos.

### Flujo del Programa

1.  **Superpaso 0: Cómputo Local y Envío a Raíz**
    -   **Cada procesador** genera un dato y lo envía a la raíz (pid 0). Es crucial enviar el `pid` del remitente como `tag` para que la raíz sepa de quién es cada dato.

    ```cpp
    // Todos los procesadores ejecutan esto
    int dato_local = 100 * (pid + 1);
    // El PID se usa como tag para identificar al remitente
    bsp_send(0, &pid, &dato_local, sizeof(dato_local));
    ```
2.  **Sincronización:**
    -   `bsp_sync()` asegura que todos los datos enviados por los procesadores lleguen al buffer del proceso raíz.
3.  **Superpaso 1: Recolección y Ensamblaje (solo en Raíz)**
    -   El **Procesador 0** recibe los mensajes. Usa `bsp_get_tag()` para ver de quién es el siguiente mensaje antes de moverlo con `bsp_move()`, y así poder colocarlo en la posición correcta del arreglo.

    ```cpp
    // Solo en el Procesador 0
    if (pid == 0) {
        std::vector<int> arreglo_recolectado(nprocs);
        for(int i = 0; i < nprocs; ++i) {
            int pid_origen;
            int dato_recibido;

            bsp_get_tag(NULL, &pid_origen); // Espiar el tag (pid del remitente)
            bsp_move(&dato_recibido, sizeof(dato_recibido));

            arreglo_recolectado[pid_origen] = dato_recibido;
        }
        // ... (imprimir arreglo) ...
    }
    ```

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_simple ejemplo_simple.cc

# Ejecutar con varios procesadores
bsprun -n 4 ./ejemplo_simple
``` 