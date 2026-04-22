Cuando en los Indices secundarios utilizo la clave primaria como

referencia es necesario reorganizar el índice secundario en la eliminación

F



Al momento de definir el tipo de Archivo. ¡Cual es la principal métrica que utilizaría

usted para elegir entre una Organización Indexada Vs Organización Directa?

Una de las principales métricas es la volatilidad de los datos, si voy a hacer muchas inserciones o no es fundamental.

DEPENDE DEL OBJETIVO DEL ARCHIVO.



Las Hashing perfectas producen colisiones que se pueden solucionar por medio

del Direccionamiento abierto o del Separación de desborde

F



Elije la correcta



Frases y términos próximos: Registra si esta contenido y su posición en cada Archivo



Modelo booleano: Registra si esta contenido en cada Archivo



Modelo Vectorial: Registra si esta contenido y su frecuencia en cada Archivo



Los Archivos de **Datos Maestros** contienen datos de un sistema de información que

representan entidades de existencia real o ideal, por ejemplo productos o servicios, o

valores de Referencia para determinar características o atributos de otros datos. En cambio los

archivos de **Datos Transaccionales** contienen Registros de hechos o eventos

relacionados con datos del archivo de **Datos Maestros** , por ejemplo de ventas de productos o de

prestaciones de servicios.



Elegir

Definición de bloques o unidades de organización dentro de un archivo:Diseño logico



La definición conceptual de datos implica la definición de atributos, sin especificar tipos o

dominios: Diseño conceptual





Que entiende por Archivo Invertido:

Es un tipo archivo indexado organizado por claves secundarias basadas en la clave primaria.



Resolver la colisión almacenando la

misma fuera del área principal del archivo relativo: Area de separacion



Resolver la colisión buscando una nueva dirección en el archivo relativo: Progressive Overflow





Clasificacion de archivos por tipo de accesso

Secuencial

Directo

Stream

Relativo

Indexado





Cuales son los métodos de ordenamientos válidos para ordenar en memoria externa.



Mezcla directa

Natural sort



En la organización secuencial con registros Ordenados:

La inserción directa consiste Crear un nuevo archivo con las altas ordenadas; luego

insertar por mezcla.

F





Método de la mitad del cuadrado: Extracción

Dividir la clave en partes iguales y sumarlas: FOLD \& ADD

La clave se divide entre el número de direcciones: Modulo



Los arboles B nunca son binarios. Son arboles de búsqueda, se denominan B-trees.

F





Se tiene un registro de longitud fija con un identificador (ID) y un archivo de 100

posiciones en el cual las primeras 25 ya están ocupadas. Se desea realizar una

organización directa en el espacio disponible. Se propone utilizar una función de

dispersión del tipo ID MOD 101. ¿Es óptima esta solución? En caso afirmativo,

justificar. Caso contrario, proponer una mejor función y explicar.



MOD 75 +25





Los Montículo Binarios, son una buena solución para la implementación de indices

para la Organización indexada

F



En la organización secuencial con registros Ordenados:

La única manera de Manera de hacer una modificación es hacerlo por Apareo

Maestro-Novedades-Nuevo Maestro

F



La aplicación más común, de un Apareo es para generar reportes que acumulen

cantidades. Se utilizan contadores, Sumadores, Promedios.

Se informan SubTotales por Niveles y Totales Generales

F





En la Organización indexada Si existe alta volatilidad es conveniente que los indices

secundarios apunten a la dirección relativa de los registros.

F



Se compacta el bloque donde se encontró

el registro; se actualiza el espacio libre del

bloque y se actualiza el bloque. : RRLV



Se Busca el registro a eliminar; se toma su

posición; se graba el ultimo Registro del

archivo en la posición a eliminar y trunca

el archivo: RRLF



Posicionamiento al inicio del archivo y

lectura secuencial de registros hasta

encontrar el buscado o llegar al final del

archivo. Solo búsqueda linea: Recuperación de Registros an Archivos

secuenciales



Para las siguientes afirmaciones: Todas las hojas se encuentran en el mismo nivel, el

mas bajo.Los nodos hoja se encuentran unidos entre sí como una

lista enlazada

Arboles B+



En la Organización directa Es posible evitar el Hashing cuando la clave es

Alfanumerica

F



Es el conjunto de todas las palabras distintas que aparecen en el texto

Diccionario.



El corte de control consiste que a partir de dos archivos (uno principal y otro

relacionado) y tienen alguna información que los enlace, generar un tercero (o una

salida por pantalla), como una mezcla de los dos.

F



Dada la siguiente definición conceptual:

Libros\_Biblioteca(

(ISBN)ie,

(TITULO),

(EDITORIAL)d,

(AUTOR)d,

(CANT\_PAGINAS),

(EJEMPLAR\_PARA\_PRESTAR),

(GENERO)d?,

(DESCRIPCION)?,

(UBICACION)

)

a) Determine la cantidad mínima de archivos involucrados. Proponga un nombre

apropiado para cada uno de ellos.

b) Se conoce que el factor de bloqueo lógico es de 512 bytes y el factor de bloqueo

físico es de 4KB. Construya la definición lógica de la definición conceptual dada a fin

de formar un registro de longitud fija.

c) Se espera tener una alta volatilidad. ¿Es adecuada la definición lógica construida?

Justifique su respuesta



a) Como mínimo tengo Libros, Editorial, Autor, Genero

b)

Libros\_Biblioteca(

(ISBN)ie C13,

(TITULO) C50,

(EDITORIAL)d E2,

(AUTOR)d E2,

(CANT\_PAGINAS) E2,

(EJEMPLAR\_PARA\_PRESTAR) L,

(GENERO)d  E2,

(DESCRIPCION) C410,

(UBICACION) C30

)



c) Si, es adecuada porque la definición lógica es igual al factor de bloqueo lógico optimizando las lecturas y escrituras.

