#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define MAX_CORREDORES 12000
#define TAM_HASH 12007
#define ACTIVO 1
#define INACTIVO 0

typedef struct {
    int dni;
    char nombre[MAX];
    char sexo;
    unsigned short int edad;
    char categoria[MAX];
    char tiempo[9];
    unsigned short int activo;
} Treg;

// Prototipos de funciones
int hash_function(int dni);
int buscar_posicion(FILE* f, int dni, int* posicion);
void inicializar_archivo(FILE* f);
int alta_corredor(FILE* f, Treg corredor);
int baja_corredor(FILE* f, int dni);
int modificar_corredor(FILE* f, int dni, Treg nuevo_datos);
int cargar_tiempo(FILE* f, int dni, char* tiempo);
void listar_tiempos_general(FILE* f);
void listar_tiempos_categoria(FILE* f, char* categoria);
void mostrar_menu();

int main() {
    FILE* archDatos;
    int opcion;
    Treg corredor;
    int dni_buscar;
    char tiempo_temp[9];
    char categoria_buscar[MAX];

    // Verificar si el archivo existe
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
                fgets(corredor.categoria, MAX, stdin);
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
    printf("========================================\n");
    printf("Seleccione una opcion: ");
}

// Función hash: usa número primo (12007) para mejor distribución
// Puede retornar valores 0-12006 (se ajustan al rango en las funciones)
int hash_function(int dni) {
    return dni % TAM_HASH;
}

// Busca la posición de un DNI en el archivo usando linear probing
// Retorna 0 si encuentra el DNI, -1 si no existe, guarda la posición en *posicion
int buscar_posicion(FILE* f, int dni, int* posicion) {
    int pos_inicial = hash_function(dni);

    // Ajustar si el hash retornó valor fuera del rango del archivo (12000-12006)
    if (pos_inicial >= MAX_CORREDORES) {
        pos_inicial = pos_inicial % MAX_CORREDORES;
    }

    int pos_actual = pos_inicial;
    Treg temp;
    int intentos = 0;

    do {
        fseek(f, pos_actual * sizeof(Treg), SEEK_SET);
        fread(&temp, sizeof(Treg), 1, f);

        // Si encontramos el DNI y está activo
        if (temp.dni == dni && temp.activo == ACTIVO) {
            *posicion = pos_actual;
            return 0;  // Encontrado
        }

        // Si la posición está inactiva o vacía, podría ser lugar para insertar
        if (temp.activo == INACTIVO || temp.dni == 0) {
            *posicion = pos_actual;
            return -1;  // No encontrado, pero posición disponible
        }

        // Linear probing: siguiente posición (con wrap-around en MAX_CORREDORES)
        pos_actual = (pos_actual + 1) % MAX_CORREDORES;
        intentos++;

    } while (pos_actual != pos_inicial && intentos < MAX_CORREDORES);

    *posicion = -1;
    return -1;  // Archivo lleno o no encontrado
}

int buscapos(unsigned long int dni, FILE* arch) {
    int pos = hash_function(dni), i;
    Treg registro;

    if (pos > 12000 && pos <= 12006) {  // hash invalido
        pos = 0;                        // reinicia y busca lineal en caso de que la primera posicion este ocupada
    }

    fseek(arch, pos, SEEK_SET);
    fread(&registro, sizeof(registro), 1, arch);

    if (registro.activo) {
        // solucionar colision
        i = pos;
        while (fread(&registro, sizeof(registro), 1, arch) == 1 && registro.activo)
            i++;

        if (registro.activo) {  // si no encotro delante, busca detras
            fseek(arch, 0, SEEK_SET);
            i = 0;
            while (fread(&registro, sizeof(registro), 1, arch) == 1 && registro.activo && i < pos)
                i++;

            if (i < pos && !registro.activo)
                pos = i;  // encontro posicion
            else
                pos = -1;  // archivo lleno
        } else {
            pos = i;
        }
    }
    return pos;
}

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

// Alta de corredor: registra un nuevo corredor usando hash
int alta_corredor(FILE* f, Treg corredor) {
    int pos_inicial = hash_function(corredor.dni);

    // Ajustar si el hash retornó valor fuera del rango del archivo (12000-12006)
    if (pos_inicial >= MAX_CORREDORES) {
        pos_inicial = pos_inicial % MAX_CORREDORES;
    }

    int pos_actual = pos_inicial;
    Treg temp;
    int intentos = 0;

    // Buscar posición libre usando linear probing
    do {
        fseek(f, pos_actual * sizeof(Treg), SEEK_SET);
        fread(&temp, sizeof(Treg), 1, f);

        // Verificar si el DNI ya existe y está activo
        if (temp.dni == corredor.dni && temp.activo == ACTIVO) {
            printf("Error: El DNI ya esta registrado\n");
            return -1;
        }

        // Si encontramos posición libre (inactiva o vacía)
        if (temp.activo == INACTIVO || temp.dni == 0) {
            fseek(f, pos_actual * sizeof(Treg), SEEK_SET);
            fwrite(&corredor, sizeof(Treg), 1, f);
            fflush(f);
            printf("Corredor registrado en posicion: %d\n", pos_actual);
            return 0;
        }

        pos_actual = (pos_actual + 1) % MAX_CORREDORES;
        intentos++;

    } while (pos_actual != pos_inicial && intentos < MAX_CORREDORES);

    printf("Error: No hay cupo disponible\n");
    return -1;
}

// Baja lógica: marca el corredor como inactivo
int baja_corredor(FILE* f, int dni) {
    int posicion;
    Treg corredor;

    if (buscar_posicion(f, dni, &posicion) == 0) {
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fread(&corredor, sizeof(Treg), 1, f);
        corredor.activo = INACTIVO;
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fwrite(&corredor, sizeof(Treg), 1, f);
        fflush(f);
        return 0;
    }
    return -1;
}

// Modifica los datos de un corredor existente
int modificar_corredor(FILE* f, int dni, Treg nuevo_datos) {
    int posicion;

    if (buscar_posicion(f, dni, &posicion) == 0) {
        // Mantener el tiempo anterior si existe
        Treg anterior;
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fread(&anterior, sizeof(Treg), 1, f);

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

// Carga el tiempo de carrera de un corredor
int cargar_tiempo(FILE* f, int dni, char* tiempo) {
    int posicion;
    Treg corredor;

    if (buscar_posicion(f, dni, &posicion) == 0) {
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fread(&corredor, sizeof(Treg), 1, f);
        strcpy(corredor.tiempo, tiempo);
        fseek(f, posicion * sizeof(Treg), SEEK_SET);
        fwrite(&corredor, sizeof(Treg), 1, f);
        fflush(f);
        return 0;
    }
    return -1;
}

// Función auxiliar para comparar tiempos (para qsort)
int comparar_tiempos(const void* a, const void* b) {
    Treg* corredor_a = (Treg*)a;
    Treg* corredor_b = (Treg*)b;

    // Si alguno no tiene tiempo, va al final
    if (strlen(corredor_a->tiempo) == 0) return 1;
    if (strlen(corredor_b->tiempo) == 0) return -1;

    return strcmp(corredor_a->tiempo, corredor_b->tiempo);
}

// Lista todos los tiempos ordenados
void listar_tiempos_general(FILE* f) {
    int i, count = 0;

    printf("\n%-10s %-30s %-10s %-10s\n", "DNI", "Nombre", "Categoria", "Tiempo");
    printf("-----------------------------------------------------------------------\n");
    // Leer todos los corredores activos con tiempo
    fseek(f, 0, SEEK_SET);
    for (i = 0; i < MAX_CORREDORES; i++) {
        Treg temp;
        fread(&temp, sizeof(Treg), 1, f);
        if (temp.activo == ACTIVO && strlen(temp.tiempo) > 0) {
            printf("%-10d %-30s %-10s %-10s\n",
                   temp.dni,
                   temp.nombre,
                   temp.categoria,
                   temp.tiempo);
        }
    }
}

// Lista tiempos filtrados por categoría
void listar_tiempos_categoria(FILE* f, char* categoria) {
    int i, count = 0;

    printf("\nCategoria: %s\n", categoria);
    printf("%-10s %-30s %-10s\n", "DNI", "Nombre", "Tiempo");
    printf("-----------------------------------------------------------------------\n");
    // Leer corredores activos con tiempo de la categoría especificada
    fseek(f, 0, SEEK_SET);
    for (i = 0; i < MAX_CORREDORES; i++) {
        Treg temp;
        fread(&temp, sizeof(Treg), 1, f);
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