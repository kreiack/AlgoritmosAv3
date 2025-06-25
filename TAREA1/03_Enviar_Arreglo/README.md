# ➡️ Tarea: Enviar un Arreglo Completo

> Análisis del problema de enviar un arreglo de datos de un proceso a otro, contrastando un código inicial erróneo con una solución correcta basada en comunicación punto a punto.

---

## 1. Problema: Lógica Incorrecta en Código Original

El objetivo de la tarea es claro: **enviar un arreglo de N datos del procesador 0 al procesador 1**. Sin embargo, el código de partida (`ejemplo2.cc.txt`) falla en dos aspectos clave:
1.  **Operación Incorrecta:** Implementa un *broadcast* (uno a todos) en lugar de una comunicación *punto a punto* (uno a uno).
2.  **Tipo de Dato Incorrecto:** Envía un único `int` en lugar de un arreglo.

El resultado de este código sería que todos los procesadores imprimirían haber recibido un único número, lo cual no cumple el requisito.

## 2. Lógica de la Solución (`ejemplo2_resuelto.cc`)

La solución correcta modifica completamente la lógica para implementar una comunicación punto a punto selectiva.

### Superpaso 1: Creación y Envío del Arreglo
El procesador `0` inicializa un arreglo y lo envía directamente al procesador `1`. Es crucial notar que el `payload` de `bsp_send` es el puntero al arreglo y el tamaño es `N * sizeof(int)`.

```cpp
// Solo se ejecuta en el procesador 0
if (pid == 0) {
    const int N = 10;
    int arreglo[N];
    for (int i = 0; i < N; i++) {
        arreglo[i] = (i + 1) * 10;
    }
    
    // Envío directo al PID 1
    if (nprocs > 1) {
        printf("PID %d: Enviando arreglo al PID 1...\n", pid);
        bsp_send(1, &tag, arreglo, N * sizeof(int));
    }
}
bsp_sync();
```

### Superpaso 2: Recepción e Impresión del Arreglo
Solo el procesador `1` ejecuta la lógica de recepción. Mueve el bloque de memoria recibido a un arreglo local y lo imprime para verificar.

```cpp
// Solo se ejecuta en el procesador 1
if (pid == 1) {
    const int N = 10;
    int arreglo_recibido[N];
    bsp_move(arreglo_recibido, sizeof(arreglo_recibido)); // sizeof es seguro aquí
    
    printf("PID %d: Arreglo recibido:\n", pid);
    for (int i = 0; i < N; i++) {
        printf("  arreglo[%d] = %d\n", i, arreglo_recibido[i]);
    }
}
```

## 3. Resultado de la Ejecución

La salida del código corregido confirma que solo el procesador `1` recibe el arreglo completo, demostrando una comunicación punto a punto exitosa.

```text
PID 0: Enviando arreglo al PID 1...
PID 1: Arreglo recibido:
  arreglo[0] = 10
  arreglo[1] = 20
  ...
  arreglo[9] = 100
```
---
[Volver al Índice de Tareas](../README.md) | [Volver al README Principal](../../README.md)

> *Documentación mejorada con la asistencia de IA.*
