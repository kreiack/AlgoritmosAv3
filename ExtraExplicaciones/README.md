# Base de Conocimiento de Patrones BSP

Este directorio contiene explicaciones detalladas y ejemplos de código minimalistas para los conceptos y patrones de comunicación más importantes en el modelo **Bulk Synchronous Parallel (BSP)**.

## Conceptos Fundamentales

-   [**01. Comunicación Punto a Punto**](./01_Punto_a_Punto/explicacion.md)
    -   La operación más básica: un procesador envía un mensaje a otro.
-   [**02. Broadcast (Uno a Todos)**](./02_Broadcast/explicacion.md)
    -   Un proceso raíz envía la misma información a todos los demás.
-   [**03. Scatter (Dispersión)**](./03_Scatter/explicacion.md)
    -   Un proceso raíz reparte porciones únicas de un conjunto de datos a cada proceso.
-   [**04. Gather (Recolección)**](./04_Gather/explicacion.md)
    -   Un proceso raíz recolecta una pieza de información de cada uno de los demás procesos.
-   [**05. Reducción (Todos a Uno con Operación)**](./05_Reduccion/explicacion.md)
    -   Similar a un `Gather`, pero aplicando una operación (suma, máximo, etc.) para combinar los resultados en uno solo.
-   [**06. All-to-All (Todos a Todos)**](./06_All_to_All/explicacion.md)
    -   Cada procesador envía un mensaje individual a cada uno de los otros procesadores.
-   [**07. Serialización**](./07_Serializacion/explicacion.md)
    -   La técnica crucial para convertir objetos y estructuras de datos complejas en buffers de bytes para su transmisión.
-   [**08. Superpasos y Sincronización**](./08_Superpasos_y_Sincronizacion/explicacion.md)
    -   El concepto central del modelo BSP que organiza el cómputo y la comunicación en fases discretas. 