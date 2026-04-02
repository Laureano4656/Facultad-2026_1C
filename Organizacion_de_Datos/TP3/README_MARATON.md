# Sistema de Gestión de Maratón

## Descripción
Sistema completo para gestionar una maratón de 42km con capacidad máxima de 12,000 corredores. Implementa un archivo directo con función hash para acceso rápido a los registros.

## Características Implementadas

### ✅ Estructura de Datos
- **DNI**: Número entero (int) - 8 dígitos (clave de búsqueda)
- **Nombre**: hasta 50 caracteres
- **Sexo**: M/F
- **Edad**: número entero
- **Categoría**: texto libre (hasta 50 caracteres)
- **Tiempo**: formato HH:MM:SS
- **Estado**: activo/inactivo (baja lógica)

### ✅ Función Hash
- **Técnica**: Módulo (`DNI % 12000`)
- **Resolución de colisiones**: Linear Probing
- Mapea cada DNI a una posición única en el archivo

### ✅ Operaciones CRUD

1. **Alta de Corredor**: Registra nuevos participantes
   - Valida DNI duplicado
   - Asigna posición automáticamente vía hash
   - Controla cupo máximo (12,000)

2. **Baja de Corredor**: Baja lógica
   - Marca el registro como inactivo
   - No elimina físicamente del archivo
   - Preserva integridad del sistema hash

3. **Modificación**: Actualiza datos existentes
   - Preserva el tiempo de carrera si ya existe
   - Busca por DNI usando hash

4. **Carga de Tiempos**: Registra tiempo final
   - Formato HH:MM:SS
   - Se carga al finalizar la carrera

### ✅ Listados

1. **Listado General**
   - Muestra todos los corredores con tiempo registrado
   - Ordenado por tiempo (menor a mayor)
   - Incluye: DNI, Nombre, Categoría, Tiempo

2. **Listado por Categoría**
   - Filtra corredores de una categoría específica
   - Ordenado por tiempo dentro de la categoría
   - Incluye: DNI, Nombre, Tiempo

## Compilación

```bash
gcc -o Ejercicio1.exe Ejercicio1.c -Wall
```

## Uso

```bash
.\Ejercicio1.exe
```

### Menú Principal

```
========================================
   SISTEMA DE GESTION DE MARATON
========================================
1. Alta de corredor
2. Baja de corredor
3. Modificar corredor
4. Cargar tiempo de carrera
5. Listado general de tiempos
6. Listado de tiempos por categoria
7. Salir
========================================
```

## Detalles Técnicos

### Archivo Directo
- **Nombre**: `datos_carrera.dat`
- **Tamaño**: ~1,392,000 bytes (12,000 registros × 116 bytes)
- **Inicialización**: Automática en la primera ejecución
- **Acceso**: Aleatorio usando `fseek()`

### Función Hash
```c
int hash_function(char* dni) {
    long dni_numerico = atol(dni);
    return dni_numerico % 12000;
}
```

### Resolución de Colisiones
Cuando dos DNIs mapean a la misma posición:
1. Se prueba la siguiente posición: `(posicion + 1) % 12000`
2. Se continúa hasta encontrar espacio libre
3. Envuelve al inicio si llega al final del archivo

### Ordenamiento
- Usa `qsort()` de la biblioteca estándar
- Carga registros en memoria para ordenar
- Compara strings de tiempo lexicográficamente

## Ejemplo de Uso

### 1. Registrar Corredor
```
Opción: 1
DNI: 12345678
Nombre: Juan Perez
Sexo: M
Edad: 25
Categoría: Senior
```

### 2. Cargar Tiempo
```
Opción: 4
DNI: 12345678
Tiempo: 03:15:30
```

### 3. Ver Listado General
```
Opción: 5

DNI        Nombre                         Categoria  Tiempo    
-----------------------------------------------------------------------
23456789   Maria Lopez                    Veterano   02:58:45  
12345678   Juan Perez                     Senior     03:15:30  
34567890   Carlos Gomez                   Master     03:45:20  

Total de corredores: 3
```

## Refactorización DNI a Int

### Cambios Realizados (v2.0)
El sistema fue refactorizado para usar `int` en lugar de `char[9]` para el campo DNI:

**Ventajas:**
- ✅ **Código más simple**: Comparaciones con `==` en lugar de `strcmp()`
- ✅ **Más eficiente**: Elimina conversiones string-número (`atol()`)
- ✅ **Menor uso de memoria**: 4 bytes vs 9 bytes por DNI
- ✅ **Hash más directo**: `dni % 12000` sin conversiones

**Cambios en el código:**
1. Estructura: `char dni[9]` → `int dni`
2. Prototipos: `char* dni` → `int dni` en 6 funciones
3. Entrada: `scanf("%s", ...)` → `scanf("%d", ...)`
4. Salida: `printf("%s", ...)` → `printf("%d", ...)`
5. Comparaciones: `strcmp()` → `==`
6. Validaciones: `strlen() == 0` → `dni == 0`
7. Hash: Sin `atol()`, uso directo del entero

**Impacto:**
- Tamaño de registro: 124 bytes → 116 bytes (reducción del 6.5%)
- Tamaño de archivo: 1,488,000 → 1,392,000 bytes
- ⚠️ **Incompatible** con archivos de la versión anterior

## Validaciones Implementadas

- ✅ No permite DNI duplicados
- ✅ Controla cupo máximo (12,000 corredores)
- ✅ Verifica que el corredor exista antes de modificar/eliminar
- ✅ Maneja colisiones de hash correctamente
- ✅ Baja lógica preserva integridad del archivo

## Estructura del Código

- **Líneas 11-19**: Definición de estructura `Treg`
- **Líneas 21-30**: Prototipos de funciones
- **Líneas 32-146**: Función `main()` con menú interactivo
- **Líneas 148-158**: Función `mostrar_menu()`
- **Líneas 160-164**: Función `hash_function()`
- **Líneas 167-199**: Función `buscar_posicion()` con linear probing
- **Líneas 201-220**: Función `inicializar_archivo()`
- **Líneas 222-260**: Función `alta_corredor()`
- **Líneas 262-276**: Función `baja_corredor()`
- **Líneas 278-298**: Función `modificar_corredor()`
- **Líneas 300-314**: Función `cargar_tiempo()`
- **Líneas 316-325**: Función auxiliar `comparar_tiempos()`
- **Líneas 327-368**: Función `listar_tiempos_general()`
- **Líneas 370-417**: Función `listar_tiempos_categoria()`

## Estado del Proyecto

✅ **Versión 2.0 - DNI como Int**
- ✅ Refactorización completa: 9/9 todos
- ✅ Compilación sin errores ni warnings
- ✅ Código optimizado y simplificado
- ✅ Menor uso de memoria (116 bytes/registro)

### Historial de Versiones
**v2.2** - Manejo de rango en funciones (enfoque alternativo)
- Hash simple: `dni % 12007` (sin doble módulo)
- Ajuste de rango en buscar_posicion() y alta_corredor()
- Mapeo explícito: 12000→0, 12001→1, ..., 12006→6

**v2.1** - Optimización de hash con número primo
- HASH_PRIMO = 12007 para mejor distribución
- Doble módulo: (dni % HASH_PRIMO) % TAM_HASH
- Reduce colisiones matemáticamente

**v2.0** - Refactorización DNI a int (9 todos completados)
- Cambio de tipo de dato DNI
- Simplificación de comparaciones
- Optimización de memoria

**v1.0** - Implementación inicial (12 todos completados)
- Sistema completo con todas las funcionalidades
- DNI como string char[9]

## Autor
Implementación completa del sistema de gestión de maratón según especificaciones.
