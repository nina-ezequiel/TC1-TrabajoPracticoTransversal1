#ifndef TREE_H
#define TREE_H

#include "String.h"
#include <string.h>

#define STR 1 
#define SET 2
#define LIST 3

typedef struct dataType {
	int nodeType;
	union {
		Str Str;
		struct {
			struct dataType* data; // Primer elemento (coleccion interna)
			struct dataType* next; // Siguiente elemento en la coleccion
		};
	};
} DataType;

typedef DataType* tData;

tData copy_tData(tData original);						// Realiza una copia profunda de un nodo (incluyendo subestructuras)
int equal_tData(tData elem1, tData elem2);				// Devuelve 1 si son iguales, 0 en caso contrario
tData newNodeTree(int tipo);							// Crea un nodo vacio del tipo indicado (STR, LIST o SET)
tData newNodeStr();										// Crea un nodo STR pidiendo un string por teclado
tData newNodeList();									// Crea una LIST interactiva (puede contener STR, LIST o SET)
tData newNodeSet();										// Crea un SET interactivo (sin elementos repetidos)
void append(tData *coleccion, tData elemento);			// Agrega un elemento al final de una colecci�n (LIST o SET)
void insert_set(tData *coleccion, tData elemento);		// Agrega un elemento en un SET (sin repetidos)
void printDataRecursive(tData data);
void printData(tData data);								// Muestra la estructura completa en pantalla
void free_tData(tData data);							// Libera toda la memoria de la estructura
int pertainSet(tData elemento, tData set);				// Verifica si un elemento pertenece al conjunto
int inclusionSet(tData set1, tData set2);				// Verifica si set1 esta incluido en set2 (set1 c set2)
tData unionSet(tData set1, tData set2);					// Union de dos conjuntos (A u B)
tData intersectionSet(tData set1, tData set2);			// Interseccion de dos conjuntos (A n B)
tData differenceSet(tData set1, tData set2);			// Diferencia de conjuntos (A - B)
int cardinalitySet(tData set);							// Devuelve la cantidad de elementos del conjunto
tData removeFromSet(tData set, tData elemento);			// Elimina un elemento del conjunto
tData newNodeStrHard(Str valor);						// Crea nodo STR a partir de un string dado
tData newEmptyNodeList();								// Crea LIST vacia
tData newEmptyNodeSet();								// Crea SET vacio
tData concatData(tData l1, tData l2);					// Concatena dos listas (retorna nueva lista)
tData search(tData list, tData elem);					// Busca un elemento en una lista (retorna copia del elemento encontrado o NULL)
tData createPair(tData first, tData second);			// Funcion auxiliar para crear un par ordenado (como lista de 2 elementos)
tData cartesianProduct(tData col1, tData col2);			// Producto cartesiano entre dos estructuras (pueden ser STR, LIST o SET)
tData strToList(tData str);								// Conversion STR -> LIST
tData listToStr(tData list);							// Conversion LIST -> STR
tData strToSet(tData str);								// Conversion STR -> SETtData setToStr(tData set);								
tData setToStr(tData set);								// Conversion SET -> STR
tData strToSetToken(Str s, char token);   

int tData_getType(tData d);               				// obtiene nodeType
Str tData_getStr(tData d);                				// obtiene el string si es STR
tData tData_getFirst(tData d);            				// primer elemento de LIST/SET
tData tData_getNext(tData d);            				// siguiente elemento en la lista
void tData_addToSet(tData set, tData elem); 			// agrega elem al conjunto set

tData parseTree(const char* str);

#endif
