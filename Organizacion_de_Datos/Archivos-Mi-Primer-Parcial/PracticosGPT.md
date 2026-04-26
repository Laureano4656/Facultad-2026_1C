1\. El Dilema de la Veterinaria (Cálculo de Costos y Estrategia)



Se tienen tres archivos:



MASCOTAS: (ID\_Mascota (E4), Nombre (C20), ID\_Dueño (E4), Especie (C10)). 

10.000 registros.

DUEÑOS: (ID\_Dueño (E4), Nombre (C30), Teléfono (C15)). 

5.000 registros.

CONSULTAS: (ID\_Consulta (E4), ID\_Mascota (E4), Fecha (C8), Diagnóstico (C50)). 

50.000 registros.



El Problema: Se necesita un listado de los Nombres de los Dueños cuyas mascotas (Especie = 'Perro') tuvieron consultas en el mes de Abril 2026.

Consigna: Diseña el camino de acceso más eficiente (qué archivo abrís primero, cuál después y cómo cruzás los datos).

Estima el costo en términos de accesos a disco (I/O). 

Supone que no hay índices y que los archivos están desordenados.

Para pensar: Si pudieras crear un solo índice en cualquiera de los tres archivos, ¿en cuál lo harías y sobre qué campo para reducir drásticamente el costo de esta consulta? Justifica.



Respuesta:

**+1** acceso por entrar a consultas. 

Por cada fecha de consulta = Abril 2026, hago un acceso (**+1**) al archivo de mascotas y si la especia coincide con "Perro" hago un acceso (**+1**) al archivo de dueños para guardar en el listado su nombre. 



Costo de obtener el listado = 1 + N (Numero de consultas de mascotas de Abril 2026, por cada consulta en esta fecha accedo al archivo de mascotas) + M (Numero de perros que hicieron consulta en Abril 2026, accedo al archivo de dueños por cada uno de estos para obtener el nombre de sus dueños).  



1 + N + M, N>=M



\------------------------------------------------------------------------------------------------------------------------------------------------

2\. Ingeniería de Nodos: El B+ Tree "Apretado"

Estás diseñando el índice para el archivo de ALUMNOS de la facultad.



Tamaño de bloque de disco: 4096 bytes (4KB).



Clave (DNI): Entero de 4 bytes.



Puntero/Offset a bloque: Entero de 8 bytes.



Metadata del nodo (n, es\_hoja, etc.): 12 bytes en total.



Consigna:



Calculá el Orden (M) máximo que puede tener este árbol para que un nodo entre exactamente en un bloque de disco. (Mostrá la ecuación).



Si el árbol tiene 3 niveles (Raíz, Intermedio, Hoja) y está lleno al 70% (factor de carga típico), ¿cuántos registros de alumnos puede indexar aproximadamente?



Teórico: En un Árbol B+, ¿por qué es fundamental que las hojas estén doblemente enlazadas si queremos optimizar una consulta como SELECT \* FROM ALUMNOS WHERE DNI > 30.000.000?



Respuesta: 



Cada registro del nodo van a ser 24 bytes, el nodo debe ocupar 4096 bytes 

Entonces: 4096 bytes/bloque / 24 bytes/registro = 170 registros/bloque -> SI en un B+ tenemos M-1 registros en un nodo como maximo, entonces si queremos 170 registros M debe ser 171.



Factor de carga = cant. registros ocupados / cant. registros totales = 0.7



cant. registros totales = 170 + 170\*171 + 170\*171\*171 = 5000210 - Corrección = 170\*171\*171 = 4970970



cant. registros ocupados = 0.7 \* 5000210 = 3500147 - Corrección = 0.7 \* 4970970 = 3479679



Teorico: 



\------------------------------------------------------------------------------------------------------------------------------------------------



3\. Hashing y la "Zona de Desastre"



Tenes un archivo con Organización Directa (Hashing) para gestionar el inventario de un depósito.



Capacidad del archivo: 1000 baldes (buckets). Cada balde puede almacenar 5 registros.



Método de resolución de colisiones: Encadenamiento en zona de rebalse.



Consigna: Si el Factor de Carga llega a 0.95, describí detalladamente qué sucede con el tiempo de acceso (I/O) para una búsqueda exitosa y para una no exitosa.



Si empiezas a notar que muchas claves terminan en el mismo balde (colisiones masivas), ¿el problema es del tamaño del archivo o de la Función de Hash? Justifica cómo elegirías una mejor función.



Justifica: ¿Por qué en un archivo de organización directa es un error conceptual intentar realizar un "Listado ordenado por clave"?



Respuesta:



Factor de carga = 0.95 -> De los 5000 registros hay 4750. 



Búsqueda exitosa: En el mejor de los casos tengo que realizar un solo acceso O(1) y buscar dentro del bucket. En una búsqueda no exitosa vamos a tener O(N) accesos y vamos a necesitar recorrer todos los buckets correspondientes a la clave de hasheo.



El problema es de la function de Hash que no se ajusto correctamente a un factor de carga elevado. Necesitas una función que garantice Uniformidad. Método de la División: Usar el resto de la división por un número primo cercano al tamaño del archivo (h(k) = k \\mod P). Los primos ayudan a distribuir mejor las claves que tienen patrones.









