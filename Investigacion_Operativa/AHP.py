import numpy as np

# =========================
# CONFIGURACIÓN
# =========================

# Cantidad de criterios y alternativas (MODIFICABLE)
num_criterios = 3
num_alternativas = 3

# =========================
# MATRICES DE ENTRADA
# =========================

# Matriz de comparación de criterios (nxn)
matriz_criterios = np.array([
    [1,   3,   4],
    [1/3, 1,   3],
    [1/4,   1/3,   1]
    
])

# Lista de matrices de alternativas (una por criterio)
matrices_alternativas = [
    np.array([
        [1,   4,   2],
        [1/2, 1,   1/3],
        [1/2, 3, 1]
    ]),
    
    np.array([
        [1,   1/2, 1/4],
        [2,   1,   1/3],
        [4,   3, 1]
    ]),
    np.array([
        [1,   4, 2],
        [1/4,   1,   1],
        [1/2,   1,   1]
    ])
]

# =========================
# FUNCIONES AHP
# =========================

def normalizar_matriz(matriz):
    suma_columnas = np.sum(matriz, axis=0)
    return matriz / suma_columnas

def calcular_pesos(matriz_normalizada):
    return np.mean(matriz_normalizada, axis=1)

def procesar_matriz(nombre, matriz):
    print(f"\n=== {nombre} ===")
    
    matriz_norm = normalizar_matriz(matriz)
    pesos = calcular_pesos(matriz_norm)

    print("\nMatriz normalizada:")
    print(np.round(matriz_norm, 4))

    print("\nPesos (Wi):")
    print(np.round(pesos, 4))

    return pesos, matriz_norm


# =========================
# 1. PROCESAR CRITERIOS
# =========================

pesos_criterios, matriz_norm_criterios = procesar_matriz(
    "Matriz de Criterios", matriz_criterios
)

# =========================
# 2. PROCESAR ALTERNATIVAS
# =========================

pesos_alternativas = []

for i in range(num_criterios):
    pesos, _ = procesar_matriz(
        f"Matriz Alternativas - Criterio {i+1}",
        matrices_alternativas[i]
    )
    pesos_alternativas.append(pesos)

# Convertir a matriz de prioridades P
matriz_prioridades = np.array(pesos_alternativas).T

print("\n==============================")
print("MATRIZ DE PRIORIDADES (P)")
print("==============================")
print(np.round(matriz_prioridades, 4))

# =========================
# 3. PRIORIDADES GLOBALES
# =========================

prioridades_globales = matriz_prioridades @ pesos_criterios

print("\n==============================")
print("PRIORIDADES GLOBALES")
print("==============================")

for i, val in enumerate(prioridades_globales):
    print(f"Alternativa {i+1}: {round(val, 4)}")

print("\nSuma (control):", round(np.sum(prioridades_globales), 4))