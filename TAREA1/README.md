# 📁 TAREA 1: Ejercicios Prácticos

Este directorio contiene la resolución de una serie de ejercicios prácticos de programación paralela utilizando el modelo BSP. Cada ejercicio se encuentra en su propia carpeta auto-contenida, la cual incluye el enunciado, el código fuente de la solución y una explicación detallada del enfoque utilizado.

> Navegue por los directorios para ver el análisis completo y el código fuente de cada ejercicio.

---

## 📖 Índice de Ejercicios

| Sección                                                              | Concepto Principal Aplicado                          | Descripción Breve                                                                                                                      |
| -------------------------------------------------------------------- | ---------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| 1️⃣ [**01. Suma de Arreglo (2 Pasos)**](./01_Suma_Arreglo_2_Pasos/)      | **Reducción (Todos a Todos)**                        | Resuelve la suma de un arreglo distribuido en exactamente dos superpasos, demostrando una implementación simple pero costosa del patrón de reducción. |
| 2️⃣ [**02. Suma de Arreglo (Optimizado)**](./02_Suma_Arreglo_Optimizado/) | **Reducción en Árbol**                               | Implementa una solución algorítmicamente eficiente (O(log P) superpasos) para la suma de arreglos, utilizando un árbol de reducción y broadcast. |
| ➡️ [**03. Enviar Arreglo**](./03_Enviar_Arreglo/)                        | **Comunicación Punto a Punto**                       | Demuestra cómo enviar un arreglo completo de un proceso a otro, un bloque de construcción fundamental.                                |
| 📦 [**04. Enviar Objeto Serializado**](./04_Enviar_Objeto_Serializado/)  | **Serialización y Broadcast**                        | Aborda el problema de enviar datos no primitivos (objetos/structs) empaquetándolos en un buffer de bytes antes de difundirlos a todos los procesos. |

---
[Volver al README Principal](../README.md)

> *Documentación mejorada con la asistencia de IA.*
