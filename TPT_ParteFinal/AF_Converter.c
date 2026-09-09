#include "AF_Converter.h"
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------
* Crea un AFD vacio y copia el alfabeto del AFND.
* ------------------------------------------------------------ */
static Af newEmptyAFDwithAlf(const Af afnd) {
	Af afd = newEmptyAF();
	if (!afd) return NULL;
	tData sigmaNode = tData_getFirst(getAlphabet(afnd));
	while (sigmaNode) {
		AF_addSymbol(afd, sigmaNode);
		sigmaNode = tData_getNext(sigmaNode);
	}
	return afd;
}

/* ------------------------------------------------------------
* Genera el nombre de un estado del AFD a partir de su ID (p0, p1, ...).
* ------------------------------------------------------------ */
Str getStateName(int id) {
	char buf[10];
	sprintf(buf, "p%d", id);
	return loadStr2(buf);
}

/* ------------------------------------------------------------
* Inicializa los arreglos subConjuntos y estadosAFD con el subconjunto inicial {q0_nd}.
* ------------------------------------------------------------ */
static void initializeAuxArr(Af afd, const Af afnd, tData** subConjuntos, tData** estadosAFD, int* numConjuntos) {
	tData startSet = newEmptyNodeSet();
	tData_addToSet(startSet, copy_tData(getInitial(afnd)));
	*subConjuntos = (tData*)realloc(*subConjuntos, sizeof(tData));
	(*subConjuntos)[0] = startSet;
	*numConjuntos = 1;
	
	Str name0 = getStateName(0);
	tData state0 = newNodeStrHard(name0);
	freeStr(&name0);
	AF_addState(afd, state0);
	AF_setInitial(afd, state0);
	*estadosAFD = (tData*)realloc(*estadosAFD, sizeof(tData));
	(*estadosAFD)[0] = state0;
}

/* ============================================================
* FUNCION PUBLICA: reachableStates
* ============================================================ */
tData reachableStates(const Af af, tData currentStates, Symbol sym) {
	tData possibleStates = newEmptyNodeSet();
	tData stateNode = tData_getFirst(currentStates);
	while (stateNode) {
		tData dests = getDestinations(af, stateNode, sym);
		if (dests != NULL) {
			tData dnode = tData_getFirst(dests);
			while (dnode) {
				tData_addToSet(possibleStates, copy_tData(dnode));
				dnode = tData_getNext(dnode);
			}
		}
		stateNode = tData_getNext(stateNode);
	}
	if (tData_getFirst(possibleStates) == NULL) {
		free_tData(possibleStates);
		return NULL;
	}
	return possibleStates;
}

/* ------------------------------------------------------------
* Busca o aniade un destSet en los arreglos.
* ------------------------------------------------------------ */
int getOrcreate_Index(Af afd, tData destSet, tData** subConjuntos, tData** estadosAFD, int* numConjuntos) {
	for (int k = 0; k < *numConjuntos; k++) {
		if (equal_tData((*subConjuntos)[k], destSet))
			return k;
	}
	int idx = *numConjuntos;
	*subConjuntos = (tData*)realloc(*subConjuntos, (idx + 1) * sizeof(tData));
	(*subConjuntos)[idx] = destSet;
	(*numConjuntos)++;
	
	Str newName = getStateName(idx);
	tData newState = newNodeStrHard(newName);
	freeStr(&newName);
	AF_addState(afd, newState);
	*estadosAFD = (tData*)realloc(*estadosAFD, *numConjuntos * sizeof(tData));
	(*estadosAFD)[idx] = newState;
	
	return idx;
}

/* ------------------------------------------------------------
* Agrega una transicion al AFD desde currentState con simbolo sigmaNode
* hacia el estado con indice destIdx.
* ------------------------------------------------------------ */
static void addTransitionAFD(Af afd, tData currentState, tData sigmaNode, int destIdx, tData* estadosAFD) {
	tData unit = newEmptyNodeSet();
	tData_addToSet(unit, copy_tData(estadosAFD[destIdx]));
	AF_addTransition(afd, currentState, sigmaNode, unit);
	free_tData(unit);
}

/* ------------------------------------------------------------
* Marca como finales los estados del AFD cuyo subconjunto asociado
* contiene al menos un estado final del AFND.
* Se usa intersectionSet para comprobar si la interseccion no es vacia.
* ------------------------------------------------------------ */
static void dialFinalsAFD(Af afd, const Af afnd, tData* subConjuntos, tData* estadosAFD, int numConjuntos) {
	for (int i = 0; i < numConjuntos; i++) {
		tData inter = intersectionSet(subConjuntos[i], getFinals(afnd));
		if (inter != NULL && tData_getFirst(inter) != NULL) {
			AF_addFinal(afd, estadosAFD[i]);
		}
		free_tData(inter);  // Liberamos siempre, intersectionSet puede devolver NULL o un conjunto
	}
}

/* ------------------------------------------------------------
* Libera la memoria de los arreglos auxiliares.
* ------------------------------------------------------------ */
static void freeArr(tData* subConjuntos, tData* estadosAFD, int numConjuntos) {
	for (int i = 0; i < numConjuntos; i++) {
		free_tData(subConjuntos[i]);
		free_tData(estadosAFD[i]);
	}
	free(subConjuntos);
	free(estadosAFD);
}

/* ============================================================
* FUNCION PRINCIPAL: AFNDtoAFD
* ============================================================ */

Af AFNDtoAFD(const Af afnd) {
	if (afnd == NULL) return NULL;
	
	Af afd = newEmptyAFDwithAlf(afnd);
	if (!afd) return NULL;
	
	tData* subConjuntos = NULL;
	tData* estadosAFD = NULL;
	int numConjuntos = 0;
	initializeAuxArr(afd, afnd, &subConjuntos, &estadosAFD, &numConjuntos);
	
	for (int i = 0; i < numConjuntos; i++) {
		tData currentSet = subConjuntos[i];
		tData currentState = estadosAFD[i];
		tData sigmaNode = tData_getFirst(getAlphabet(afnd));
		while (sigmaNode) {
			tData destSet = reachableStates(afnd, currentSet, sigmaNode);
			if (destSet != NULL) {
				int idx = getOrcreate_Index(afd, destSet, &subConjuntos, &estadosAFD, &numConjuntos);
				addTransitionAFD(afd, currentState, sigmaNode, idx, estadosAFD);
			}
			sigmaNode = tData_getNext(sigmaNode);
		}
	}	
	dialFinalsAFD(afd, afnd, subConjuntos, estadosAFD, numConjuntos);
	freeArr(subConjuntos, estadosAFD, numConjuntos);

	return afd;
}



