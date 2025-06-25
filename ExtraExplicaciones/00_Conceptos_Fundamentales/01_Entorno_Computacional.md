# 01 - Inspección del Entorno Computacional

Antes de escribir código paralelo, es fundamental comprender el hardware sobre el que se ejecutará. Conocer el número de procesadores y núcleos disponibles nos permite tomar decisiones informadas sobre cuántos procesos paralelos lanzar para obtener el máximo rendimiento.

## Comandos para macOS

En sistemas basados en Unix como macOS, el comando `sysctl` nos permite inspeccionar los parámetros del kernel y del hardware.

### 1. Modelo exacto de la CPU

Para saber con qué procesador estamos trabajando, podemos usar:

```bash
sysctl -n machdep.cpu.brand_string
```

### 2. Número de Núcleos Lógicos (Hilos de Ejecución)

Este es el número más relevante para `bsprun -np P`. Indica el máximo número de hilos que el sistema operativo puede ejecutar simultáneamente. Generalmente, no es recomendable usar un `P` mayor a este número.

```bash
sysctl -n hw.logicalcpu
```

### 3. Número de Núcleos Físicos

Indica cuántos núcleos de procesamiento reales tiene el chip, sin contar tecnologías como Hyper-Threading que los duplican a nivel lógico.

```bash
sysctl -n hw.physicalcpu
```

## Salida Real en su Sistema

A continuación, se muestra la salida de estos comandos ejecutados en su equipo para que sirva como referencia concreta.

```text
Modelo de CPU:
Apple M4 Max

Núcleos Lógicos:
16

Núcleos Físicos:
16
```

Estos datos nos indican que su computador cuenta con un procesador Apple M4 Max con 16 núcleos de alto rendimiento, lo que lo hace un sistema excepcionalmente potente para la experimentación en cómputo paralelo. Podríamos, por ejemplo, ejecutar hasta `bsprun -np 16` y cada proceso tendría un núcleo lógico a su disposición. 