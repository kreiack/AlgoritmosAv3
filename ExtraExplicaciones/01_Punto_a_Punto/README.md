#  # Concepto: Comunicación Punto a Punto


---

La comunicación **Punto a Punto** es la operación más fundamental en la computación paralela. Consiste en el envío de un mensaje desde un proceso de origen a un único proceso de destino. Es el bloque de construcción sobre el cual se implementan patrones de comunicación más complejos.

## Componentes Clave en BSP

-   `bsp_send(pid_destino, tag, payload, tamaño)`: Envía un mensaje.
    -   `pid_destino`: El ID del proceso que recibirá el mensaje.
    -   `tag`: Una "etiqueta" para identificar el mensaje. Debe ser registrada y su tamaño configurado previamente.
    -   `payload`: Un puntero al bloque de memoria (los datos) que se quiere enviar.
    -   `tamaño`: El tamaño del payload en bytes.
-   `bsp_move(destino, tamaño)`: Mueve un mensaje recibido desde la cola de entrada del procesador a una variable local.
-   `bsp_sync()`: La barrera de sincronización que finaliza un superpaso y garantiza que todos los mensajes enviados con `bsp_send` han sido entregados.

## Análisis del Ejemplo (`ejemplo_simple.cc`)

El código de ejemplo demuestra este concepto de la forma más simple posible:
- **Procesador 0** envía un único número entero al **Procesador 1**.

### Flujo del Programa

1.  **Superpaso 0: Envío**
    -   El **Procesador 0** define un `dato_a_enviar` y llama a `bsp_send()`.

    ```cpp
    // Solo en el Procesador 0
    if (bsp_pid() == 0) {
        int dato_a_enviar = 42;
        bsp_send(1, &tag_data, &dato_a_enviar, sizeof(dato_a_enviar));
    }
    ```

2.  **Sincronización:**
    -   La llamada a `bsp_sync()` asegura que el mensaje llegue al buffer del Procesador 1.

3.  **Superpaso 1: Recepción**
    -   El **Procesador 1** verifica si ha llegado un mensaje y usa `bsp_move()` para leerlo.

    ```cpp
    // Solo en el Procesador 1
    if (bsp_pid() == 1) {
        int n_mensajes, tam_total_bytes;
        bsp_qsize(&n_mensajes, &tam_total_bytes);

        if (n_mensajes > 0) {
            int dato_recibido;
            bsp_move(&dato_recibido, sizeof(dato_recibido));
            printf("Procesador %d: He recibido el número %d.
", bsp_pid(), dato_recibido);
        }
    }
    ```

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_simple ejemplo_simple.cc

# Ejecutar con 2 o más procesadores
bsprun -n 2 ./ejemplo_simple
``` 

---
[Volver al Índice de Explicaciones](../README.md) | [Volver al README Principal](../../README.md)
