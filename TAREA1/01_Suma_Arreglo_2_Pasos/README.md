#  Ejercicio de Cómputo Colectivo: Operación de Reducción

> Análisis y ejemplo de código del concepto **Ejercicio de Cómputo Colectivo: Operación de Reducción**.

---

## 1. ¿Qué es una Operación de Reducción?

En computación paralela, una **reducción** (o `reduce`) es una operación colectiva que combina datos de todos los procesadores para producir un único resultado final. Imagina que cada procesador tiene un número y quieres calcular la suma de todos ellos. Una reducción tomaría estos valores distribuidos y los combinaría (usando la operación de suma) en un solo procesador, usualmente el procesador raíz (pid 0).

Otras operaciones comunes de reducción incluyen encontrar el valor máximo, el mínimo o realizar un "Y" lógico.

## 2. Análisis del Ejercicio (`SumaArreglo.cc`)

El objetivo de esta tarea es **sumar los elementos de un gran arreglo que está distribuido entre todos los procesadores, y hacerlo en solo dos superpasos.** Este es un problema clásico que se resuelve de manera muy eficiente con el patrón de reducción.

La clave para resolverlo en dos superpasos es evitar centralizar todo en un solo procesador. En su lugar, se usa una estrategia distribuida.

### Lógica de la Solución en Dos Superpasos

#### **Superpaso 1: Suma Local y Distribución de Sumas Parciales**

1.  **Distribución de Datos:** Se asume que el arreglo `A` de tamaño `N` está distribuido. Cada uno de los `P` procesadores se encarga de una porción del arreglo.
2.  **Suma Local:** Cada procesador calcula la suma de los elementos que le corresponden. A este resultado lo llamamos `suma_parcial`.
3.  **Comunicación "Todos a Todos":** Cada procesador envía su `suma_parcial` a **todos** los demás procesadores (incluyéndose a sí mismo). Al final de esta fase, la cola de mensajes de cada procesador contiene las sumas parciales de toda la red.

```cpp
// Superpaso 1: Cada procesador calcula su suma parcial y la envía a todos.
long long suma_parcial = 0;
for (int i = 0; i < N; i++) {
    suma_parcial += A[i];
}

for (int i = 0; i < nprocs; i++) {
    bsp_send(i, &tag, &suma_parcial, sizeof(suma_parcial));
}

bsp_sync();
```

#### **Superpaso 2: Suma Global y Verificación**

1.  **Recepción y Suma Global:** Cada procesador procesa su cola de mensajes, leyendo las `P` sumas parciales que le han llegado y acumulándolas en una variable `suma_total`.
2.  **Resultado Final:** Al final de este superpaso, **todos los procesadores** tienen la `suma_total` correcta. El procesador `0` (o cualquiera de ellos) la imprime para verificar el resultado.

```cpp
// Superpaso 2: Cada procesador recibe las sumas parciales y calcula la suma total.
long long suma_total = 0;
for (int i = 0; i < nprocs; i++) {
    long long suma_recibida;
    bsp_move(&suma_recibida, sizeof(suma_recibida));
    suma_total += suma_recibida;
}

if (pid == 0) {
    printf("La suma total es %lld
", suma_total);
}
```

Esta técnica combina una **reducción** (calcular la suma total) con un **broadcast** (distribuir el resultado final a todos) de manera implícita en solo dos superpasos, a costa de una alta carga comunicacional (O(P²)).

El código de la solución se encuentra en `sumaarreglo_resuelto.cc`.

## 3. Resultado de la Ejecución

Al ejecutar el código con 4 procesadores (`bsprun -np 4 ...`), la salida muestra cómo el procesador 0 (y solo él, según la lógica del `printf`) reporta la suma final. El valor exacto de la suma variará en cada ejecución debido a la inicialización con números aleatorios.

```text
PID 0: La suma total (calculada en 2 superpasos) es = 258265
``` 

---
[Volver al Índice Anterior](../README.md) | [Volver al README Principal](../../README.md)

> *Documentación mejorada con la asistencia de IA.*
