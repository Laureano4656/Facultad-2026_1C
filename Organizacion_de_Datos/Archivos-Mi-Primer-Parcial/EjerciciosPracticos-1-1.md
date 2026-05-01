Al momento de definir el tipo de Archivo. ¡Cual es la principal métrica que utilizaría

usted para elegir entre una Organización Indexada Vs Organización Directa?



Que criterio valoro mas entre estos 3



*TIEMPO DE ACCESO*

Organización directa es más rápida (En el mejor caso O(1)) en cambio Organización indexada O(log(n))

* Volatilidad: si voy a hacer muchas inserciones o no es fundamental. Si tenemos alta volatibilidad es mejor recurrir a una Organización Indexada



*COSTO DE ALMACENAMIENTO*

En la organización indexada solo es necesario guardar un espacio para el índice (1 archivo puro de indices) y los datos se encuentran compactos. Por otro lado en la organización directa requiere un archivo propio para el archivo de datos y según como resolvemos las colisiones podemos llegar a necesitar un archivo más (Area de Overflow) o necesitaríamos aumentar el tamaño del archivo (Direccionamiento cerrado - Sondeo Lineal, cuadrático)



*FACILIDAD DE USO*

En la organización directa es mas compleja de tratar ya que se debe establecer una relación entre la clave primaria de los registros y su posición física en el archivo, además hay que tener en cuenta la posibilidad de colisiones y como las vamos a resolver. 



Respuesta valida para el parcial:

"La volatilidad actúa como un criterio eliminatorio antes que los otros dos. Esto es clave:

Si el archivo tiene alta volatilidad (muchas inserciones/eliminaciones frecuentes), la Organización Directa se degrada rápidamente. Las colisiones aumentan, el área de overflow crece, y lo que era O(1) en el mejor caso empieza a acercarse a O(n) en la práctica. El archivo pierde su propiedad más valiosa.

Con Organización Indexada, las inserciones se manejan de forma ordenada y el índice se mantiene consistente sin degradar el rendimiento estructuralmente."



\------------------------------------------------------------------------------------------------------------------------------------------------



Se tiene un registro de longitud fija con un identificador (ID) y un archivo de 100 posiciones en el cual las primeras 25 ya están ocupadas. Se desea realizar una organización directa en el espacio disponible. Se propone utilizar una función de dispersión del tipo ID MOD 101. ¿Es óptima esta solución? En caso afirmativo, justificar. Caso contrario, proponer una mejor función y explicar.



No. La function de dispersión que propongo es la siguiente:



ID MOD 79 + 25. De esta forma me aseguro no caer en las primeras 25 posiciones, evitando así las colisiones en un principio. En caso de que la función de hashing devolviera 100, 101, 102 o 103 ser realizaría un sondeo lineal negativo



\----------------------------------------------------------------------------------------------------------------------------



Dada la siguiente definición conceptual:
Libros_Biblioteca(
(ISBN)ie,
(TITULO),
(EDITORIAL)d,
(AUTOR)d,
(CANT_PAGINAS),
(EJEMPLAR_PARA_PRESTAR),
(GENERO)d?,
(DESCRIPCION)?,
(UBICACION)
)

a) Determine la cantidad mínima de archivos involucrados. Proponga un nombre apropiado para cada uno de ellos.
b) Se conoce que el factor de bloqueo lógico es de 256 bytes y el factor de bloqueo físico es de 2KB.
Construya la definición lógica de la definición conceptual dada a fin de formar un registro de longitud fija.
c) Se espera tener una alta volatilidad. ¿Es adecuada la definición lógica construida? Justifique su respuesta.


a) Se necesitan por lo menos 4 archivos: Uno para los libros, otro para la editorial, otro para el autor y el cuarto para el género del libro.
Para los libros "Libros", para la editorial "Editoriales", para el autor "Autores" y para el género "Géneros"

b) Libros_Biblioteca(
    (ISBN): C13,
    (TITULO): C36,
    (EDITORIAL): E2,
    (AUTOR): E2,
    (CANT_PAGINAS): E2,
    (EJEMPLAR_PARA_PRESTAR): L,
    (GENERO): E2?,
    (DESCRIPCION): C50?,
    (UBICACION): C20
)

La longitud total es 128 asi entran dos registros en un bloque logico

c) La definicion es adecuada porque al ser un registro fija se accede de forma directa y se puede eliminar facilmente y para insertar se ocupa un espacio libre, aunque si el archivo esta ordenado se complicaria un poco mas




\----------------------------------------------------------------------------------------------------------------------------



Suponer un árbol B+ que está alojado en memoria RAM. ¿Qué estructura utilizaría para realizar su persistencia?
Describa el proceso de recuperación a memoria desde la estructura propuesta.

La estructura que usaria es un archivo secuencial en el que cada registro sea un nodo con tamaño igual a un bloque fisico
y con esta estructura:

1 entero para indicar cuantas claves tiene un nodo dado.
1 vector de capacidad M-1 para almacenar las claves del nodo.
1 entero para indicar la cantidad de hijos; n<=m
1 vector de capacidad M que almacena los enlaces a los hijos
En el caso que sea hoja, un puntero a la siguiente hoja

Segun Claude:
TIPO_NODO   E1              I = interno / H = hoja
N_CLAVES    E2              Cantidad de claves activas
CLAVES[]    vector de M-1   Claves del nodo
HIJOS[]     vector de M     Posiciones en el archivo de los nodos hijo
PTR_HERMANO E4              Solo hojas: posición del siguiente nodo hoja en el archivo. En nodos internos se ignora


Proceso de recuperación a memoria:

Paso 1 — Leer la raíz
Paso 2 — Recorrer el archivo y crear todos los nodos en RAM
Paso 3 — Segunda pasada: Con los nodos ya creados debo enlazarlos




\----------------------------------------------------------------------------------------------------------------------------




Probabilidad Infima de Colisiones -> Sondeo Lineal. Si casi nunca hay choques, buscás el lugar de al lado y listo. Es simple y no requiere estructuras extra.

Gran cantidad de colisiones en pocas claves -> Encadenamiento. En lugar de concentrar todas las claves que colisionan en un mismo punto de la tabla principal, mediante el encadenamiento las guardo fuera de la tabla principal

Gran cantidad de colisiones dispersas en las claves -> Doble Hashing. A diferencia del sondeo lineal, este utiliza una segunda función hash para calcular el "salto" o intervalo entre pruebas. Permitiendo asi una distribucion mas uniforme en el arreglo de la tabla. 













