# Base de Conocimiento de Cómputo Paralelo con BSP

Este directorio es una guía de referencia y aprendizaje sobre los patrones de comunicación y conceptos clave en el modelo **Bulk Synchronous Parallel (BSP)**. Cada subdirectorio contiene una explicación teórica detallada (`.md`) y un ejemplo de código minimalista (`.cc`) que ilustra el concepto en acción.

---

## Índice Temático

### [00. Conceptos Fundamentales](./00_Conceptos_Fundamentales/)
Esta sección es el punto de partida. Antes de escribir código paralelo, es crucial entender el entorno y los comandos más básicos.
-   **Inspección del Entorno:** Cómo usar comandos del sistema (`sysctl`) para descubrir las capacidades del hardware (CPU, núcleos).
-   **Hola Mundo Paralelo:** El programa más elemental. Demuestra cómo iniciar el entorno BSP, obtener el ID de un proceso (`bsp_pid`) y el número total de procesos (`bsp_nprocs`).
-   **Comunicación Básica:** Un ejemplo de envío punto a punto de un solo dato para ilustrar el ciclo `bsp_send` -> `bsp_sync` -> `bsp_move`.

### [01. Comunicación Punto a Punto](./01_Punto_a_Punto/explicacion.md)
La operación más fundamental, donde un proceso envía un mensaje directamente a otro. Es el bloque de construcción para patrones más complejos.

### [02. Broadcast (Uno a Todos)](./02_Broadcast/explicacion.md)
Un patrón de comunicación colectiva donde un único proceso (la raíz) envía la misma pieza de información a todos los demás procesos en el sistema.

### [03. Scatter (Dispersión)](./03_Scatter/explicacion.md)
Un proceso raíz posee un conjunto de datos (ej. un arreglo) y distribuye porciones contiguas y únicas de ese conjunto a cada uno de los procesos.

### [04. Gather (Recolección)](./04_Gather/explicacion.md)
La operación inversa a `Scatter`. Cada proceso tiene una pieza de información, y un proceso raíz las recolecta todas, ensamblándolas en un conjunto de datos más grande.

### [05. Reducción (Todos a Uno con Operación)](./05_Reduccion/explicacion.md)
Similar a `Gather`, pero en lugar de simplemente recolectar los datos, se aplica una operación asociativa (como suma, producto, máximo o mínimo) en el camino, resultando en un único valor final en el proceso raíz.

### [06. All-to-All (Todos a Todos)](./06_All_to_All/explicacion.md)
El patrón de comunicación más denso, donde cada procesador envía un mensaje individual y único a cada uno de los otros procesadores.

### [07. Serialización](./07_Serializacion/explicacion.md)
Una técnica crítica de programación que consiste en convertir estructuras de datos complejas (como clases u objetos con punteros) en un formato plano (un buffer de bytes) que pueda ser transmitido de forma segura a través de la red a otro proceso.

### [08. Superpasos y Sincronización](./08_Superpasos_y_Sincronizacion/explicacion.md)
Explora el corazón del modelo BSP: el concepto de **superpaso**. Aclara cómo la barrera `bsp_sync` separa el cómputo local de la comunicación global, garantizando que todos los mensajes de un superpaso estén disponibles en el siguiente. 