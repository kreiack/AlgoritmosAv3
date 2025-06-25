# Proyecto de Computación Paralela con BSP

Este repositorio contiene una colección de ejercicios resueltos y material de estudio sobre los patrones fundamentales de la programación paralela utilizando el modelo **Bulk Synchronous Parallel (BSP)** y la biblioteca `bsponmpi`.

## Estructura del Repositorio

El proyecto está organizado en dos directorios principales:

### 1. [`TAREA1/`](./TAREA1)

Este directorio contiene las soluciones a tres ejercicios específicos de programación paralela. Cada ejercicio incluye:
-   Un archivo `.cc.txt` con el enunciado original.
-   Un archivo `_resuelto.cc` con el código de la solución en C++.
-   Un archivo `.md` que explica el concepto paralelo subyacente utilizando el ejercicio como caso de estudio.

Los ejercicios cubren:
-   **Suma de Arreglo:** Implementación de una operación de **Reducción** en dos superpasos.
-   **Ejemplo 2:** Demostración de comunicación **Punto a Punto** para enviar un arreglo.
-   **Ejemplo 3:** Uso de **Serialización** para enviar objetos y realizar un **Broadcast**.

### 2. [`ExtraExplicaciones/`](./ExtraExplicaciones)

Este directorio es una base de conocimiento diseñada para ser un recurso de aprendizaje. Contiene explicaciones detalladas y ejemplos de código minimalistas para los conceptos y patrones de comunicación más importantes en BSP.

Cada subdirectorio se enfoca en un único concepto y contiene:
-   Un archivo `explicacion.md` con la teoría y el análisis del patrón.
-   Uno o más archivos de código `ejemplo_*.cc` claros y comentados.

Los temas cubiertos incluyen la comunicación punto a punto, broadcast, scatter, gather, reducción, all-to-all, serialización y el modelo de superpasos.

## Compilación y Ejecución

Todos los ejemplos de C++ que utilizan la biblioteca BSP deben ser compilados con el compilador `bspcxx` y ejecutados con `bsprun`.

```bash
# Para compilar un archivo
bspcxx -o mi_ejecutable mi_codigo.cc

# Para ejecutarlo con 4 procesadores
bsprun -n 4 ./mi_ejecutable
```

---
*Este repositorio fue desarrollado con la asistencia de un modelo de IA como herramienta de pair programming y tutoría.* 