#include "Tree.h"
#include "String.h"
#include <string.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */

tData copy_tData(tData original) {
	if (original == NULL) 
		return NULL;
	tData copia = newNodeTree(original->nodeType);
	switch(original->nodeType) {
		
	case STR:
		copia->Str = copyStr(original->Str);
		break;
		
	case LIST:
	case SET: {
		tData actual = original->data;
		tData inicio = NULL;
		tData fin = NULL;		
		while (actual != NULL) {
			tData elemCopia = copy_tData(actual);
			if (inicio == NULL) {
				inicio = elemCopia;
				fin = elemCopia;
			} 
			else {
				fin->next = elemCopia;
				fin = elemCopia;
			}
			actual = actual->next;
		}
		copia->data = inicio;
		break;
	}
	}
	return copia;
}

int equal_tData(tData elem1, tData elem2) {
	if (elem1 == NULL || elem2 == NULL) 
		return 0;	
	if (elem1->nodeType != elem2->nodeType) 
		return 0;
	switch(elem1->nodeType) {
		
	case STR:
		return equalStr(elem1->Str, elem2->Str);
		
	case LIST: {
		tData actual1 = elem1->data;
		tData actual2 = elem2->data;
		
		while (actual1 != NULL && actual2 != NULL) {
			if (!equal_tData(actual1, actual2)) 
				return 0;
			actual1 = actual1->next;
			actual2 = actual2->next;
		}
		
		return (actual1 == NULL && actual2 == NULL);
	}
	case SET:
		return inclusionSet(elem1, elem2) && inclusionSet(elem2, elem1);
	}
	
	return 0;
}

/* --------------------------------------------- */

tData newNodeTree(int tipo) {
	tData nvo = (tData)malloc(sizeof(DataType));
	nvo->nodeType = tipo;
	nvo->data = NULL;
	nvo->next = NULL;
	return nvo;
}

tData newNodeStr() {
	printf("Ingrese el string: ");
	Str valor = loadStr();
	tData nvo = newNodeTree(STR);
	if (nvo == NULL || valor == NULL) {
		freeStr(&valor);
		if (nvo != NULL) 
			free(nvo);
		return NULL;
	}
	nvo->Str = valor;
	return nvo;
}

tData newNodeList() {
	tData Lista = NULL;
	int opcion;
	do {
		printf("\n1. Agregar String.");
		printf("\n2. Agregar Lista.");
		printf("\n3. Agregar Conjunto.");
		printf("\n4. Terminar lista.");
		printf("\nOpcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();
		
		tData ElementoLista = NULL;
		switch(opcion) {
			
		case 1:
			ElementoLista = newNodeStr();
			break;
			
		case 2:
			ElementoLista = newNodeList();
			break;
			
		case 3:
			ElementoLista = newNodeSet();
			break;
			
		case 4:
			break;
			
		default:
			printf("Opcion no valida.\n");
			continue;
		}
		
		if (ElementoLista != NULL)
			append(&Lista, ElementoLista);
	} while(opcion != 4);
	
	tData nodoLista = newEmptyNodeList();
	nodoLista->data = Lista;
	return nodoLista;
}

tData newNodeSet() {
	tData Conjunto = NULL;
	int opcion;
	do {
		printf("\n1. Agregar String.");
		printf("\n2. Agregar Lista.");
		printf("\n3. Agregar Conjunto.");
		printf("\n4. Terminar conjunto.");
		printf("\nOpcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();
		
		tData ElementoSet = NULL;
		
		switch(opcion) {
			
		case 1:
			ElementoSet = newNodeStr();
			break;
			
		case 2:
			ElementoSet = newNodeList();
			break;
			
		case 3:
			ElementoSet = newNodeSet();
			break;
			
		case 4:
			break;
			
		default:
			printf("Opcion no valida.\n");
			continue;
		}
		
		if (ElementoSet != NULL) {
			insert_set(&Conjunto,ElementoSet);
		}
		
	} while(opcion != 4);
	
	tData nodoConjunto = newEmptyNodeSet();
	nodoConjunto->data = Conjunto;
	return nodoConjunto;
}

/* ------------------------------------------------------- */

void insert_set(tData *coleccion,tData elemento){
	tData actual = *coleccion;
	int existe = 0;
	while (actual != NULL && !existe) {
		if (equal_tData(actual, elemento)) {
			existe = 1;
		}
		actual = actual->next;
	}
	if (existe) {
		free_tData(elemento);
	} 
	else
		append(&(*coleccion), elemento);
}
	
void append(tData *coleccion, tData elemento) {
	if (coleccion == NULL || elemento == NULL) 
		return;
	if (*coleccion == NULL) {
		*coleccion = elemento;
	} 
	else {
		tData aux = *coleccion;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = elemento;
	}
	elemento->next = NULL;
}

/* ------------------------------------------------------ */

void printDataRecursive(tData data) {
	int firstElement = 1;
	while (data != NULL) {
		if (!firstElement) {
			printf(", ");
		}
		firstElement = 0;
		switch(data->nodeType) {
		case STR:
			printStr(data->Str);
			break;
		case LIST:
			printf("[");
			printDataRecursive(data->data);
			printf("]");
			break;
		case SET:
			printf("{");
			printDataRecursive(data->data);
			printf("}");
			break;
		}
		data = data->next;
	}
}

void printData(tData data) {
	if (data == NULL) {
		printf("NULL");
		return;
	}
	printDataRecursive(data);
	//printf("\n");
}

void free_tData(tData data) {
	while (data != NULL) {
		tData temp = data;
		data = data->next;
		switch(temp->nodeType) {
		case STR:
			freeStr(&(temp->Str));
			break;
		case LIST:
		case SET:
			free_tData(temp->data);
			break;
		}
		free(temp);
	}
}

/* -------------------------------------------------- */

int pertainSet(tData elemento, tData set) {
	if (elemento == NULL || set == NULL || set->nodeType != SET) 
		return 0;
	tData actual = set->data;
	while (actual != NULL) {
		if (equal_tData(elemento, actual)) {
			return 1;
		}
		actual = actual->next;
	}
	return 0;
}

int inclusionSet(tData set1, tData set2) {
	if (set1 == NULL) 
		return 1;
	if (set2 == NULL) 
		return 0;
	tData actual = set1->data;
	while (actual != NULL) {
		if (!pertainSet(actual, set2)) {
			return 0;
		}
		actual = actual->next;
	}
	return 1;
}

tData unionSet(tData set1, tData set2) {
	if (set1 == NULL && set2 == NULL) 
		return NULL;
	tData resultado = newEmptyNodeSet();
	// Copiar elementos de set1
	tData actual = (set1 != NULL) ? set1->data : NULL;
	while (actual != NULL) {
		tData copia = copy_tData(actual);
		append(&(resultado->data), copia);
		actual = actual->next;
	}
	// Agregar elementos de set2 que no est�n ya
	actual = (set2 != NULL) ? set2->data : NULL;
	while (actual != NULL) {
		tData copia = copy_tData(actual);
		insert_set(&(resultado->data), copia);
		actual = actual->next;
	}
	return resultado;
}

tData intersectionSet(tData set1, tData set2) {
	if (set1 == NULL || set2 == NULL) 
		return NULL;
	tData resultado = newEmptyNodeSet();
	tData actual = set1->data;
	while (actual != NULL) {
		if (pertainSet(actual, set2)) {
			tData copia = copy_tData(actual);
			append(&(resultado->data), copia);
		}
		actual = actual->next;
	}
	return resultado;
}

tData differenceSet(tData set1, tData set2) {
	if (set1 == NULL) 
		return NULL;
	tData resultado = newEmptyNodeSet();
	tData actual = set1->data;
	while (actual != NULL) {
		if (set2 == NULL || !pertainSet(actual, set2)) {
			tData copia = copy_tData(actual);
			append(&(resultado->data), copia);
		}
		actual = actual->next;
	}
	return resultado;
}

int cardinalitySet(tData set) {
	if (set == NULL || set->nodeType != SET) 
		return 0;
	int count = 0;
	tData actual = set->data;
	while (actual != NULL) {
		count++;
		actual = actual->next;
	}
	return count;
}

tData removeFromSet(tData set, tData elemento) {
	if (set == NULL || set->nodeType != SET || elemento == NULL)
		return set;	
	tData anterior = NULL;
	tData actual = set->data;
	while (actual != NULL) {
		if (equal_tData(actual, elemento)) {
			tData aEliminar = actual;
			if (anterior == NULL) {
				set->data = actual->next;
			} 
			else {
				anterior->next = actual->next;
			}
			actual = actual->next;
			aEliminar->next = NULL;
			free_tData(aEliminar);
			continue;
		}
		anterior = actual;
		actual = actual->next;
	}
	return set;
}

/* ------------------------------------------- */

tData newNodeStrHard(Str valor) {
	tData nvo = newNodeTree(STR);
	if (nvo != NULL) {
		nvo->Str = copyStr(valor);
	}
	return nvo;
}

tData newEmptyNodeList() {
	tData nvo = newNodeTree(LIST);
	return nvo;
}

tData newEmptyNodeSet() {
	tData nvo = newNodeTree(SET);
	return nvo;
}

/* --------------------------------------------------------------------- */

tData concatData(tData l1, tData l2) {
	if (l1 == NULL || l2 == NULL) 
		return NULL;
	if (l1->nodeType != LIST || l2->nodeType != LIST)
		return NULL;
	tData resultado = newEmptyNodeList();
	// Copiar todos los elementos de l1
	tData actual = l1->data;
	while (actual != NULL) {
		tData copia = copy_tData(actual);
		append(&(resultado->data), copia);
		actual = actual->next;
	}
	// Copiar todos los elementos de l2
	actual = l2->data;
	while (actual != NULL) {
		tData copia = copy_tData(actual);
		append(&(resultado->data), copia);
		actual = actual->next;
	}
	return resultado;
}

tData search(tData list, tData elem) {
	if (list == NULL || elem == NULL)
		return NULL;
	if (list->nodeType != LIST)
		return NULL;
	tData actual = list->data;
	while (actual != NULL) {
		if (equal_tData(actual, elem)) {
			return copy_tData(actual); // Retorna copia de elemento encontrado
		}
		actual = actual->next;
	}
	return NULL; // No encontrado
}

/* --------------------------------------------------------------------- */

tData createPair(tData first, tData second) {
	tData pair = newEmptyNodeList();
	tData copyFirst = copy_tData(first);
	tData copySecond = copy_tData(second);
	append(&(pair->data), copyFirst);
	append(&(pair->data), copySecond);
	return pair;
}

tData cartesianProduct(tData col1, tData col2) {
	if (col1 == NULL || col2 == NULL)
		return NULL;
	tData resultado = newEmptyNodeSet();
	tData elem1 = col1->data;
	while (elem1 != NULL) {
		tData elem2 = col2->data;
		while (elem2 != NULL) {
			tData pair = createPair(elem1, elem2);
			if (pair != NULL) {
				insert_set(&(resultado->data), pair);
			}
			elem2 = elem2->next;
		}
		elem1 = elem1->next;
	}
	return resultado;
}

tData strToList(tData string) {
	if (string == NULL || string->nodeType != STR)
		return NULL;
	tData lista = newEmptyNodeList();
	if (lista == NULL)
		return NULL;
	Str actual = string->Str;
	while (actual != NULL) {
		char caracter[2] = {actual->car, '\0'};
		tData charStr = newNodeStrHard(loadStr2(caracter));
		append(&(lista->data), charStr);
		actual = actual->next;
	}
	return lista;
}

tData listToStr(tData list) {
	if (list == NULL || list->nodeType != LIST)
		return NULL;
	Str resultadoStr = createStr();
	tData actual = list->data;
	while (actual != NULL) {
		if (actual->nodeType == STR) {
			Str temp = copyStr(actual->Str);
			Str nueva = concatStr(resultadoStr, temp);
			freeStr(&resultadoStr);
			freeStr(&temp);
			resultadoStr = nueva;
		}
		actual = actual->next;
	}
	tData resultado = newNodeTree(STR);
	resultado->Str = resultadoStr;
	return resultado;
}

tData strToSet(tData string) {
	if (string == NULL || string->nodeType != STR)
		return NULL;
	tData conjunto = newEmptyNodeSet();
	if (conjunto == NULL)
		return NULL;	
	Str actual = string->Str;
	while (actual != NULL) {
		char caracter[2] = {actual->car, '\0'};
		tData charStr = newNodeStrHard(loadStr2(caracter));
		insert_set(&(conjunto->data), charStr);
		actual = actual->next;
	}
	return conjunto;
}

tData setToStr(tData set) {
	if (set == NULL || set->nodeType != SET)
		return NULL;
	Str resultadoStr = createStr();  // string vac�o
	tData actual = set->data;
	while (actual != NULL) {
		if (actual->nodeType == STR) {
			Str temp = copyStr(actual->Str);
			Str nueva = concatStr(resultadoStr, temp);
			freeStr(&resultadoStr);
			freeStr(&temp);
			resultadoStr = nueva;
		}
		actual = actual->next;
	}
	tData resultado = newNodeTree(STR);
	resultado->Str = resultadoStr;
	return resultado;
}

// Convierte una cadena "elem1,elem2,elem3" en un conjunto (SET) de strings
tData strToSetToken(Str s, char token) {
	tData set = newEmptyNodeSet();
	if (set == NULL || s == NULL) 
		return set;
	Str resto = copyStr(s);
	while (resto != NULL) {
		Str before = beforeToken(resto, token);
		if (before != NULL) {
			tData elem = newNodeStrHard(before);
			insert_set(&(set->data), elem);
			freeStr(&before);
		}
		Str nuevoResto = afterToken(resto, token);
		freeStr(&resto);
		resto = nuevoResto;
	}
	return set;
}

int tData_getType(tData d) {
	if (d == NULL) 
		return 0;
	return d->nodeType;
}

Str tData_getStr(tData d) {
	if (d == NULL || d->nodeType != STR) 
		return NULL;
	return d->Str;
}

tData tData_getFirst(tData d) {
	if (d == NULL) 
		return NULL;
	if (d->nodeType == LIST || d->nodeType == SET)
		return d->data;
	return NULL;
}

tData tData_getNext(tData d) {
	if (d == NULL) 
		return NULL;
	return d->next;
}

void tData_addToSet(tData set, tData elem) {
	if (set == NULL || elem == NULL) 
		return;
	insert_set(&(set->data), elem);
}

/* -------------------- Funciones auxiliares para parseTree -------------------- */

static tData parseElement(const char* str, int* pos) {
    while (str[*pos] == ' ' || str[*pos] == '\t' || str[*pos] == '\n') (*pos)++;
    char c = str[*pos];
    if (c == '\0') return NULL;

    if (c == '{') {
        (*pos)++;
        tData setNode = newEmptyNodeSet();
        if (!setNode) return NULL;
        while (str[*pos] != '}' && str[*pos] != '\0') {
            tData elem = parseElement(str, pos);
            if (elem) {
                insert_set(&(setNode->data), elem);
            } else {
                free_tData(setNode);
                return NULL;
            }
            while (str[*pos] == ' ' || str[*pos] == '\t' || str[*pos] == '\n') (*pos)++;
            if (str[*pos] == ',') {
                (*pos)++;
                while (str[*pos] == ' ' || str[*pos] == '\t' || str[*pos] == '\n') (*pos)++;
            } else {
                break;
            }
        }
        if (str[*pos] == '}') {
            (*pos)++;
            return setNode;
        } else {
            free_tData(setNode);
            return NULL;
        }
    } else if (c == '[') {
        (*pos)++;
        tData listNode = newEmptyNodeList();
        if (!listNode) return NULL;
        while (str[*pos] != ']' && str[*pos] != '\0') {
            tData elem = parseElement(str, pos);
            if (elem) {
                append(&(listNode->data), elem);
            } else {
                free_tData(listNode);
                return NULL;
            }
            while (str[*pos] == ' ' || str[*pos] == '\t' || str[*pos] == '\n') (*pos)++;
            if (str[*pos] == ',') {
                (*pos)++;
                while (str[*pos] == ' ' || str[*pos] == '\t' || str[*pos] == '\n') (*pos)++;
            } else {
                break;
            }
        }
        if (str[*pos] == ']') {
            (*pos)++;
            return listNode;
        } else {
            free_tData(listNode);
            return NULL;
        }
    } else {
        int start = *pos;
        while (str[*pos] != '\0' && str[*pos] != ',' && str[*pos] != '{' &&
               str[*pos] != '}' && str[*pos] != '[' && str[*pos] != ']' &&
               str[*pos] != ' ' && str[*pos] != '\t' && str[*pos] != '\n') {
            (*pos)++;
        }
        int len = *pos - start;
        if (len == 0) return NULL;
        char* substr = (char*)malloc(len + 1);
        if (!substr) return NULL;
        strncpy(substr, str + start, len);
        substr[len] = '\0';
        Str s = loadStr2(substr);
        free(substr);
        if (!s) return NULL;
        tData strNode = newNodeStrHard(s);
        freeStr(&s);
        return strNode;
    }
}

tData parseTree(const char* str) {
    if (!str) return NULL;
    int pos = 0;
    tData root = parseElement(str, &pos);
    if (!root) return NULL;
    while (str[pos] == ' ' || str[pos] == '\t' || str[pos] == '\n') pos++;
    if (str[pos] != '\0') {
        free_tData(root);
        return NULL;
    }
    return root;
}