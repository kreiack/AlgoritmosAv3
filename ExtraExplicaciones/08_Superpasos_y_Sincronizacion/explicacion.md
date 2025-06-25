# Concepto: Superpasos y Sincronización (`bsp_sync`)

El modelo de Cómputo Síncrono en Masa (Bulk Synchronous Parallel, BSP) no se basa en interacciones continuas y asíncronas, sino que estructura los programas en una secuencia de **Superpasos**. Cada superpaso está separado del siguiente por una **barrera de sincronización global**.

Esta estructura impone un orden y una previsibilidad al flujo del programa paralelo, eliminando muchas de las condiciones de carrera y los interbloqueos (deadlocks) comunes en otros modelos.

## Anatomía de un Superpaso

Cada superpaso consiste en tres fases:

1.  **Cómputo Local:** Cada procesador trabaja de forma independiente sobre los datos que posee localmente en su memoria. No hay comunicación entre procesadores en esta fase.
2.  **Comunicación Global:** Cada procesador envía y recibe mensajes para intercambiar datos. En `BSPlib`, esto se hace típicamente con `bsp_send()`, `bsp_put()`, o `bsp_get()`. Las operaciones no son instantáneas; solo se *inician* en esta fase.
3.  **Sincronización de Barrera:** Esta es la fase crucial, marcada por la llamada a `bsp_sync()`. Cuando un procesador llega a `bsp_sync()`, se detiene y espera hasta que **todos** los demás procesadores también hayan llegado a su respectiva llamada a `bsp_sync()`. Además, y más importante, la barrera garantiza que todas las comunicaciones iniciadas en la fase 2 se hayan completado. Es decir, después de un `bsp_sync()`, un procesador tiene la certeza de que todos los mensajes que le fueron enviados en el superpaso anterior están disponibles en su buffer de entrada.

## Análisis del Ejemplo (`ejemplo_pingpong.cc`)

Este ejemplo ilustra la naturaleza de los superpasos con un "ping-pong" entre dos procesadores.

### Flujo del Programa

-   **Superpaso 0: PING**
    -   **Cómputo/Comunicación:** El Procesador 0 envía un mensaje "PING" al Procesador 1.
    -   **Sincronización:** `bsp_sync()`.

    ```cpp
    // --- SUPERPASO 0 ---
    if (bsp_pid() == 0) {
        int ping_msg = 1;
        bsp_send(1, &tag_data, &ping_msg, sizeof(ping_msg));
    }
    bsp_sync();
    ```

-   **Superpaso 1: PONG y Procesamiento del PING**
    -   **Cómputo/Comunicación:** El Procesador 1 recibe el PING, lo procesa y envía un "PONG" de vuelta.
    -   **Sincronización:** `bsp_sync()`.

    ```cpp
    // --- SUPERPASO 1 ---
    if (bsp_pid() == 1) {
        int received_msg;
        bsp_move(&received_msg, sizeof(received_msg)); // Recibe PING
        
        int pong_msg = 2;
        bsp_send(0, &tag_data, &pong_msg, sizeof(pong_msg)); // Envía PONG
    }
    bsp_sync();
    ```

-   **Superpaso 2: Procesamiento del PONG**
    -   **Cómputo:** El Procesador 0 recibe el PONG.
    -   **Sincronización:** `bsp_sync()` final.
    
    ```cpp
    // --- SUPERPASO 2 ---
    if (bsp_pid() == 0) {
        int received_msg;
        bsp_move(&received_msg, sizeof(received_msg)); // Recibe PONG
        printf("PID %d: Recibido PONG (%d).\n", bsp_pid(), received_msg);
    }
    bsp_sync(); // En un programa real, podría haber más superpasos.
    ```

Este ejemplo demuestra que la respuesta a un mensaje (`PONG`) no puede ocurrir en el mismo superpaso que el envío del mensaje original (`PING`). Se necesita una barrera de sincronización para asegurar que el mensaje se ha recibido antes de poder actuar sobre él.

## Para Compilar y Ejecutar

```bash
# Compilar
bspcxx -o ejemplo_pingpong ejemplo_pingpong.cc

# Ejecutar con 2 procesadores
bsprun -n 2 ./ejemplo_pingpong
``` 