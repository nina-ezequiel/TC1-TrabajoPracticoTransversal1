#include "String.h"

// Crea un nuevo nodo
Str crearNodo(char c) {
	Str nvo = (Str)malloc(sizeof(T_nodo));
	nvo->car = c;
	nvo->next = NULL;
	return nvo;
}

// libera memoria
void freeStr(Str *s) {
	while (*s != NULL) {
		Str cop = *s;
		*s = (*s)->next;
		free(cop);
	}
	*s = NULL;
}

// Crea cadena vacia
Str createStr() {
	return NULL;
}

// Lee cadena desde teclado
Str loadStr() {
	Str ini = NULL;
	Str ult = NULL;
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
		Str nvo = crearNodo(c);
		if (ini == NULL) {
			ini = nvo;
			ult = nvo;
		} else {
			ult->next = nvo;
			ult = nvo;
		}
	}
	return ini;
}

// Crea cadena desde const char*
Str loadStr2(const char* cadena) {
	Str ini = NULL;
	Str ult = NULL;
	for (int i = 0; cadena[i] != '\0'; i++) {
		Str nvo = crearNodo(cadena[i]);
		if (ini == NULL) {
			ini = nvo;
			ult = nvo;
		} else {
			ult->next = nvo;
			ult = nvo;
		}
	}
	return ini;
}

// Muestra la cadena
void printStr(Str s) {
	if(s != NULL){
		Str temp = s;
		printf("\"");
		while (temp != NULL) {
			printf("%c", temp->car);
			temp = temp->next;
		}
		printf("\"");
	}
	else{
		printf("\nString Vacio ...");
	}
}

// Copia una cadena
Str copyStr(Str s){
	Str ini = NULL;
	Str fin = NULL;
	Str temp = s;
	while(temp != NULL){
		Str nvo = crearNodo(temp->car);
		if(ini == NULL){
			ini = nvo;
			fin = nvo;
		}
		else{
			fin->next = nvo;
			fin = nvo;
		}
		temp = temp->next;
	}
	return ini;
}
	
// Concatena dos cadenas
Str concatStr(Str s1, Str s2) {
	Str cop1 = copyStr(s1);
	Str cop2 = copyStr(s2);
	if (cop1 == NULL && cop2 == NULL) {
		return NULL;
	}
	if (cop1 == NULL) {
		return cop2;
	}
	if (cop2 == NULL) {
		return cop1;
	}
	Str temp = cop1;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = cop2;
	
	return cop1;
}
	
// Parte antes del token
Str beforeToken(Str s, char token) {
	if (s == NULL) 
		return NULL;
	Str temp = s;
	Str ini = NULL;
	Str ult = NULL;
	while (temp != NULL && temp->car != token) {
		Str nvo = crearNodo(temp->car);
		if (ini == NULL) {
			ini = ult = nvo;
		} 
		else {
			ult->next = nvo;
			ult = nvo;
		}
		temp = temp->next;
	}
	if (temp == NULL) {
		return copyStr(s);
	}
	return ini;
}
	
// Parte despues del token
Str afterToken(Str s, char token) {
	if (s == NULL) 
		return NULL;
	Str temp = s;
	while (temp != NULL && temp->car != token) {
		temp = temp->next;
	}
	if (temp == NULL || temp->next == NULL) {
		return NULL;
	}
	return copyStr(temp->next);
}
	
// Verifica si s2 esta en s1
int isinStr(Str s1, Str s2) {
	if (s2 == NULL) 
		return 1;
	if (s1 == NULL) 
		return 0;
	Str temp = s1;
	while (temp != NULL) {
		if (temp->car == s2->car) {
			Str ss1 = temp;
			Str ss2 = s2;
			while (ss1 != NULL && ss2 != NULL && ss1->car == ss2->car) {
				ss1 = ss1->next;
				ss2 = ss2->next;
			}
			if (ss2 == NULL) 
				return 1;
		}
		temp = temp->next;
	}
	return 0;
}
	
// Verifica si s1 y s2 son iguales
int equalStr(Str s1,Str s2){
	if(isinStr(s1,s2) && isinStr(s2,s1))
		return 1;
	else
		return 0;
}
	
// Retorna tamanio del string
int sizeStr(Str s){
	int c = 0;
	while(s != NULL){
		c++;
		s = s->next;
	}
	return c;
}
	
// limpia buffer
void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

char str_getFirst(Str s){
	return s->car;
}
