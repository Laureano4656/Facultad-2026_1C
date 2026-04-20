#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define MAX_CORREDORES 12000
#define TAM_HASH 12007
#define ACTIVO 1
#define INACTIVO 0

// ==============================
// Estructuras de datos
// ==============================

typedef struct {
    int dni;
    char nombre[MAX];
    char sexo;
    unsigned short int edad;
    char categoria[MAX];
    char tiempo[9];
    unsigned short int activo;
} Treg;

int tabla_hash[TAM_HASH];

// ==============================
// Prototipos
// ==============================
// Prototipos de funciones
int hash_function(int dni);
int resolver_colision_sondeo_lineal(int pos_inicial, int dni, int* encontrado);
int resolver_posicion_hash(int dni, int crear);
void eliminar_dni_de_hash(int dni);
void inicializar_tabla_hash(void);
void reconstruir_tabla_hash(FILE* f);
void inicializar_archivo(FILE* f);
int alta_corredor(FILE* f, Treg corredor);
int baja_corredor(FILE* f, int dni);
int modificar_corredor(FILE* f, int dni, Treg nuevo_datos);
int cargar_tiempo(FILE* f, int dni, const char* tiempo);
void listar_tiempos_general(FILE* f);
void listar_tiempos_categoria(FILE* f, const char* categoria);
void mostrar_tabla_hash(void);
void mostrar_menu();

// ==============================
// Programa principal
// ==============================

int main() {
    FILE* archDatos;
    int opcion;
    Treg corredor;
    int dni_buscar;
    char tiempo_temp[9];
    char categoria_buscar[MAX];

    // Abre archivo existente; si no existe, lo crea e inicializa.
    archDatos = fopen("datos_carrera.dat", "rb+");
    if (archDatos == NULL) {
        // Si no existe, crear y inicializar
        archDatos = fopen("datos_carrera.dat", "wb+");
        if (archDatos == NULL) {
            printf("Error al crear el archivo\n");
            return 1;
        }
        printf("Inicializando archivo de datos...\n");
        inicializar_archivo(archDatos);
        printf("Archivo inicializado correctamente\n");
        fclose(archDatos);
        archDatos = fopen("datos_carrera.dat", "rb+");
    }

    inicializar_tabla_hash();
    reconstruir_tabla_hash(archDatos);

    // Construye el indice hash en memoria a partir del archivo.
    do {
        mostrar_menu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:  // Alta
                printf("\n=== ALTA DE CORREDOR ===\n");
                printf("DNI (8 digitos): ");
                scanf("%d", &corredor.dni);
                getchar();
                printf("Nombre: ");
                fgets(corredor.nombre, MAX, stdin);
                corredor.nombre[strcspn(corredor.nombre, "\n")] = 0;
                printf("Sexo (M/F): ");
                scanf("%c", &corredor.sexo);
                printf("Edad: ");
                scanf("%hu", &corredor.edad);
                fflush(stdin);
                printf("Categoria: ");
                scanf(" %[^\n]", corredor.categoria);
                corredor.categoria[strcspn(corredor.categoria, "\n")] = 0;
                strcpy(corredor.tiempo, "");
                corredor.activo = ACTIVO;

                if (alta_corredor(archDatos, corredor) == 0) {
                    printf("Corredor registrado exitosamente\n");
                } else {
                    printf("Error al registrar corredor\n");
                }
                break;

            case 2:  // Baja
                printf("\n=== BAJA DE CORREDOR ===\n");
                printf("DNI del corredor a dar de baja: ");
                scanf("%d", &dni_buscar);

                if (baja_corredor(archDatos, dni_buscar) == 0) {
                    printf("Corredor dado de baja exitosamente\n");
                } else {
                    printf("Error: Corredor no encontrado\n");
                }
                break;

            case 3:  // Modificación
                printf("\n=== MODIFICACION DE CORREDOR ===\n");
                printf("DNI del corredor a modificar: ");
                scanf("%d", &dni_buscar);
                getchar();

                printf("Nuevos datos:\n");
                corredor.dni = dni_buscar;
                printf("Nombre: ");
                fgets(corredor.nombre, MAX, stdin);
                corredor.nombre[strcspn(corredor.nombre, "\n")] = 0;
                printf("Sexo (M/F): ");
                scanf("%c", &corredor.sexo);
                printf("Edad: ");
                scanf("%hu", &corredor.edad);
                getchar();
                printf("Categoria: ");
                fgets(corredor.categoria, MAX, stdin);
                corredor.categoria[strcspn(corredor.categoria, "\n")] = 0;
                corredor.activo = ACTIVO;

                if (modificar_corredor(archDatos, dni_buscar, corredor) == 0) {
                    printf("Corredor modificado exitosamente\n");
                } else {
                    printf("Error: Corredor no encontrado\n");
                }
                break;

            case 4:  // Cargar tiempo
                printf("\n=== CARGA DE TIEMPO ===\n");
                printf("DNI del corredor: ");
                scanf("%d", &dni_buscar);
                printf("Tiempo (HH:MM:SS): ");
                scanf("%s", tiempo_temp);

                if (cargar_tiempo(archDatos, dni_buscar, tiempo_temp) == 0) {
                    printf("Tiempo cargado exitosamente\n");
                } else {
                    printf("Error: Corredor no encontrado\n");
                }
                break;

            case 5:  // Listado general
                printf("\n=== LISTADO GENERAL DE TIEMPOS ===\n");
                listar_tiempos_general(archDatos);
                break;

            case 6:  // Listado por categoría
                printf("\n=== LISTADO POR CATEGORIA ===\n");
                printf("Categoria a buscar: ");
                getchar();
                fgets(categoria_buscar, MAX, stdin);
                categoria_buscar[strcspn(categoria_buscar, "\n")] = 0;
                listar_tiempos_categoria(archDatos, categoria_buscar);
                break;

            case 7:
                printf("Saliendo del sistema...\n");
                break;

            case 8:  // Ver tabla hash
                printf("\n=== TABLA HASH (DNI) ===\n");
                mostrar_tabla_hash();
                break;

            default:
                printf("Opcion invalida\n");
        }

        printf("\nPresione Enter para continuar...");
        getchar();

    } while (opcion != 7);

    fclose(archDatos);
    return 0;
}

void mostrar_menu() {
    printf("\n");
    printf("========================================\n");
    printf("   SISTEMA DE GESTION DE MARATON\n");
    printf("========================================\n");
    printf("1. Alta de corredor\n");
    printf("2. Baja de corredor\n");
    printf("3. Modificar corredor\n");
    printf("4. Cargar tiempo de carrera\n");
    printf("5. Listado general de tiempos\n");
    printf("6. Listado de tiempos por categoria\n");
    printf("7. Salir\n");
    printf("8. Mostrar tabla hash\n");
    printf("========================================\n");
    printf("Seleccione una opcion: ");
}

void mostrar_tabla_hash(void) {
    int i;
    int ocupadas = 0;

    printf("%-10s %-12s\n", "Indice", "DNI");
    printf("------------------------\n");

    for (i = 0; i < TAM_HASH; i++) {
        if (tabla_hash[i] != 0) {
            printf("%-10d %-12d\n", i, tabla_hash[i]);
            ocupadas++;
        }
    }

    printf("------------------------\n");
    printf("Celdas ocupadas: %d de %d\n", ocupadas, TAM_HASH);
}

// ==============================
// Funciones de hash
// ==============================

// Funcion hash para DNI.
int hash_function(int dni) {
    return dni % TAM_HASH;
}

int resolver_colision_sondeo_lineal(int pos_inicial, int dni, int* encontrado) {
    int i;

    // Primer tramo: desde la posicion hash hasta el final de la tabla.
    for (i = pos_inicial; i < TAM_HASH; i++) {
        if (tabla_hash[i] == dni) {
            *encontrado = 1;
            return i;
        }

        if (tabla_hash[i] == 0) {
            *encontrado = 0;
            return i;
        }
    }

    // Segundo tramo: desde el inicio hasta la posicion hash - 1.
    for (i = 0; i < pos_inicial; i++) {
        if (tabla_hash[i] == dni) {
            *encontrado = 1;
            return i;
        }

        if (tabla_hash[i] == 0) {
            *encontrado = 0;
            return i;
        }
    }

    *encontrado = 0;
    return -1;
}

// Busca o inserta un DNI en la tabla hash.
// crear=0: solo busca. crear=1: inserta cuando no existe.
// Retorna el indice hash (que coincide con la posicion fisica).
int resolver_posicion_hash(int dni, int crear) {
    int pos_inicial = hash_function(dni);
    int encontrado;
    int idx = resolver_colision_sondeo_lineal(pos_inicial, dni, &encontrado);

    if (idx == -1) {
        return -1;
    }

    if (encontrado) {
        return idx;
    }

    if (!crear) {
        return -1;
    }

    tabla_hash[idx] = dni;
    return idx;
}

void eliminar_dni_de_hash(int dni) {
    int pos_inicial = hash_function(dni);
    int encontrado;
    int idx = resolver_colision_sondeo_lineal(pos_inicial, dni, &encontrado);

    if (idx == -1 || !encontrado) {
        return;
    }

    tabla_hash[idx] = 0;

    // Reinserta la corrida para mantener consistente el sondeo lineal.
    for (;;) {
        int siguiente = idx + 1;
        int dni_reinsertar;

        if (siguiente >= TAM_HASH) {
            siguiente = 0;
        }

        if (tabla_hash[siguiente] == 0) {
            break;
        }

        dni_reinsertar = tabla_hash[siguiente];

        tabla_hash[siguiente] = 0;

        resolver_posicion_hash(dni_reinsertar, 1);
        idx = siguiente;
    }
}

void inicializar_tabla_hash(void) {
    int i;

    for (i = 0; i < TAM_HASH; i++) {
        tabla_hash[i] = 0;
    }
}

// Reconstruye la tabla hash leyendo los registros activos del archivo.
void reconstruir_tabla_hash(FILE* f) {
    int i;
    Treg temp;

    fseek(f, 0, SEEK_SET);
    for (i = 0; i < MAX_CORREDORES; i++) {
        if (fread(&temp, sizeof(Treg), 1, f) != 1) {
            break;
        }

        if (temp.dni != 0 && temp.activo == ACTIVO) {
            resolver_posicion_hash(temp.dni, 1);
        }
    }
}

// ==============================
// Funciones de archivo
// ==============================

// Inicializa el archivo con 12000 registros vacíos
void inicializar_archivo(FILE* f) {
    Treg vacio;
    int i;

    vacio.dni = 0;
    strcpy(vacio.nombre, "");
    vacio.sexo = ' ';
    vacio.edad = 0;
    strcpy(vacio.categoria, "");
    strcpy(vacio.tiempo, "");
    vacio.activo = INACTIVO;

    fseek(f, 0, SEEK_SET);
    for (i = 0; i < MAX_CORREDORES; i++) {
        fwrite(&vacio, sizeof(Treg), 1, f);
    }
    fflush(f);
}

// Alta de corredor: usa hash para obtener posicion y luego escribe en archivo.
int alta_corredor(FILE* f, Treg corredor) {
    int pos_fisica = resolver_posicion_hash(corredor.dni, 0);
    Treg existente;

    if (pos_fisica != -1) {
        fseek(f, pos_fisica * sizeof(Treg), SEEK_SET);
        if (fread(&existente, sizeof(Treg), 1, f) != 1) {
            return -1;
        }

        if (existente.activo == ACTIVO) {
            printf("Error: El DNI ya esta registrado\n");
            return -1;
        }

        fseek(f, pos_fisica * sizeof(Treg), SEEK_SET);
        fwrite(&corredor, sizeof(Treg), 1, f);
        fflush(f);
        printf("Corredor registrado en posicion: %d\n", pos_fisica);
        return 0;
    }

    pos_fisica = resolver_posicion_hash(corredor.dni, 1);
    if (pos_fisica == -1) {
        printf("Error: Tabla hash sin espacio\n");
        return -1;
    }

    fseek(f, pos_fisica * sizeof(Treg), SEEK_SET);
    fwrite(&corredor, sizeof(Treg), 1, f);
    fflush(f);
    printf("Corredor registrado en posicion: %d\n", pos_fisica);
    return 0;
}

// Baja logica: marca inactivo en archivo y elimina DNI del indice hash.
int baja_corredor(FILE* f, int dni) {
    int posicion = resolver_posicion_hash(dni, 0);
    Treg corredor;

    if (posicion != -1) {
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        if (fread(&corredor, sizeof(Treg), 1, f) != 1) {
            return -1;
        }

        if (corredor.dni != dni || corredor.activo != ACTIVO) {
            return -1;
        }

        corredor.activo = INACTIVO;
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fwrite(&corredor, sizeof(Treg), 1, f);
        fflush(f);

        eliminar_dni_de_hash(dni);
        return 0;
    }
    return -1;
}

// Modifica un corredor existente manteniendo su tiempo previo si ya lo tenia.
int modificar_corredor(FILE* f, int dni, Treg nuevo_datos) {
    int posicion = resolver_posicion_hash(dni, 0);

    if (posicion != -1) {
        // Mantener el tiempo anterior si existe
        Treg anterior;
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        if (fread(&anterior, sizeof(Treg), 1, f) != 1) {
            return -1;
        }

        if (anterior.dni != dni || anterior.activo != ACTIVO) {
            return -1;
        }

        if (strlen(anterior.tiempo) > 0) {
            strcpy(nuevo_datos.tiempo, anterior.tiempo);
        }

        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fwrite(&nuevo_datos, sizeof(Treg), 1, f);
        fflush(f);
        return 0;
    }
    return -1;
}

// Carga/actualiza el tiempo de carrera de un corredor existente.
int cargar_tiempo(FILE* f, int dni, const char* tiempo) {
    int posicion = resolver_posicion_hash(dni, 0);
    Treg corredor;

    if (posicion != -1) {
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        if (fread(&corredor, sizeof(Treg), 1, f) != 1) {
            return -1;
        }

        if (corredor.dni != dni || corredor.activo != ACTIVO) {
            return -1;
        }

        strcpy(corredor.tiempo, tiempo);
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fwrite(&corredor, sizeof(Treg), 1, f);
        fflush(f);
        return 0;
    }
    return -1;
}

// ==============================
// Funciones de listados
// ==============================

// Funcion auxiliar para comparar tiempos (para qsort).
int comparar_tiempos(const void* a, const void* b) {
    Treg* corredor_a = (Treg*)a;
    Treg* corredor_b = (Treg*)b;

    // Si alguno no tiene tiempo, va al final
    if (strlen(corredor_a->tiempo) == 0) return 1;
    if (strlen(corredor_b->tiempo) == 0) return -1;

    return strcmp(corredor_a->tiempo, corredor_b->tiempo);
}

// Lista todos los tiempos cargados.
void listar_tiempos_general(FILE* f) {
    int i;
    Treg temp;

    printf("\n%-10s %-30s %-10s %-10s\n", "DNI", "Nombre", "Categoria", "Tiempo");
    printf("-----------------------------------------------------------------------\n");
    // Recorre la tabla hash y accede directo al registro del archivo.
    for (i = 0; i < TAM_HASH; i++) {
        if (tabla_hash[i] == 0) {
            continue;
        }

        fseek(f, i * sizeof(Treg), SEEK_SET);
        if (fread(&temp, sizeof(Treg), 1, f) != 1) {
            continue;
        }

        if (temp.activo == ACTIVO && strlen(temp.tiempo) > 0) {
            printf("%-10d %-30s %-10s %-10s\n",
                   temp.dni,
                   temp.nombre,
                   temp.categoria,
                   temp.tiempo);
        }
    }
}

// Lista tiempos filtrados por categoria.
void listar_tiempos_categoria(FILE* f, const char* categoria) {
    int i;
    Treg temp;

    printf("\nCategoria: %s\n", categoria);
    printf("%-10s %-30s %-10s\n", "DNI", "Nombre", "Tiempo");
    printf("-----------------------------------------------------------------------\n");
    // Recorre la tabla hash y accede directo al registro del archivo.
    for (i = 0; i < TAM_HASH; i++) {
        if (tabla_hash[i] == 0) {
            continue;
        }

        fseek(f, i * sizeof(Treg), SEEK_SET);
        if (fread(&temp, sizeof(Treg), 1, f) != 1) {
            continue;
        }

        if (temp.activo == ACTIVO &&
            strlen(temp.tiempo) > 0 &&
            strcmp(temp.categoria, categoria) == 0) {
            printf("%-10d %-30s %-10s\n",
                   temp.dni,
                   temp.nombre,
                   temp.tiempo);
        }
    }
}