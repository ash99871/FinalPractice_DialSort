# Benchmark Sorting Algorithms – DialSort vs RadixSort

# Integrantes: Maybeth López y Ashly Sofía Robayo Parra

## Link del simulador:

https://comparisonofalgorithms.lovable.app

## Descripción:

Este proyecto implementa y compara el rendimiento de dos algoritmos de ordenamiento:

- DialSort
- RadixSort

El objetivo es analizar su comportamiento bajo diferentes condiciones de entrada, evaluando su eficiencia en términos de tiempo de ejecución, estabilidad y uso de memoria.

---

## Estructura del proyecto

```

├── main.cpp          # Punto de entrada y flujo principal
├── dialSort.h/.cpp  # Implementación del DialSort
├── radixSort.h/.cpp   # Implementación de RadixSort
├── benchmark.h/.cpp    # Implementación de las comparaciones 
└── CMakeLists.txt    # Configuración de compilación
```

---

## Enfoque de Implementación:

El sistema se divide en tres partes principales:

- **Algoritmos de ordenamiento**
  - `dialSort.cpp`: implementación de DialSort
  - `radixSort.cpp`: implementación de RadixSort

- **Benchmark**
  - `benchmark.cpp`: ejecuta pruebas con diferentes tipos de datos

- **Main**
  - `main.cpp`: punto de entrada del programa

Se generaron datasets automáticamente sin necesidad de archivos externos.

---

## Tipos de Datos Evaluados

Se probaron los algoritmos con diferentes distribuciones:

- **Uniforme** → datos aleatorios
- **Skewed** → muchos valores repetidos
- **Ordenado** → datos ya ordenados
- **Inverso** → orden descendente

---

## Métricas Evaluadas:

Para cada algoritmo se midieron:

- Tiempo promedio (ms)
- Desviación estándar
- Throughput (datos procesados por segundo)
- Uso de memoria (teórico)

---

## Instrucciones de Ejecución

### Requisitos:
- C++
- CMake
- CLion (opcional)

### Compilar y ejecutar:

```bash
mkdir build
cd build
cmake ..
make
./FinalPractice_DialSort_main
```
---
## Resultados

### Ejemplo de salida:


```text
--- INVERSO ---

ALGORITMO        Promedio(ms)   Desviacion Estandar     Datos por segundo
-------------------------------------------------------------------------
DialSort           17.7                    1.2            56000000
RadixSort          214.6                   10.4           4600000

COMPARACION:
-- Mejor en tiempo: DialSort
Memoria DialSort: O(U) = 1000
Memoria RadixSort: O(n) = 1000000
-- Mejor en memoria: DialSort
```
---
## Análisis y Comparación

### DialSort

- Complejidad: **O(n + U)**
- Excelente rendimiento cuando el rango de valores (U) es pequeño
- Muy rápido en datasets con valores repetidos

### RadixSort

- Complejidad: **O(n · d)**
- Rendimiento estable
- No depende directamente de U

---
## Uso de Memoria

- **DialSort:** O(U)
- **RadixSort:** O(n)

DialSort es más eficiente en memoria cuando **U < n**.

---
## ¿Cuál algoritmo fue mejor?

**DialSort fue el algoritmo con mejor rendimiento en la mayoría de los casos evaluados**, especialmente cuando:

- El universo de valores (U) es pequeño
- Existen muchos valores repetidos

Sin embargo, RadixSort presenta un comportamiento más estable cuando el rango de valores es grande.


## Versión del Sistema Operativo :

Windows 11

## Versión del Compilador :

MinGW-w64


