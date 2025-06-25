# ⏳ Concepto: Superpasos y Sincronización

> Análisis del corazón del modelo BSP: el concepto de **superpaso** y cómo la barrera `bsp_sync()` organiza el cómputo y la comunicación en fases discretas y predecibles.

---

## 1. El Modelo Bulk Synchronous Parallel (BSP)

El modelo BSP no se basa en eventos o mensajes individuales, sino en una secuencia de fases globales llamadas **superpasos**. Cada superpaso consta de tres componentes:

1.  **Cómputo Local:** Cada procesador realiza cálculos de forma independiente, utilizando únicamente los datos que posee localmente.
2.  **Comunicación Global:** Cada procesador puede enviar y recibir mensajes de cualquier otro procesador. En `bsponmpi`, esto se hace con `bsp_send`, `bsp_put`, o `bsp_get`.
3.  **Sincronización de Barrera:** Al final del superpaso, todos los procesadores deben llegar a una barrera (`bsp_sync()`). Ningún proceso puede avanzar al siguiente superpaso hasta que todos los demás hayan llegado a la barrera.

Esta barrera garantiza que toda la comunicación iniciada en un superpaso `S` se complete y esté disponible para ser procesada en el superpaso `S+1`.

> **🥪 No Hay Almuerzos Gratis:** La simplicidad y predictibilidad del modelo BSP tienen un costo. El "almuerzo" de no tener que lidiar con interbloqueos (`deadlocks`) complejos o condiciones de carrera se paga con el **costo de la sincronización de barrera**. El sistema debe esperar al proceso más lento en cada superpaso, lo que puede introducir tiempo de inactividad si las cargas de trabajo no están bien balanceadas.

## 2. Análisis del Ejemplo (`ejemplo_pingpong.cc`)

Este ejemplo ilustra la naturaleza de los superpasos con una comunicación de "ping-pong":
- **Superpaso 0:** El proceso 0 envía un "ping" al proceso 1.
- **Superpaso 1:** El proceso 1 recibe el "ping" y envía un "pong" de vuelta al proceso 0.
- **Superpaso 2:** El proceso 0 recibe el "pong".

Cada `bsp_sync()` es crucial para delimitar las fases y asegurar que un mensaje se recibe antes de que se envíe la respuesta.

```cpp
// Superpaso 0: P0 envía "ping"
if (pid == 0) {
    bsp_send(1, ...); 
}
bsp_sync();

// Superpaso 1: P1 recibe "ping" y envía "pong"
if (pid == 1) {
    bsp_move(...); // Recibe el ping
    bsp_send(0, ...); // Envía el pong
}
bsp_sync();

// Superpaso 2: P0 recibe "pong"
if (pid == 0) {
    bsp_move(...);
}
```

---
[Volver al Índice de Explicaciones](../README.md) | [Volver al README Principal](../../README.md)

> *Documentación mejorada con la asistencia de IA.* 