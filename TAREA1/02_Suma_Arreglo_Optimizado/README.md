# 2️⃣ Tarea: Suma de Arreglo Optimizada

> Análisis de una solución algorítmicamente eficiente para la suma distribuida, que utiliza un patrón de **árbol de reducción y broadcast** para minimizar la comunicación y los superpasos.

---

## 1. Deficiencia del Enfoque Base

La solución en dos superpasos (ver ejercicio anterior) utiliza una comunicación "todos a todos" que, si bien es simple, tiene un costo comunicacional de **O(P²)**. Esto se convierte en un cuello de botella a medida que aumenta el número de procesadores (`P`), haciendo que la solución no sea escalable.

## 2. Estrategia Optimizada: Comunicación en Árbol

Para superar esta limitación, se implementa un algoritmo basado en un **árbol de reducción y broadcast**. Esta estrategia reduce el costo comunicacional a **O(P)** y requiere **O(log P)** superpasos.

1.  **Fase de Reducción (Hacia Arriba):** Los resultados se combinan progresivamente. En cada superpaso, la mitad de los procesadores activos envían sus sumas parciales a la otra mitad, que las recibe y las agrega a su propia suma. El proceso se repite hasta que el procesador raíz (P0) acumula la suma total.
2.  **Fase de Broadcast (Hacia Abajo):** Una vez que P0 tiene el resultado final, se inicia el proceso inverso. P0 utiliza `bsp_put` para escribir directamente el resultado en la memoria de los otros procesadores, diseminando la información eficientemente.

## 3. Lógica de la Solución (`sumaarreglooptimizado_resuelto.cc`)

### Fase 1: Reducción en Árbol
La asignación de roles (emisor/receptor) en cada superpaso se gestiona con operaciones de bits, lo que evita la necesidad de estructuras de datos complejas.

```cpp
// Lógica de la fase de Redución
int n_superpasos = (nprocs > 1) ? floor(log2(nprocs - 1)) + 1 : 0;
for (int k = 0; k < n_superpasos; ++k) {
    int mask = 1 << k;
    if ((pid & mask) != 0) { // Rol: Emisor
        int dest_pid = pid & ~mask;
        bsp_send(dest_pid, &tag, &suma_parcial, sizeof(suma_parcial));
    } else { // Rol: Receptor
        int src_pid = pid | mask;
        if (src_pid < nprocs) { /* Recibir y sumar */ }
    }
    bsp_sync();
}
```

### Fase 2: Broadcast Directo con `bsp_put`
En lugar de un árbol de envío manual, se usa una comunicación de un solo lado más robusta.

```cpp
// Lógica de la fase de Broadcast
long long suma_global = suma_parcial; 
bsp_push_reg(&suma_global, sizeof(long long));
bsp_sync();

if (pid == 0) {
    for (int i = 1; i < nprocs; i++) {
        bsp_put(i, &suma_global, &suma_global, 0, sizeof(long long));
    }
}
bsp_sync();
bsp_pop_reg(&suma_global);
```

## 4. Resultado de la Ejecución

La salida demuestra que todos los procesadores han recibido la suma total final después de las fases de reducción y broadcast.

```text
PID 0: La suma global final (optimizada) es = 12534
PID 2: La suma global final (optimizada) es = 12534
PID 3: La suma global final (optimizada) es = 12534
PID 1: La suma global final (optimizada) es = 12534
```

---
[Volver al Índice de Tareas](../README.md) | [Volver al README Principal](../../README.md)

> *Documentación mejorada con la asistencia de IA.*
