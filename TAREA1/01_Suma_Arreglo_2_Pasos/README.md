# 1️⃣ Tarea: Suma de Arreglo en 2 Superpasos

> Análisis de una solución directa pero costosa para la suma distribuida, implementando el patrón de reducción "todos a todos" para cumplir el requisito de solo dos superpasos.

---

## 1. Objetivo del Ejercicio

El requisito es **sumar los elementos de un gran arreglo distribuido entre todos los procesadores, resolviéndolo en exactamente dos superpasos.**

Para lograr esto, se utiliza una estrategia de comunicación colectiva "todos a todos", que si bien es funcional, introduce una alta carga comunicacional.

## 2. Lógica de la Solución (`sumaarreglo_resuelto.cc`)

### Superpaso 1: Suma Local y Difusión Total

1.  **Suma Local:** Cada procesador calcula la suma de la porción del arreglo que le corresponde (`suma_parcial`).
2.  **Comunicación "Todos a Todos":** En lugar de enviar la suma solo a un nodo raíz, cada procesador envía su `suma_parcial` a **todos** los demás procesadores (incluyéndose a sí mismo).

```cpp
// Cada procesador calcula su suma parcial
long long suma_parcial = 0;
// ... bucle de suma local ...

// Cada procesador envía su suma a todos los demás
for (int i = 0; i < nprocs; i++) {
    bsp_send(i, &tag, &suma_parcial, sizeof(suma_parcial));
}

bsp_sync();
```

### Superpaso 2: Suma Global y Verificación

1.  **Recepción y Suma Global:** Después de la sincronización, la cola de mensajes de cada procesador contiene las `P` sumas parciales de toda la red. Cada uno las lee y las acumula en una variable `suma_total`.
2.  **Resultado Final:** Al final de este superpaso, **todos los procesadores** conocen la `suma_total` correcta. El procesador `0` la imprime para verificar.

```cpp
// Cada procesador recibe P sumas y calcula el total
long long suma_total = 0;
bsp_qsize(&nm, &nb); // nm == nprocs
for (int i = 0; i < nm; i++) {
    long long suma_recibida;
    bsp_move(&suma_recibida, sizeof(suma_recibida));
    suma_total += suma_recibida;
}

if (pid == 0) {
    printf("La suma total es %lld\n", suma_total);
}
```
> **🥪 No Hay Almuerzos Gratis:** Esta técnica combina implícitamente una **reducción** y un **broadcast**, logrando el objetivo en solo dos superpasos. Sin embargo, el "almuerzo" no es gratis: se paga con un costo comunicacional de **O(P²)**, lo que la hace ineficiente para un gran número de procesadores.

## 3. Resultado de la Ejecución

La ejecución del código con 4 procesadores (`bsprun -np 4 ...`) produce la siguiente salida, donde el procesador 0 reporta la suma final:

```text
PID 0: La suma total (calculada en 2 superpasos) es = 258265
```

---
[Volver al Índice de Tareas](../README.md) | [Volver al README Principal](../../README.md)

> *Documentación mejorada con la asistencia de IA.*
