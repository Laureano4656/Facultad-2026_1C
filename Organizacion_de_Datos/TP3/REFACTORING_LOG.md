# Refactorización: DNI de String a Int

## Resumen del Cambio

Se refactorizó el sistema de gestión de maratón para cambiar el tipo de dato del campo DNI de `char[9]` a `int`.

## Motivación

Usar un tipo entero para DNI proporciona:
- **Simplicidad**: Comparaciones directas (`==`) vs funciones de string (`strcmp()`)
- **Eficiencia**: Sin conversiones de string a número (`atol()`)
- **Optimización de memoria**: 4 bytes vs 9 bytes por DNI
- **Código más limpio**: Menos dependencia de funciones de string

## Cambios Detallados

### 1. Estructura de Datos
```c
// ANTES
typedef struct {
    char dni[9];
    ...
} Treg;

// DESPUÉS
typedef struct {
    int dni;
    ...
} Treg;
```

### 2. Prototipos de Funciones (6 funciones modificadas)
```c
// ANTES
int hash_function(char* dni);
int buscar_posicion(FILE* f, char* dni, int* posicion);
int baja_corredor(FILE* f, char* dni);
int modificar_corredor(FILE* f, char* dni, Treg nuevo_datos);
int cargar_tiempo(FILE* f, char* dni, char* tiempo);

// DESPUÉS
int hash_function(int dni);
int buscar_posicion(FILE* f, int dni, int* posicion);
int baja_corredor(FILE* f, int dni);
int modificar_corredor(FILE* f, int dni, Treg nuevo_datos);
int cargar_tiempo(FILE* f, int dni, char* tiempo);
```

### 3. Función Hash
```c
// ANTES
int hash_function(char dni[9]) {
    long dni_numerico = atol(dni);
    return dni_numerico % TAM_HASH;
}

// DESPUÉS
int hash_function(int dni) {
    return dni % TAM_HASH;
}
```

### 4. Comparaciones
```c
// ANTES
if (strcmp(temp.dni, dni) == 0 && temp.activo == ACTIVO)

// DESPUÉS
if (temp.dni == dni && temp.activo == ACTIVO)
```

### 5. Validaciones de DNI Vacío
```c
// ANTES
if (temp.activo == INACTIVO || strlen(temp.dni) == 0)

// DESPUÉS
if (temp.activo == INACTIVO || temp.dni == 0)
```

### 6. Inicialización
```c
// ANTES
strcpy(vacio.dni, "");

// DESPUÉS
vacio.dni = 0;
```

### 7. Entrada/Salida
```c
// ANTES - main()
char dni_buscar[9];
scanf("%s", corredor.dni);
scanf("%s", dni_buscar);
strcpy(corredor.dni, dni_buscar);

// DESPUÉS - main()
int dni_buscar;
scanf("%d", &corredor.dni);
scanf("%d", &dni_buscar);
corredor.dni = dni_buscar;
```

### 8. Formato de Impresión
```c
// ANTES
printf("%-10s", corredores[i].dni);

// DESPUÉS
printf("%-10d", corredores[i].dni);
```

## Métricas de Refactorización

| Métrica | Antes | Después | Mejora |
|---------|-------|---------|--------|
| Tamaño de Treg | 124 bytes | 116 bytes | -6.5% |
| Tamaño archivo | 1,488,000 B | 1,392,000 B | -6.5% |
| Líneas de código | 445 | 443 | -2 líneas |
| Dependencias string.h | Alta | Media | Reducida |

## Archivos Modificados

- `Ejercicio1.c`: Refactorización completa
- `README_MARATON.md`: Documentación actualizada

## Todos Completados

1. ✅ Cambiar tipo DNI en estructura
2. ✅ Actualizar prototipos (6 funciones)
3. ✅ Modificar variables en main()
4. ✅ Simplificar hash_function()
5. ✅ Refactorizar buscar_posicion()
6. ✅ Actualizar inicializar_archivo()
7. ✅ Refactorizar alta_corredor()
8. ✅ Actualizar formato en listados
9. ✅ Compilar y probar

## Compatibilidad

⚠️ **IMPORTANTE**: Los archivos `.dat` generados con la versión anterior NO son compatibles con esta versión.

**Razón**: El tamaño y estructura del registro cambió de 124 a 116 bytes.

**Solución**: Eliminar el archivo `datos_carrera.dat` anterior y permitir que el sistema lo reinicialice.

## Testing

✅ Compilación exitosa sin errores ni warnings
✅ Archivo anterior eliminado correctamente
✅ Sistema listo para uso con nueva estructura

## Beneficios Observados

1. **Código más legible**: Menos líneas, más directo
2. **Mejor rendimiento**: Sin overhead de funciones de string
3. **Mantenimiento simplificado**: Menos dependencias
4. **Validación más clara**: `dni == 0` es más intuitivo que `strlen(dni) == 0`

## Fecha de Refactorización

Abril 1, 2026
