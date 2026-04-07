#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 10007  // Tamaño del archivo principal (idealmente un número primo)

typedef struct {
    char nyap[80];  // Clave de acceso: Apellido y Nombre
    char direccion[100];
    char telefono[20];
    char fecha_ingreso[11];  // Formato DD/MM/AAAA
    int estado;              // 0: Libre, 1: Ocupado, 2: Borrado
    int ptr_overflow;        // -1 si no apunta a nada, o el byte/índice en overflow.dat
} Empleado;

typedef struct {
    char nyap[80];
    int overflowIndex;
} TRegTable;

typedef TRegTable THashTable[M];

int funcion_hash(const char* clave) {
    int suma = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        suma += clave[i];
    }
    return suma % M;  // Retorna un índice entre 0 y M-1
}
void crear_archivos() {
    FILE* fp = fopen("principal.dat", "wb");
    FILE* fo = fopen("overflow.dat", "wb");

    Empleado emp_vacio = {"", "", "", "", 0, -1};  // Estado 0 (Libre), sin overflow

    // Formatear archivo principal
    for (int i = 0; i < M; i++) {
        fwrite(&emp_vacio, sizeof(Empleado), 1, fp);
    }

    fclose(fp);
    fclose(fo);
    printf("Archivos inicializados correctamente.\n");
}

void alta_empleado(Empleado nuevo) {
    FILE* fp = fopen("principal.dat", "r+b");
    FILE* fo = fopen("overflow.dat", "r+b");

    nuevo.estado = 1;
    nuevo.ptr_overflow = -1;

    int pos_home = funcion_hash(nuevo.nyap);
    Empleado actual;

    // Leemos el slot principal
    fseek(fp, pos_home * sizeof(Empleado), SEEK_SET);
    fread(&actual, sizeof(Empleado), 1, fp);

    if (actual.estado == 0 || actual.estado == 2) {
        // Lugar libre o borrado en el área primaria.
        // ¡Importante! Si pisamos un borrado, debemos conservar su cadena de overflow
        nuevo.ptr_overflow = actual.ptr_overflow;
        fseek(fp, pos_home * sizeof(Empleado), SEEK_SET);
        fwrite(&nuevo, sizeof(Empleado), 1, fp);
        printf("Alta exitosa en archivo principal.\n");
    } else {
        // Colisión: Hay que ir al área de overflow
        fseek(fo, 0, SEEK_END);
        int pos_nuevo_overflow = ftell(fo) / sizeof(Empleado);  // Índice del nuevo registro

        // Escribimos el nuevo registro al final del overflow
        fwrite(&nuevo, sizeof(Empleado), 1, fo);

        if (actual.ptr_overflow == -1) {
            // Es el primer colisionado, el principal apunta al overflow
            actual.ptr_overflow = pos_nuevo_overflow;
            fseek(fp, pos_home * sizeof(Empleado), SEEK_SET);
            fwrite(&actual, sizeof(Empleado), 1, fp);
        } else {
            // Ya hay una lista de sinónimos, hay que recorrerla hasta el final
            Empleado over_actual;
            int ptr_actual = actual.ptr_overflow;
            int ptr_anterior;

            do {
                ptr_anterior = ptr_actual;
                fseek(fo, ptr_actual * sizeof(Empleado), SEEK_SET);
                fread(&over_actual, sizeof(Empleado), 1, fo);
                ptr_actual = over_actual.ptr_overflow;
            } while (ptr_actual != -1);

            // Actualizamos el puntero del último nodo de la lista
            over_actual.ptr_overflow = pos_nuevo_overflow;
            fseek(fo, ptr_anterior * sizeof(Empleado), SEEK_SET);
            fwrite(&over_actual, sizeof(Empleado), 1, fo);
        }
        printf("Alta exitosa en área de overflow.\n");
    }

    fclose(fp);
    fclose(fo);
}
void modificar_empleado(char* clave, char* nueva_dir, char* nuevo_tel) {
    FILE* fp = fopen("principal.dat", "r+b");
    FILE* fo = fopen("overflow.dat", "r+b");

    int pos_home = funcion_hash(clave);
    Empleado actual;

    fseek(fp, pos_home * sizeof(Empleado), SEEK_SET);
    fread(&actual, sizeof(Empleado), 1, fp);

    // Chequear en principal
    if (actual.estado == 1 && strcmp(actual.nyap, clave) == 0) {
        strcpy(actual.direccion, nueva_dir);
        strcpy(actual.telefono, nuevo_tel);
        fseek(fp, pos_home * sizeof(Empleado), SEEK_SET);
        fwrite(&actual, sizeof(Empleado), 1, fp);
        printf("Modificado en principal.\n");
    } else {
        // Buscar en overflow
        int ptr_actual = actual.ptr_overflow;
        int encontrado = 0;

        while (ptr_actual != -1 && !encontrado) {
            fseek(fo, ptr_actual * sizeof(Empleado), SEEK_SET);
            fread(&actual, sizeof(Empleado), 1, fo);

            if (actual.estado == 1 && strcmp(actual.nyap, clave) == 0) {
                strcpy(actual.direccion, nueva_dir);
                strcpy(actual.telefono, nuevo_tel);
                fseek(fo, ptr_actual * sizeof(Empleado), SEEK_SET);
                fwrite(&actual, sizeof(Empleado), 1, fo);
                printf("Modificado en overflow.\n");
                encontrado = 1;
            } else {
                ptr_actual = actual.ptr_overflow;
            }
        }
        if (!encontrado) printf("Empleado no encontrado.\n");
    }

    fclose(fp);
    fclose(fo);
}
void baja_empleado(char* clave) {
    FILE* fp = fopen("principal.dat", "r+b");
    FILE* fo = fopen("overflow.dat", "r+b");

    int pos_home = funcion_hash(clave);
    Empleado actual;

    fseek(fp, pos_home * sizeof(Empleado), SEEK_SET);
    fread(&actual, sizeof(Empleado), 1, fp);

    if (actual.estado == 1 && strcmp(actual.nyap, clave) == 0) {
        actual.estado = 2;  // Borrado lógico, MANTIENE su ptr_overflow intacto
        fseek(fp, pos_home * sizeof(Empleado), SEEK_SET);
        fwrite(&actual, sizeof(Empleado), 1, fp);
        printf("Baja lógica en principal.\n");
    } else {
        int ptr_actual = actual.ptr_overflow;
        int encontrado = 0;

        while (ptr_actual != -1 && !encontrado) {
            fseek(fo, ptr_actual * sizeof(Empleado), SEEK_SET);
            fread(&actual, sizeof(Empleado), 1, fo);

            if (actual.estado == 1 && strcmp(actual.nyap, clave) == 0) {
                actual.estado = 2;  // Borrado lógico
                fseek(fo, ptr_actual * sizeof(Empleado), SEEK_SET);
                fwrite(&actual, sizeof(Empleado), 1, fo);
                printf("Baja lógica en overflow.\n");
                encontrado = 1;
            } else {
                ptr_actual = actual.ptr_overflow;
            }
        }
        if (!encontrado) printf("Empleado no encontrado para baja.\n");
    }

    fclose(fp);
    fclose(fo);
}