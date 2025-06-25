# 📦 Tarea: Enviar Objetos mediante Serialización

> Análisis del desafío de enviar datos complejos (objetos/structs) en un entorno de memoria distribuida, demostrando cómo la **serialización manual** es la solución clave para este problema.

---

## 1. Problema: Envío Directo de Punteros

El objetivo es enviar un conjunto de objetos `Dato` desde el proceso `0` al resto. El código original (`ejemplo3.cc.txt`) intenta enviar una `std::list` de punteros a objetos `Dato` directamente.

```cpp
class Dato {
 public:
  int edad;
  char *nombre; // Puntero a memoria dinámica
  //...
};

list<Dato*> listaPersonas;
// ...
bsp_send(i, &tag, &listaPersonas, ...);
```

Esto falla catastróficamente por una razón fundamental:
-   **Envío de Punteros Inválidos:** Lo que se envía por la red no son los datos (nombres, edades), sino los **punteros** (direcciones de memoria). Estas direcciones solo tienen sentido en el espacio de memoria del proceso `0`. Para cualquier otro proceso, son direcciones inválidas, y usarlas resulta en un **fallo de segmentación**.

> **Concepto Clave:** En computación distribuida, cada proceso tiene su propio espacio de memoria. Para transmitir datos complejos, estos deben ser empaquetados en un formato plano y contiguo (un buffer de bytes). Este proceso se llama **serialización**.

## 2. Lógica de la Solución (`ejemplo3_resuelto.cc`)

La solución correcta evita por completo el envío de punteros.

1.  **Estructura de Datos Plana (POD):** Se redefine `Dato` para que no contenga punteros, usando un arreglo de `char` de tamaño fijo para las cadenas. Esto asegura que cada objeto `Dato` sea un bloque de memoria contiguo.
2.  **Serialización en un Buffer:** El procesador `0` crea un arreglo de estos objetos `Dato`. Como el arreglo es contiguo en memoria, puede ser enviado directamente como un único buffer.
3.  **Deserialización:** Los procesos receptores simplemente mueven el buffer recibido a un arreglo local del mismo tipo. El sistema se encarga de la copia de bytes, y los objetos se reconstruyen correctamente.

### Superpaso 1: Creación, Serialización y Broadcast

El procesador `0` crea los datos en un arreglo contiguo (`buffer_envio`) y lo difunde a todos.

```cpp
struct DatoSerializado {
    int edad;
    char nombre[MAX_NAME_LEN];
    char apellido[MAX_NAME_LEN];
};

// ...
if (pid == 0) {
    DatoSerializado buffer_envio[NUM_PERSONAS];
    buffer_envio[0] = {25, "Juan", "Perez"};
    // ...
    for (int i = 0; i < nprocs; i++) {
        bsp_send(i, &tag, buffer_envio, sizeof(buffer_envio));
    }
}
bsp_sync();
```

### Superpaso 2: Deserialización y Cómputo
Todos los procesadores reciben el `buffer`, lo mueven a una estructura local idéntica y realizan el cálculo.

```cpp
DatoSerializado buffer_recibido[NUM_PERSONAS];
bsp_move(buffer_recibido, sizeof(buffer_recibido));

// ... calcular edad máxima y mínima a partir de buffer_recibido ...
printf("PID %d: Edad máxima = %d, Edad mínima = %d\n", pid, edad_max, edad_min);
```

## 3. Resultado de la Ejecución

La salida confirma que todos los procesadores recibieron y procesaron los datos correctamente, demostrando el éxito de la serialización.

```text
PID 0: Enviando 3 objetos a todos los procesadores...
PID 0: Edad máxima = 32, Edad mínima = 18
PID 1: Edad máxima = 32, Edad mínima = 18
PID 2: Edad máxima = 32, Edad mínima = 18
PID 3: Edad máxima = 32, Edad mínima = 18
```

---
[Volver al Índice de Tareas](../README.md) | [Volver al README Principal](../../README.md)

> *Documentación mejorada con la asistencia de IA.*
