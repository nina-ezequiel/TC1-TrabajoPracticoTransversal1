#include "AF.h"
#include "AF_Operations.h"
#include "AF_Converter.h"

/* -------------------- Creacion y destruccion ------------------- */

Af newEmptyAF(void) {
	Af af = (Af)malloc(sizeof(Automata));
	if (!af) return NULL;
	af->Q = newEmptyNodeSet();
	af->Sigma = newEmptyNodeSet();
	af->delta = NULL;
	af->deltaCount = 0;
	af->q0 = NULL;
	af->F = newEmptyNodeSet();
	return af;
}

void freeAF(Af af) {
	if (!af) return;
	free_tData(af->Q);
	free_tData(af->Sigma);
	for (int i = 0; i < af->deltaCount; i++) {
		free_tData(af->delta[i].from);
		free_tData(af->delta[i].symbol);
		free_tData(af->delta[i].destinations);
	}
	free(af->delta);
	free_tData(af->q0);
	free_tData(af->F);
	free(af);
}

/* -------------------- Getters ------------------- */
tData getStates(const Af af) { return af->Q; }
tData getAlphabet(const Af af) { return af->Sigma; }
State getInitial(const Af af) { return af->q0; }
tData getFinals(const Af af) { return af->F; }
const DeltaEntry* getDelta(const Af af) { return af->delta; }
int getDeltaCount(const Af af) { return af->deltaCount; }

/* -------------------- Funciones de modificacion ------------------- */
void AF_addState(Af af, State s) {
	if (!af || !s) return;
	tData_addToSet(af->Q, copy_tData(s));
}

void AF_addSymbol(Af af, Symbol s) {
	if (!af || !s) return;
	tData_addToSet(af->Sigma, copy_tData(s));
}

void AF_setInitial(Af af, State s) {
	if (!af || !s) return;
	if (af->q0) free_tData(af->q0);
	af->q0 = copy_tData(s);
}

void AF_addFinal(Af af, State s) {
	if (!af || !s) return;
	tData_addToSet(af->F, copy_tData(s));
}

void AF_addTransition(Af af, State from, Symbol sym, tData dests) {
	if (!af || !from || !sym || !dests) return;
	DeltaEntry newEntry;
	newEntry.from = copy_tData(from);
	newEntry.symbol = copy_tData(sym);
	newEntry.destinations = copy_tData(dests);
	addTransition(&af->delta, &af->deltaCount, newEntry);
}

/* -------------------- Creacion De AF desde la Consola ------------------- */
Af createAF(void) {
	Af af = newEmptyAF();
	if (!af) return NULL;
	
	printf("\n=== Creacion desde Consola del Automata Finito ===\n\n");
	printf("Ingrese las transiciones en el formato:\n\t origen,simbolo,destino\n\n");
	printf("Para multiples destinos ingrese las transiciones en el formato:\n\t origen,simbolo,dest1,dest2,...,dest_n\n\n");
	printf("Escriba 'fin' para terminar.\n");
	printf("La primera transicion determinara el estado inicial.\n\n");
	
	tData Q_set = newEmptyNodeSet();
	tData Sigma_set = newEmptyNodeSet();
	DeltaEntry* delta = NULL;
	int deltaCount = 0;
	State q0 = NULL;
	tData F = NULL;
	
	readTransitions(&delta, &deltaCount, Q_set, Sigma_set, &q0, &F);
	
	if (deltaCount == 0) {
		printf("No se ingresaron transiciones. Automata vacio.\n");
		free_tData(Q_set);
		free_tData(Sigma_set);
		freeAF(af);
		return NULL;
	}
	
	af->Q = Q_set;
	af->Sigma = Sigma_set;
	af->delta = delta;
	af->deltaCount = deltaCount;
	af->q0 = q0;
	af->F = F;
	
	return af;
}

/* -------------------- Determinismo ------------------- */
int isDeterministic(const Af af) {
	if (!af) 
		return 0;
	for (int i = 0; i < af->deltaCount; i++) {
		if (cardinalitySet(af->delta[i].destinations) != 1) 
			return 0;
	}
	return 1;
}

/* -------------------- Aceptacion de cadenas ------------------- */
tData getDestinations(const Af af, State from, Symbol sym) {
	for (int i = 0; i < af->deltaCount; i++) {
		if (equal_tData(af->delta[i].from, from) && equal_tData(af->delta[i].symbol, sym))
			return af->delta[i].destinations;
	}
	return NULL;
}

static int acceptRecursive(const Af af, tData currentStates, Str string) {
	if (string == NULL) {
		tData inter = intersectionSet(currentStates, getFinals(af));
        int result = (inter != NULL && tData_getFirst(inter) != NULL);
        free_tData(inter);
        return result;
	}
	char symBuf[2] = {str_getFirst(string), '\0'};
	Symbol sym = newNodeStrHard(loadStr2(symBuf));
	tData possibleStates = computeStates(af, currentStates, sym);
	free_tData(sym);
	int result = acceptRecursive(af, possibleStates, string->next);
	free_tData(possibleStates);
	return result;
}

int acceptString(const Af af, Str cadena) {
	if (!af || !cadena) 
		return 0;
	tData startState = newEmptyNodeSet();
	tData_addToSet(startState, copy_tData(af->q0));
	int accepted = acceptRecursive(af, startState, cadena);
	free_tData(startState);
	return accepted;
}

int acceptHardcoded(const Af af, const char* cadena) {
	Str s = loadStr2(cadena);
	int res = acceptString(af, s);
	freeStr(&s);
	return res;
}

int acceptFromConsole(const Af af) {
	printf("\nIngrese la cadena a evaluar: ");
	Str s = loadStr();
	int res = acceptString(af, s);
	freeStr(&s);
	return res;
}
