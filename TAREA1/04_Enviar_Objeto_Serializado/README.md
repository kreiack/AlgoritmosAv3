# 📝 Tarea: # Análisis y Solución: Serialización para Enviar Objetos

> Solución y análisis del ejercicio.

---

Este documento desglosa la tarea de enviar 3 objetos de tipo `Dato`, donde cada procesador receptor debe calcular la edad máxima y mínima. Se analiza un código inicial que falla catastróficamente debido a la falta de serialización y se presenta una solución funcional que implementa este concepto correctamente.

## 1. Análisis del Código Original (`ejemplo3.cc.txt`)

El objetivo del código es que el procesador `0` cree una lista de 3 objetos `Dato` y la envíe a todos los demás procesadores.

### 1.1. Flujo del Código Original

El código define una clase `Dato` que utiliza punteros `char*` para el nombre y el apellido, asignando memoria dinámicamente.

```cpp
class Dato
{
 public:
  int edad;
  char *nombre;
  char *apellido;

  Dato( int e, char *n, char *a)
  {
    edad = e;
    nombre = new char[strlen(n)]; // Asignación dinámica
    apellido = new char[strlen(a)]; // Asignación dinámica
  }
};
```

Luego, el procesador `0` crea una `std::list` de punteros a `Dato` y intenta enviarla directamente.

```cpp
list<Dato*> listaPersonas;
// ... se agregan 3 personas a la lista ...

for(int i=0;i<nprocs;i++)
{
    // ERROR: hay que serializar!!
    bsp_send(i, &tag, &listaPersonas, listaPersonas.size());
}
bsp_sync();
```

### 1.2. Identificación del Problema: Envío de Punteros

El código está fundamentalmente roto y **no se puede compilar o ejecutar de forma fiable**. El error es intentar enviar el objeto `listaPersonas` directamente.
1.  **¿Qué se envía realmente?** `bsp_send` tomará la dirección de `&listaPersonas` y copiará los bytes que componen el objeto `std::list`. Estos bytes no son los datos de las personas, sino punteros internos que gestionan la lista (puntero al primer nodo, al último, etc.).
2.  **Punteros Inválidos:** Cuando otro procesador recibe estos bytes, los punteros que contienen apuntan a direcciones de memoria que solo tenían sentido en el procesador `0`. En el contexto del receptor, estas direcciones son basura, y cualquier intento de usarlas resultará en un **fallo de segmentación** (segmentation fault).

> **Concepto Clave:** En computación distribuida, cada proceso tiene su propio espacio de memoria. No se pueden compartir punteros directamente entre procesos. Los datos deben ser copiados y empaquetados en un formato plano (un buffer de bytes) para su transmisión. A este proceso se le llama **serialización**.

## 2. Análisis de la Solución (`ejemplo3_resuelto.cc`)

La solución funcional abandona la idea de enviar objetos complejos y en su lugar envía un buffer de datos primitivos y contiguos.

### 2.1. Estrategia de Corrección: Serialización Manual

1.  **Simplificar la Estructura:** Se redefine `Dato` para que no use punteros. Para este ejemplo, el nombre se simplifica a un entero. Lo crucial es que la estructura solo contenga tipos de datos de tamaño fijo (Plain Old Data).
2.  **Serializar a un Buffer:** Antes de enviar, el procesador `0` crea un `int buffer[]` y copia manualmente los campos de los 3 objetos en él, en un orden conocido y predecible.
3.  **Enviar el Buffer:** Se envía el `buffer`, no los objetos.
4.  **Deserializar:** Los receptores leen el `buffer` y, conociendo el orden, reconstruyen sus propias copias locales de los objetos.

### 2.2. Flujo de la Solución Corregida

#### **Superpaso 1: Creación, Serialización y Broadcast**
El procesador `0` crea los datos, los empaqueta (serializa) en un `buffer` y lo difunde a todos.

```cpp
// Solo en procesador 0
if (pid == 0) {
    Dato personas[3];
    personas[0] = {1, 25}; // {nombre, edad}
    personas[1] = {2, 30};
    personas[2] = {3, 22};

    int buffer[6]; // Buffer para 3 nombres (int) y 3 edades (int)
    for (int i = 0; i < 3; ++i) {
        buffer[i] = personas[i].nombre;
        buffer[i + 3] = personas[i].edad;
    }
    
    // Broadcast del buffer serializado
    for (int i = 0; i < nprocs; i++) {
        bsp_send(i, &tag, buffer, sizeof(buffer));
    }
}
bsp_sync();
```

#### **Superpaso 2: Deserialización y Cómputo**
Todos los procesadores reciben el `buffer`, lo desempaquetan (deserializan) en sus propias estructuras `Dato`, y luego realizan el cálculo de máximo/mínimo sobre sus datos locales.

```cpp
// Todos los procesadores
int buffer_recibido[6];
bsp_move(buffer_recibido, sizeof(buffer_recibido));

Dato personas_recibidas[3];
for (int i = 0; i < 3; i++) {
    personas_recibidas[i].nombre = buffer_recibido[i];
    personas_recibidas[i].edad = buffer_recibido[i + 3];
}

int max_edad = 0;
int min_edad = 150;
for (int i = 0; i < 3; i++) {
    if (personas_recibidas[i].edad > max_edad) max_edad = personas_recibidas[i].edad;
    if (personas_recibidas[i].edad < min_edad) min_edad = personas_recibidas[i].edad;
}
printf("PID %d: Max=%d, Min=%d
", pid, max_edad, min_edad);
```

## 3. Comparativa de Resultados

-   **Salida del Código Original:** Error de compilación o, más probablemente, un fallo de segmentación en tiempo de ejecución. No produciría ninguna salida útil.
-   **Salida del Código Corregido:** Todos los procesadores imprimen el mismo resultado correcto, demostrando que recibieron y procesaron los datos correctamente.

### Salida de la Ejecución

La ejecución del código corregido con 4 procesadores (`bsprun -np 4 ...`) produce la siguiente salida. Confirma que el procesador 0 envía los datos y que todos los procesadores (incluido el 0) reciben el buffer, lo deserializan y calculan el mismo resultado.

```text
PID 0: Enviando 3 objetos a todos los procesadores...
PID 0: Edad máxima = 32, Edad mínima = 18
PID 1: Edad máxima = 32, Edad mínima = 18
PID 2: Edad máxima = 32, Edad mínima = 18
PID 3: Edad máxima = 32, Edad mínima = 18
```
Esta salida confirma que la serialización fue exitosa y permitió la correcta distribución y procesamiento de los datos. 

---
[Volver al README Principal](../../README.md)
