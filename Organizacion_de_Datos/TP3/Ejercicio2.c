#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10007  // Tamaño del archivo principal (idealmente un número primo)
// 2) Suponga un sistema para el manejo de personal de una empresa. Los datos a almacenar son apellido y nombre, dirección, teléfono, y fecha de ingreso.
// Para la solución se plantea un archivo directo donde el apellido y nombre es la clave de acceso.
// Implemente una función hash, creación de archivo principal, alta, modificaciones, bajas. El manejo de colisiones se debe resolver por área separada de overflow.
typedef struct {
    char nyap[80];  // Clave de acceso: Apellido y Nombre
    char direccion[100];
    char telefono[20];
    char fecha_ingreso[11];  // Formato DD/MM/AAAA
    int estado;              // 0: Libre, 1: Ocupado, 2: Borrado
} TRegEmpleado;

typedef struct {
    char nyap[80];
} TRegTable;

typedef TRegTable THashTable[M];
THashTable tabla_hash;

int resolver_colision(FILE* archOverflow, int pos_inicial, char* clave, int* encontrado) {
    TRegEmpleado actual;

    // Primer tramo: desde la posicion hash hasta el final de la tabla.
    if (strcmp(tabla_hash[pos_inicial].nyap, clave) == 0) {
        *encontrado = 1;
        return pos_inicial;
    }
    for (int i = pos_inicial; i < TAM_HASH; i++) {
        fseek(archOverflow, pos_inicial * sizeof(TRegEmpleado), SEEK_SET);
        fread(&actual, sizeof(TRegEmpleado), 1, archOverflow);
        if (strcmp(actual.nyap, clave) == 0) {
            *encontrado = 1;
            return i;
        }
        if (actual.estado == 0 || actual.estado == 2) {
            *encontrado = 0;
            return i;
        }
    }

    // Segundo tramo: desde el inicio hasta la posicion hash - 1.
    for (i = 0; i < pos_inicial; i++) {
        fseek(archOverflow, pos_inicial * sizeof(TRegEmpleado), SEEK_SET);
        fread(&actual, sizeof(TRegEmpleado), 1, archOverflow);
        if (strcmp(actual.nyap, clave) == 0) {
            *encontrado = 1;
            return i;
        }
        if (actual.estado == 0 || actual.estado == 2) {
            *encontrado = 0;
            return i;
        }
    }

    *encontrado = 0;
    return -1;
}

int funcion_hash(char* clave) {
    int suma = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        suma += clave[i];
    }
    return suma % M;  // Retorna un índice entre 0 y M-1
}
void crear_archivos() {
    FILE* fp = fopen("principal.dat", "wb");
    FILE* fo = fopen("overflow.dat", "wb");

    TRegEmpleado emp_vacio = {"", "", "", "", 0};  // Estado 0 (Libre), sin overflow

    // Formatear archivo principal
    for (int i = 0; i < M; i++) {
        fwrite(&emp_vacio, sizeof(TRegEmpleado), 1, fp);
    }

    fclose(fp);
    fclose(fo);
    printf("Archivos inicializados correctamente.\n");
}

void alta_empleado(TRegEmpleado nuevo) {
    FILE* fp = fopen("principal.dat", "r+b");
    FILE* fo = fopen("overflow.dat", "r+b");

    int pos_home = funcion_hash(nuevo.nyap);
    TRegEmpleado actual;
    if (strcmp(tabla_hash[pos_home].nyap, nuevo.nyap) == 0) {
        int encontrado;
        int idx = resolver_colision(pos_home, , &encontrado);
        // escribo en archivo overflow
        fseek(fo, (pos_home + idx) * sizeof(TRegEmpleado), SEEK_SET);
        fwrite(&nuevo, sizeof(TRegEmpleado), 1, fo);
    } else {
        // escribo en archivo principal y tabla hash
        strcpy(tabla_hash[pos_home].nyap, nuevo.nyap);
        fseek(fp, pos_home * sizeof(TRegEmpleado), SEEK_SET);
        fwrite(&nuevo, sizeof(TRegEmpleado), 1, fp);
    }

    fclose(fp);
    fclose(fo);
}
void modificar_empleado(char* clave, char* nueva_dir, char* nuevo_tel) {
    FILE* fp = fopen("principal.dat", "r+b");
    FILE* fo = fopen("overflow.dat", "r+b");

    int pos_home = funcion_hash(clave);
    TRegEmpleado actual;
    fseek(fp, pos_home * sizeof(TRegEmpleado), SEEK_SET);
    fread(&actual, sizeof(TRegEmpleado), 1, fp);

    if (actual.estado == 1 && strcmp(tabla_hash[pos_home].nyap, nuevo.nyap) == 0) {
        // lo encontre en el archivo principal, debo modificar.
        strcpy(actual.direccion, nueva_dir);
        strcpy(actual.telefono, nuevo_tel);
        fseek(fp, pos_home * sizeof(TRegEmpleado), SEEK_SET);
        fwrite(&actual, sizeof(TRegEmpleado), 1, fp);
        printf("Modificado en principal.\n");

    } else {
        if (actual.estado == 0 || (actual.estado == 2 && strcmp(tabla_hash[pos_home].nyap, nuevo.nyap) == 0))
            printf("Posicion Invalidad.\n");
        else {
            // debo buscar en overflow
            int encontrado;
            int idx = resolver_colision(pos_home, , &encontrado);
            if (encontrado) {
                fseek(fo, (pos_home + idx) * sizeof(TRegEmpleado), SEEK_SET);
                fwrite(&nuevo, sizeof(TRegEmpleado), 1, fo);
                printf("Modificado en overflow\n");
            }
        }
    }

    fclose(fp);
    fclose(fo);
}
void baja_empleado(char* clave) {
    FILE* fp = fopen("principal.dat", "r+b");
    FILE* fo = fopen("overflow.dat", "r+b");

    int pos_home = funcion_hash(clave);
    TRegEmpleado actual;
    // me fijo si esta en la tabla hash antes de acceder al archivo principal
    // si no esta, puede ser que este en el archivo de overflow
    if (strcmp(tabla_hash[pos_home].nyap, clave) == 0) {
        fseek(fp, pos_home * sizeof(TRegEmpleado), SEEK_SET);
        fread(&actual, sizeof(TRegEmpleado), 1, fp);
        if (actual.estado == 1 && strcmp(tabla_hash[pos_home].nyap, clave) == 0) {
            actual.estado = 2;
            fseek(fp, pos_home * sizeof(TRegEmpleado), SEEK_SET);
            fwrite(&actual, sizeof(TRegEmpleado), 1, fp);
            printf("Baja lógica en principal.\n");
        } else {
            if (actual.estado == 2 && strcmp(tabla_hash[pos_home].nyap, clave) == 0) {
                printf("Posicion invalida \n");
            } else {
                int encontrado;
                int idx = resolver_colision(pos_home, , &encontrado);
                if (encontrado) {
                    fseek(fo, (pos_home + idx) * sizeof(TRegEmpleado), SEEK_SET);
                    fwrite(&actual, sizeof(TRegEmpleado), 1, fo);
                    printf("Baja lógica en overflow\n");
                }
            }
        }
    } else {
        fseek(fo, pos_home * sizeof(TRegEmpleado), SEEK_SET);
        fread(&actual, sizeof(TRegEmpleado), 1, fo);
        int encontrado;
        int idx = resolver_colision(pos_home, , &encontrado);
        if (encontrado) {
            fseek(fo, (pos_home + idx) * sizeof(TRegEmpleado), SEEK_SET);
            fwrite(&actual, sizeof(TRegEmpleado), 1, fo);
            printf("Baja lógica en overflow\n");
        }
    }
    fclose(fp);
    fclose(fo);
}