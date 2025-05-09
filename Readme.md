# Estructura de la práctica

## build

Directorio dónde se debe compilar y ejecutar la práctica. Usar 'cmake ..', 'make' y './main' con sus respectivos parámetros (indicados en la memoria o bien con el comando ./main -h) para poder ejecutar la práctica

## common 

Directorio con ficheros dados por el profesor de uso general a lo largo de las prácticas. No han sido editados en esta práctica.

## datos

Directorio con datos sin procesar tanto de entrada como de salida. contiene:

- Directorio latexTables con un script de python para facilitar la creación de tablas. Importante usar el método 'tablePrint' en el main para obtener datos csv.
- Directorios de pruebas con los outputs de las diferentes pruebas
- Conjuntos de datos de entrada y de salida (cada conjunto de entrada x tiene su correspondiente output_x datos de salida)
- Archivo seeds.txt para introducir semillas específicas

## inc

Directorio con ficheros de cabecera de la práctica. Están subdivididos en los directorios P1 para archivos relativos a la práctica 1 y P2 para archivos relativos a la práctica 2.

## src

Directorio con ficheros cpp de las prácticas. Están subdivididos en los directorios P1 para archivos relativos a la práctica 1 y P2 para archivos relativos a la práctica 2.