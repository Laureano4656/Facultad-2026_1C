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



\------------------------------------------------------------------------------------------------------------------------------------------------





















