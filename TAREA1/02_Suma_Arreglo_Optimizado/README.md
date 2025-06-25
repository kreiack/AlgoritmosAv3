# 1️⃣ Análisis y Solución: Suma de Arreglo Optimizada con Árboles

> Análisis y ejemplo de código del concepto **Análisis y Solución: Suma de Arreglo Optimizada con Árboles**.

---

Este documento detalla una solución de alto rendimiento para el problema de la suma de un arreglo distribuido, contrastándola con un enfoque más simple pero menos escalable.

## 1. Análisis del Problema y la Solución Base

El objetivo es sumar un arreglo distribuido entre `P` procesadores. Una solución directa, como la implementada en `sumaarreglo_resuelto.cc`, resuelve esto en **dos superpasos** mediante una comunicación **"todos a todos"**.

### 1.1. Estrategia Base: "Todos a Todos"
1.  **Superpaso 1:** Cada procesador calcula su suma parcial y la envía a *todos* los demás procesadores.
2.  **Superpaso 2:** Cada procesador recibe `P-1` sumas parciales, las suma a la suya y obtiene el total.

### 1.2. Identificación del Problema: Cuello de Botella Comunicacional
Si bien es funcional, este enfoque tiene un costo de comunicación que escala cuadráticamente (O(P²)). Cada procesador envía `P-1` mensajes, resultando en `P*(P-1)` transferencias totales. A medida que `P` crece, la red se congestiona y el rendimiento decae drásticamente.

## 2. Análisis de la Solución Optimizada (`sumaarreglooptimizado_resuelto.cc`)

Para superar la limitación de escalabilidad, se implementa un algoritmo basado en un **árbol de reducción y broadcast**. Esta estrategia reduce el costo comunicacional a O(P) y requiere O(log P) superpasos.

### 2.1. Estrategia de Corrección: Comunicación Jerárquica

Se abandona el modelo plano "todos a todos" por una estructura de árbol binario, donde la información fluye de manera jerárquica.

1.  **Fase de Reducción (Hacia Arriba):** Los resultados se combinan progresivamente hacia un procesador raíz (P0).
2.  **Fase de Broadcast (Hacia Abajo):** El resultado final se disemina desde la raíz hacia las hojas.

### 2.2. Flujo de la Solución Corregida

#### **Fase 1: Reducción en Árbol**
En `log2(P)` superpasos, los procesadores se emparejan para combinar sus sumas parciales. La lógica `bitwise` determina qué procesador envía y cuál recibe en cada paso.

```cpp
// Lógica de la fase de Redución
for (int k = 0; k < n_superpasos; ++k) {
    bsp_sync();
    int mask = 1 << k;
    if ((pid & mask) != 0) { // Rol: Emisor
        int dest_pid = pid & ~mask;
        bsp_send(dest_pid, &tag, &suma_parcial, sizeof(suma_parcial));
        break; 
    } else { // Rol: Receptor
        int src_pid = pid | mask;
        if (src_pid < nprocs) { /* Recibir y sumar */ }
    }
}
```

#### **Fase 2: Broadcast en Árbol**
Una vez P0 tiene el total, lo distribuye siguiendo el mismo patrón de árbol, pero en sentido inverso.

```cpp
// Lógica de la fase de Broadcast
for (int k = n_superpasos - 1; k >= 0; --k) {
    bsp_sync();
    int mask = 1 << k;
    if ((pid & mask) == 0) { // Rol: Emisor
        int dest_pid = pid | mask;
        if (dest_pid < nprocs) {
            bsp_send(dest_pid, &tag, &suma_global, sizeof(suma_global));
        }
    } else { // Rol: Receptor
        /* Recibir y almacenar suma_global */
        break;
    }
}
```

## 3. El Corazón del Algoritmo: Lógica Bitwise

La asignación de roles (emisor/receptor) y parejas de comunicación se gestiona eficientemente con operaciones de bits, lo que evita la necesidad de estructuras de datos complejas para representar el árbol.

-   **`mask = 1 << k`**: Genera una máscara para aislar el `k`-ésimo bit.
-   **`pid & mask`**: Determina el rol. Si el resultado es no nulo, el `k`-ésimo bit del `pid` está encendido (emisor).
-   **`pid & ~mask`** y **`pid | mask`**: Calculan el `pid` del procesador pareja apagando o encendiendo el `k`-ésimo bit.

Esta técnica es la que confiere al algoritmo su elegancia y eficiencia.

## 4. Resultado de la Ejecución

Al ejecutar el código optimizado con 4 procesadores (`bsprun -np 4 ...`), la salida demuestra que todos los procesadores han recibido la suma total final después de las fases de reducción y broadcast. El orden de las impresiones puede variar.

```text
PID 0: La suma global final (optimizada) es = 12534
PID 2: La suma global final (optimizada) es = 12534
PID 3: La suma global final (optimizada) es = 12534
PID 1: La suma global final (optimizada) es = 12534
``` 

---
[Volver al Índice Anterior](../README.md) | [Volver al README Principal](../../README.md)

> *Documentación mejorada con la asistencia de IA.*
