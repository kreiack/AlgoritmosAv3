# 🎓 Base de Conocimiento de Cómputo Paralelo con BSP

Este directorio es una guía de referencia y aprendizaje sobre los patrones de comunicación y conceptos clave en el modelo **Bulk Synchronous Parallel (BSP)**. Cada subdirectorio contiene una explicación teórica detallada y un ejemplo de código minimalista que ilustra el concepto en acción.

> Navegue por los directorios para ver las explicaciones completas y el código fuente asociado.

---

## 📖 Índice Temático

| Sección                                                              | Concepto Clave                                        | Descripción Breve                                                                                                                      |
| -------------------------------------------------------------------- | ----------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
|  फाउंडेशन [**00. Conceptos Fundamentales**](./00_Conceptos_Fundamentales/) | **Primeros Pasos**                                    | Cómo inspeccionar el hardware, compilar y ejecutar, y los comandos básicos como `bsp_pid` y `bsp_nprocs`. El punto de partida esencial. |
| ➡️ [**01. Comunicación Punto a Punto**](./01_Punto_a_Punto/)         | **One-to-One**                                        | La operación más fundamental: un proceso envía un mensaje directamente a otro.                                                         |
| 📢 [**02. Broadcast**](./02_Broadcast/)                               | **One-to-All**                                        | Un proceso raíz envía la misma pieza de información a todos los demás procesos en el sistema.                                          |
| 흩어짐 [**03. Scatter**](./03_Scatter/)                                   | **One-to-All (Datos Únicos)**                         | Un proceso raíz distribuye porciones únicas y contiguas de un conjunto de datos a cada uno de los procesos.                               |
| 📥 [**04. Gather**](./04_Gather/)                                     | **All-to-One**                                        | La operación inversa a `Scatter`. Un proceso raíz recolecta una pieza de información de cada uno de los demás.                          |
| ➕ [**05. Reducción**](./05_Reduccion/)                                | **All-to-One (con Operación)**                        | Similar a `Gather`, pero aplicando una operación (suma, máximo, etc.) para combinar los resultados en un único valor.                |
| 🌐 [**06. All-to-All**](./06_All_to_All/)                               | **All-to-All**                                        | El patrón más denso: cada procesador envía un mensaje individual y único a cada uno de los otros procesadores.                         |
| 📦 [**07. Serialización**](./07_Serializacion/)                       | **Empaquetado de Datos**                              | La técnica crucial para convertir objetos y estructuras de datos complejas en un buffer de bytes para su transmisión.                   |
| ⏳ [**08. Superpasos y Sincronización**](./08_Superpasos_y_Sincronizacion/) | **El Corazón de BSP**                                 | Explora el concepto de **superpaso** y cómo la barrera `bsp_sync` organiza el cómputo y la comunicación en fases discretas.          |

---
[Volver al README Principal](../README.md) 