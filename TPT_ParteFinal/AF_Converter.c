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
* Inicializa los arreglos subSets y statesAFD con el subconjunto inicial {q0_nd}.
* ------------------------------------------------------------ */
static void initializeAuxArr(Af afd, const Af afnd, tData** subSets, tData** statesAFD, int* numConjuntos) {
	tData startSet = newEmptyNodeSet();
	tData_addToSet(startSet, copy_tData(getInitial(afnd)));
	*subSets = (tData*)realloc(*subSets, sizeof(tData));
	(*subSets)[0] = startSet;
	*numConjuntos = 1;
	
	Str name0 = getStateName(0);
	tData state0 = newNodeStrHard(name0);
	freeStr(&name0);
	AF_addState(afd, state0);
	AF_setInitial(afd, state0);
	*statesAFD = (tData*)realloc(*statesAFD, sizeof(tData));
	(*statesAFD)[0] = state0;
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
int getOrcreate_Index(Af afd, tData destSet, tData** subSets, tData** statesAFD, int* numConjuntos) {
	for (int k = 0; k < *numConjuntos; k++) {
		if (equal_tData((*subSets)[k], destSet))
			return k;
	}
	int idx = *numConjuntos;
	*subSets = (tData*)realloc(*subSets, (idx + 1) * sizeof(tData));
	(*subSets)[idx] = destSet;
	(*numConjuntos)++;
	
	Str newName = getStateName(idx);
	tData newState = newNodeStrHard(newName);
	freeStr(&newName);
	AF_addState(afd, newState);
	*statesAFD = (tData*)realloc(*statesAFD, *numConjuntos * sizeof(tData));
	(*statesAFD)[idx] = newState;
	
	return idx;
}

/* ------------------------------------------------------------
* Agrega una transicion al AFD desde currentState con simbolo sigmaNode
* hacia el estado con indice destIdx.
* ------------------------------------------------------------ */
static void addTransitionAFD(Af afd, tData currentState, tData sigmaNode, int destIdx, tData* statesAFD) {
	tData unit = newEmptyNodeSet();
	tData_addToSet(unit, copy_tData(statesAFD[destIdx]));
	AF_addTransition(afd, currentState, sigmaNode, unit);
	free_tData(unit);
}

/* ------------------------------------------------------------
* Marca como finales los estados del AFD cuyo subconjunto asociado
* contiene al menos un estado final del AFND.
* Se usa intersectionSet para comprobar si la interseccion no es vacia.
* ------------------------------------------------------------ */
static void dialFinalsAFD(Af afd, const Af afnd, tData* subSets, tData* statesAFD, int numConjuntos) {
	for (int i = 0; i < numConjuntos; i++) {
		tData inter = intersectionSet(subSets[i], getFinals(afnd));
		if (inter != NULL && tData_getFirst(inter) != NULL) {
			AF_addFinal(afd, statesAFD[i]);
		}
		free_tData(inter);  // Liberamos siempre, intersectionSet puede devolver NULL o un conjunto
	}
}

/* ------------------------------------------------------------
* Libera la memoria de los arreglos auxiliares.
* ------------------------------------------------------------ */
static void freeArr(tData* subSets, tData* statesAFD, int numConjuntos) {
	for (int i = 0; i < numConjuntos; i++) {
		free_tData(subSets[i]);
		free_tData(statesAFD[i]);
	}
	free(subSets);
	free(statesAFD);
}

/* ============================================================
* FUNCION PRINCIPAL: AFNDtoAFD
* ============================================================ */

Af AFNDtoAFD(const Af afnd) {
	if (afnd == NULL) return NULL;
	
	Af afd = newEmptyAFDwithAlf(afnd);
	if (!afd) return NULL;
	
	tData* subSets = NULL;
	tData* statesAFD = NULL;
	int numConjuntos = 0;
	initializeAuxArr(afd, afnd, &subSets, &statesAFD, &numConjuntos);
	
	for (int i = 0; i < numConjuntos; i++) {
		tData currentSet = subSets[i];
		tData currentState = statesAFD[i];
		tData sigmaNode = tData_getFirst(getAlphabet(afnd));
		while (sigmaNode) {
			tData destSet = reachableStates(afnd, currentSet, sigmaNode);
			if (destSet != NULL) {
				int idx = getOrcreate_Index(afd, destSet, &subSets, &statesAFD, &numConjuntos);
				addTransitionAFD(afd, currentState, sigmaNode, idx, statesAFD);
			}
			sigmaNode = tData_getNext(sigmaNode);
		}
	}	
	dialFinalsAFD(afd, afnd, subSets, statesAFD, numConjuntos);
	freeArr(subSets, statesAFD, numConjuntos);

	return afd;
}



