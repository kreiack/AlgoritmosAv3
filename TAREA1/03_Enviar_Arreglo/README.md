# 📝 Tarea: # Análisis y Solución: Enviar un Arreglo por Comunicación Punto a Punto

> Solución y análisis del ejercicio.

---

Este documento analiza la tarea de modificar un código para enviar un arreglo de `N` datos. Se parte de un código de ejemplo que realiza una operación diferente (un broadcast de un entero), se explica por qué no cumple el objetivo, y se presenta la solución correcta basada en comunicación punto a punto.

## 1. Análisis del Código Original (`ejemplo2.cc.txt`)

El objetivo de la tarea es claro: **enviar un arreglo de N datos del procesador 0 al procesador 1**. Sin embargo, el código de partida realiza una operación distinta.

### 1.1. Flujo del Código Original

El código implementa un **Broadcast**: el procesador `0` envía un único dato a *todos* los procesadores.

#### **Superpaso 1: Broadcast de un Entero**
El procesador `0` define un `dato` y luego itera para enviárselo a cada procesador `i` del sistema.

```cpp
if (pid==0)
{
     dato= 4;
     for(i=0;i<p;i++)
     { bsp_send(i, &tag, &dato, sizeof(int)); }
}
bsp_sync();
```

#### **Superpaso 2: Recepción Universal**
Después de la sincronización, *todos* los procesadores (incluido el `0`) reciben el dato y lo imprimen.

```cpp
bsp_qsize( &nm, &nb);
for(i=0; i<nm; i++)
{
   bsp_get_tag(&total,&tag);
   bsp_move(&dato,total);
   printf("pid %d recibio dato %d
",pid,dato);
}
```

### 1.2. Identificación del Problema: Lógica Incorrecta

El código original no cumple con los requisitos de la tarea por dos razones fundamentales:
1.  **Operación Incorrecta:** Realiza un *broadcast* (uno a todos), no una comunicación *punto a punto* (uno a uno).
2.  **Tipo de Dato Incorrecto:** Envía un único `int`, no un arreglo de `N` datos.

El resultado sería que todos los procesadores imprimirían haber recibido el número `4`, lo cual no se corresponde con el objetivo de enviar un arreglo de forma selectiva.

## 2. Análisis de la Solución (`ejemplo2_resuelto.cc`)

La solución correcta modifica completamente la lógica para implementar una comunicación punto a punto y manejar un arreglo.

### 2.1. Estrategia de Corrección: Comunicación Selectiva

Se abandona el bucle de envío. El procesador `0` enviará los datos una sola vez, especificando explícitamente al procesador `1` como único destino. Asimismo, la lógica de recepción solo se ejecutará en el procesador `1`.

### 2.2. Flujo de la Solución Corregida

#### **Superpaso 1: Creación y Envío del Arreglo**
El procesador `0` inicializa un arreglo de `N` elementos y lo envía directamente al procesador `1`. El `payload` de `bsp_send` ahora no es un puntero a un `int`, sino el puntero al arreglo, y el tamaño es `N * sizeof(int)`.

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
        bsp_send(1, &tag, arreglo, N * sizeof(int));
    }
}
bsp_sync();
```

#### **Superpaso 2: Recepción e Impresión del Arreglo**
Solo el procesador `1` ejecuta la lógica de recepción. Mueve el bloque de memoria recibido a un arreglo local y lo imprime para verificar.

```cpp
// Solo se ejecuta en el procesador 1
if (pid == 1) {
    const int N = 10;
    int arreglo_recibido[N];
    bsp_move(arreglo_recibido, N * sizeof(int));
    
    printf("PID %d: Arreglo recibido:
", pid);
    for (int i = 0; i < N; i++) {
        printf("  arreglo[%d] = %d
", i, arreglo_recibido[i]);
    }
}
```

## 3. Comparativa de Resultados y Salida Real

-   **Salida del Código Original:** Todos los procesadores imprimirían `pid X recibio dato 4`.
-   **Salida del Código Corregido:** Solo el procesador `1` imprime el contenido del arreglo, demostrando que la comunicación fue selectiva y correcta.

### Salida de la Ejecución

La ejecución del código corregido con 2 procesadores (`bsprun -np 2 ...`) produce la siguiente salida, que confirma el comportamiento esperado:

```text
PID 0: Enviando arreglo al PID 1...
PID 1: Arreglo recibido:
  arreglo[0] = 10
  arreglo[1] = 20
  arreglo[2] = 30
  arreglo[3] = 40
  arreglo[4] = 50
  arreglo[5] = 60
  arreglo[6] = 70
  arreglo[7] = 80
  arreglo[8] = 90
  arreglo[9] = 100
```
Esta salida confirma que la solución cumple con el objetivo de la tarea: un envío punto a punto de un arreglo. 

---
[Volver al README Principal](../../README.md)
