#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
	char car;       
	struct nodo *next; 
} T_nodo;

typedef T_nodo* Str;

Str createStr(); 						// Crea y retorna una cadena vacia (str = NULL)
Str loadStr(); 							// Carga una cadena desde el ingreso por teclado
Str loadStr2(const char* cadena); 		// Crea una cadena a partir de una cadena previamente escrita (const char*)
void printStr(Str s); 					// Muestra la cadena en la salida estandar (pantalla)
Str concatStr(Str s1, Str s2); 			// Concatena dos cadenas y retorna el resultado
Str beforeToken(Str s, char token); 	// Retorna la parte de la cadena antes de la primera aparicion del token
Str afterToken(Str s, char token); 		// Retorna la parte de la cadena despues de la primera aparicion del token
int isinStr(Str s1, Str s2); 			// Verifica si s2 esta contenido en s1 (1 si es verdadero, 0 si es falso)
int equalStr(Str s1,Str s2);			// Verifica si s1 y s2 son iguales (1 si es verdadero, 0 si es falso)
int sizeStr(Str s);						// Retorna el tamanio del string
void limpiarBuffer();					// Modulo auxiliar para limpiar el buffer
Str crearNodo(char c); 					// Crea un nuevo nodo con el caracter especificado
void freeStr(Str* s); 					// Libera toda la memoria utilizada por una cadena
Str copyStr(Str s);             		// Copia cadena
char str_getFirst(Str s);               // Retorna el primer caracter de una cadena.

#endif