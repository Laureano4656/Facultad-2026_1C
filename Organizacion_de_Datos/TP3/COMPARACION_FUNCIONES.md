# Comparación: buscar_posicion vs buscapos

## Resumen Ejecutivo

Ambas funciones buscan una posición en el archivo usando hash, pero tienen **diferencias significativas** en:
- Enfoque de búsqueda
- Manejo de colisiones
- Manejo de rango
- Corrección del código

---

## Tabla Comparativa

| Aspecto | `buscar_posicion` (actual) | `buscapos` (otra versión) |
|---------|---------------------------|---------------------------|
| **Firma** | `int buscar_posicion(FILE* f, int dni, int* posicion)` | `int buscapos(unsigned long int dni, FILE* arch)` |
| **Tipo DNI** | `int` | `unsigned long int` |
| **Retorna posición** | Sí, por referencia (`*posicion`) | Sí, por valor de retorno |
| **Retorna estado** | 0 = encontrado, -1 = no encontrado | Solo posición (o -1 si lleno) |
| **Hash** | Llama `hash_function(dni)` | Llama `hash(dni)` |
| **Manejo rango** | `if (pos >= 12000) pos %= 12000` | `if (pos > 12000 && pos <= 12006) pos = 0` |
| **Colisiones** | **Linear Probing circular** | **Búsqueda lineal adelante/atrás** |
| **Estructura** | `Treg temp` | `reg registro` |
| **Validación activo** | `temp.activo == ACTIVO` | `reg.activo` |

---

## Análisis Detallado

### 1. Parámetros y Retorno

**buscar_posicion:**
```c
int buscar_posicion(FILE* f, int dni, int* posicion)
// Retorna: 0 si encontrado, -1 si no
// Guarda posición en *posicion (sea encontrado o lugar libre)
```
✅ **Ventaja:** Retorna dos valores (estado + posición)
✅ **Útil para:** Saber si existe Y dónde insertar

**buscapos:**
```c
int buscapos(unsigned long int dni, FILE* arch)
// Retorna: posición encontrada o -1 si archivo lleno
```
❌ **Limitación:** No distingue entre "encontrado" y "posición libre"

---

### 2. Manejo de Rango Hash (12000-12006)

**buscar_posicion:**
```c
if (pos_inicial >= MAX_CORREDORES) {
    pos_inicial = pos_inicial % MAX_CORREDORES;
}
```
✅ **Correcto:** Mapea 12000→0, 12001→1, ..., 12006→6
✅ **Distribución:** Mantiene la distribución del hash

**buscapos:**
```c
if (pos > 12000 && pos <= 12006) {
    pos = 0;  // reinicia y busca lineal
}
```
❌ **Problema:** Todos los valores 12001-12006 van a posición 0
❌ **Consecuencia:** Pierde la ventaja de distribución del hash
❌ **Colisiones:** Genera artificialmente colisiones en posición 0

---

### 3. Resolución de Colisiones

**buscar_posicion (Linear Probing):**
```c
do {
    fseek(f, pos_actual * sizeof(Treg), SEEK_SET);
    fread(&temp, sizeof(Treg), 1, f);
    
    if (temp.dni == dni && temp.activo == ACTIVO) {
        *posicion = pos_actual;
        return 0;  // Encontrado
    }
    
    if (temp.activo == INACTIVO || temp.dni == 0) {
        *posicion = pos_actual;
        return -1;  // Posición libre
    }
    
    pos_actual = (pos_actual + 1) % MAX_CORREDORES;
    intentos++;
    
} while (pos_actual != pos_inicial && intentos < MAX_CORREDORES);
```

✅ **Ventajas:**
- Búsqueda sistemática circular
- Detecta DNI existente
- Encuentra posición libre o inactiva
- Termina cuando da la vuelta completa
- Usa `MAX_CORREDORES` para límites correctos

**buscapos (Búsqueda lineal adelante/atrás):**
```c
if (reg.activo) {
    // Buscar adelante
    while (fread(...) != 1 && reg.activo)
        i++;
    
    if (reg.activo) {
        // Buscar desde el inicio hasta pos
        fseek(arch, 0, SEEK_SET);
        while (fread(...) != 1 && reg.activo && i < pos)
            i++;
    }
}
```

❌ **Problemas críticos:**
1. **Condición incorrecta:** `fread(...) != 1` debería ser `== 1`
2. **No verifica DNI:** Solo busca espacios vacíos, no compara el DNI
3. **No circular:** Busca adelante, luego desde 0 hasta pos (ineficiente)
4. **Variable no usada:** `i` se incrementa pero no se usa para nada
5. **No busca existente:** No puede determinar si el DNI ya existe

---

### 4. Errores en `buscapos`

#### Error 1: Condición de fread
```c
while (fread(&registro, sizeof(reg), 1, arch) != 1 && reg.activo)
```
❌ **Problema:** `fread != 1` significa error o EOF
✅ **Debería ser:** `fread(...) == 1 && reg.activo`

#### Error 2: Uso de `reg` en lugar de `registro`
```c
fread(&registro, sizeof(reg), 1, arch);
if (reg.activo) {  // ❌ reg vs registro
```
❌ **Inconsistencia:** Define `registro` pero usa `reg`

#### Error 3: No verifica el DNI
```c
// Solo verifica si está activo, no si es el DNI correcto
if (reg.activo) {
    // busca siguiente activo
}
```
❌ **Problema:** No compara `registro.dni == dni`

#### Error 4: fseek mal usado
```c
fseek(arch, pos, SEEK_SET);
```
❌ **Error:** `pos` es índice, debería ser `pos * sizeof(reg)`
✅ **Correcto:** `fseek(arch, pos * sizeof(reg), SEEK_SET)`

#### Error 5: No retorna valor
```c
int buscapos(...) {
    // ... código ...
}  // ❌ No hay return al final
```
❌ **Warning:** Función debería retornar int pero no tiene return

---

## 5. Casos de Uso

### `buscar_posicion` - Uso correcto:

```c
int pos;
if (buscar_posicion(archivo, 12345678, &pos) == 0) {
    printf("DNI encontrado en posición %d\n", pos);
} else {
    printf("DNI no existe, se puede insertar en %d\n", pos);
}
```

### `buscapos` - Intento de uso (con errores):

```c
int pos = buscapos(12345678, archivo);
if (pos != -1) {
    printf("Posición: %d\n", pos);  // ¿Encontrado o libre? No se sabe
} else {
    printf("Archivo lleno\n");
}
```

---

## 6. Comparación de Eficiencia

| Operación | `buscar_posicion` | `buscapos` |
|-----------|-------------------|------------|
| **Mejor caso** | O(1) - encuentra en hash directo | O(1) - si espacio libre en hash |
| **Caso promedio** | O(k) - k colisiones consecutivas | O(n) - búsqueda lineal completa |
| **Peor caso** | O(n) - archivo lleno | O(n) - archivo lleno |
| **Búsqueda circular** | ✅ Sí | ❌ No, solo adelante o 0→pos |

---

## Conclusiones

### `buscar_posicion` ✅
- **Correcto:** Código funcionalmente correcto
- **Completo:** Busca existente y encuentra posición libre
- **Eficiente:** Linear probing circular óptimo
- **Mantenible:** Código claro y bien documentado
- **Robusto:** Maneja correctamente todos los casos

### `buscapos` ❌
- **Errores:** Múltiples bugs que impedirían compilación/ejecución
- **Incompleto:** No verifica si DNI existe
- **Ineficiente:** Búsqueda lineal no circular
- **Confuso:** Mezcla de nombres de variables
- **No funcional:** No retorna valor en todos los casos

---

## Recomendación

**Usar `buscar_posicion`** exclusivamente. La función `buscapos` tiene demasiados errores y debe ser eliminada o completamente reescrita.

### Si se desea mantener `buscapos`, se requiere:

1. ✅ Corregir `fread(...) != 1` → `== 1`
2. ✅ Unificar `reg` y `registro`
3. ✅ Agregar `fseek(arch, pos * sizeof(reg), SEEK_SET)`
4. ✅ Comparar `registro.dni == dni`
5. ✅ Agregar `return pos;` al final
6. ✅ Implementar búsqueda circular correcta
7. ✅ Distinguir entre "encontrado" y "posición libre"

**Esfuerzo estimado:** Reescribir completa la función (más fácil que arreglar)
