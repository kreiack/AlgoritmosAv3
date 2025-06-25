# Proyecto de Cómputo Paralelo y Algoritmos Distribuidos

Este repositorio es un compendio de implementaciones, ejercicios resueltos y material de estudio enfocado en los patrones fundamentales de la programación paralela. Utiliza el modelo **Bulk Synchronous Parallel (BSP)** a través de la biblioteca `bsponmpi` como base para la exploración de algoritmos distribuidos.

---

## 🏛️ Estructura del Repositorio

El proyecto se divide en dos áreas principales, cada una con un propósito distinto:

| Directorio                                   | Propósito                                                                                                                              |
| -------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| 📁 [**`TAREA1/`**](./TAREA1/)                     | Contiene la resolución de una serie de **ejercicios prácticos** de programación paralela, cada uno en su propia carpeta auto-contenida. |
| 🎓 [**`ExtraExplicaciones/`**](./ExtraExplicaciones/) | Funciona como una **base de conocimiento** teórica y práctica, explicando los patrones de comunicación y conceptos clave de BSP.     |

---

## 🚀 Compilación y Ejecución

Todos los programas de este repositorio se basan en la biblioteca `bsponmpi`. Para trabajar con ellos, se deben utilizar las herramientas que esta proporciona.

> **Nota:** La biblioteca `bsponmpi` fue instalada y compilada localmente en la carpeta del mismo nombre, la cual está explícitamente ignorada por Git a través del archivo `.gitignore` para mantener el repositorio limpio de dependencias compiladas.

### Proceso Estándar de Compilación y Ejecución

1.  **Compilar el Código Fuente:**
    Utilice el comando `bspcc` para compilar los archivos `.cc`.

    ```bash
    # Sintaxis general
    bspcc -o <nombre_del_ejecutable> <archivo_fuente.cc>
    ```

2.  **Ejecutar el Programa en Paralelo:**
    Utilice `bsprun` para lanzar el programa en múltiples procesos. El flag `-np` especifica el número de procesos a utilizar.

    ```bash
    # Sintaxis general
    bsprun -np <numero_de_procesos> ./<nombre_del_ejecutable>
    ```

---
**Autor:** Pablo Antonio Jordán González  
*Estudiante Doctorado Ciencias de la Ingeniería Mención Informática, Universidad de Santiago de Chile*  
*Director de I+D HealthPixel Spa.*

```bash
# Para compilar un archivo
bspcxx -o mi_ejecutable mi_codigo.cc

# Para ejecutarlo con 4 procesadores
bsprun -n 4 ./mi_ejecutable
```

