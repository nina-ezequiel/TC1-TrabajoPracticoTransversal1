#include "AF_Operations.h"
#include "AF_Converter.h"

/* -------------------- parseDeltaEntry ------------------- */
DeltaEntry parseDeltaEntry(Str s) {
	DeltaEntry entry = {NULL, NULL, NULL};
	if (s == NULL) return entry;
	
	// Origen (antes de la 1a coma)
	Str fromStr = beforeToken(s, ',');
	if (fromStr == NULL) 
		return entry;
	entry.from = newNodeStrHard(fromStr);
	freeStr(&fromStr);
	
	// Simbolo (entre 1er y 2da coma)
	Str afterFirst = afterToken(s, ',');
	if (afterFirst == NULL) {
		free_tData(entry.from);
		return entry;
	}
	Str symStr = beforeToken(afterFirst, ',');
	if (symStr == NULL) {
		free_tData(entry.from);
		freeStr(&afterFirst);
		return entry;
	}
	entry.symbol = newNodeStrHard(symStr);
	freeStr(&symStr);
	
	// Destinos (despues de la 2da coma)
	Str destPart = afterToken(afterFirst, ',');
	freeStr(&afterFirst);
	if (destPart == NULL) {
		free_tData(entry.from);
		free_tData(entry.symbol);
		return entry;
	}
	entry.destinations = strToSetToken(destPart, ',');
	freeStr(&destPart);
	return entry;
}

/* -------------------- addTransition ------------------- */
void addTransition(DeltaEntry** delta, int* deltaCount, DeltaEntry newEntry) {
	// Buscar si ya existe una transicion con el mismo origen y simbolo
	for (int i = 0; i < *deltaCount; i++) {
		if (equal_tData((*delta)[i].from, newEntry.from) && equal_tData((*delta)[i].symbol, newEntry.symbol)) {
			// Fusionar destinos
			tData destExistentes = (*delta)[i].destinations;
			tData destNode = tData_getFirst(newEntry.destinations);
			while (destNode) {
				tData destCopia = copy_tData(destNode);
				tData_addToSet(destExistentes, destCopia);
				destNode = tData_getNext(destNode);
			}
			free_tData(newEntry.from);
			free_tData(newEntry.symbol);
			free_tData(newEntry.destinations);
			return; // No se incrementa deltaCount
		}
	}
	// No existe: agregar nueva entrada al arreglo
	*delta = (DeltaEntry*)realloc(*delta, (*deltaCount + 1) * sizeof(DeltaEntry));
	(*delta)[(*deltaCount)++] = newEntry;
}

/* -------------------- readTransitions ------------------- */
void readTransitions(DeltaEntry** delta, int* deltaCount, tData Q_set, tData Sigma_set, State* q0, tData* F) {
	int first = 1;
	Str finStr = loadStr2("fin");
	while (1) {
		
		printf("d(estado,simbolo) = ");
		Str s = loadStr();
		if (equalStr(s, finStr)) {
			freeStr(&s);
			break;
		}
		DeltaEntry entry = parseDeltaEntry(s);
		freeStr(&s);
		if (!entry.from || !entry.symbol || !entry.destinations) {
			printf("Formato invalido. Intente de nuevo.\n");
			if (entry.from) free_tData(entry.from);
			if (entry.symbol) free_tData(entry.symbol);
			if (entry.destinations) free_tData(entry.destinations);
			continue;
		}
		// Agregar 'from' a Q_set
		tData_addToSet(Q_set, copy_tData(entry.from));
		// Agregar todos los destinos a Q_set
		tData destNode = tData_getFirst(entry.destinations);
		while (destNode) {
			tData_addToSet(Q_set, copy_tData(destNode));
			destNode = tData_getNext(destNode);
		}
		// Agregar simbolo a Sigma_set
		tData_addToSet(Sigma_set, copy_tData(entry.symbol));
		// Definir estado inicial (primera transicion)
		if (first) {
			*q0 = copy_tData(entry.from);
			first = 0;
		}
		// Guardar transicion
		addTransition(delta, deltaCount, entry);
	}
	freeStr(&finStr);
	if (*deltaCount == 0) {
		*F = NULL;
		return;
	}
	// Leer estados finales
	while (1) {
		printf("\nAhora ingrese el/los estado/s final/es (separados por comas): ");
		Str fStr = loadStr();
		tData Fset = strToSetToken(fStr, ',');
		freeStr(&fStr);
		if (inclusionSet(Fset, Q_set)) {
			*F = Fset;
			break;
		} 
		else {
			printf("\nLos estados ingresados no estan incluidos en Q. Vuelva a intentar: ");
			free_tData(Fset);
		}
	}
}
 
/* -------------------- printAF ------------------- */
void printAF(const Af af) {
	printf("\n=== AUTOMATA ===\n");
	printf("Q = "); printData(getStates(af));
	printf("\nSigma = "); printData(getAlphabet(af));
	printf("\nq0 = "); printData(getInitial(af));
	printf("\nF = "); printData(getFinals(af));
	printf("\ndelta:\n");
	for (int i = 0; i < getDeltaCount(af); i++) {
		const DeltaEntry* d = &getDelta(af)[i];
		printf("  d(");
		printData(d->from);
		printf(", ");
		printData(d->symbol);
		printf(") = ");
		printData(d->destinations);
		printf("\n");
	}
	printf("\nNumero de Transiciones %d: ", af->deltaCount);
	printf("\nEs Determinista?: %s\n", isDeterministic(af) ? "Si" : "No");
}

/* ---------------------------------------------------------------------
 * Funciones auxiliares para reverseAF
 * --------------------------------------------------------------------- */

// Calcula el conjunto de predecesores de un conjunto de estados dado,
// a través de un símbolo determinado, en el autómata 'af'.
// Retorna un nuevo conjunto (tData) de estados (STR) o NULL si no hay predecesores.
static tData predecessorStates(const Af af, tData stateSet, Symbol sym) {
    if (af == NULL || stateSet == NULL || sym == NULL)
        return NULL;

    tData result = newEmptyNodeSet();
    if (result == NULL)
        return NULL;

    // Recorremos todos los estados del autómata original
    tData stateNode = tData_getFirst(getStates(af));
    while (stateNode != NULL) {
        tData dests = getDestinations(af, stateNode, sym);
        if (dests != NULL) {
            // Verificar si dests intersecta con stateSet
            tData inter = intersectionSet(dests, stateSet);
            if (inter != NULL && tData_getFirst(inter) != NULL) {
                // stateNode es un predecesor válido
                tData_addToSet(result, copy_tData(stateNode));
            }
            free_tData(inter);
        }
        stateNode = tData_getNext(stateNode);
    }

    if (tData_getFirst(result) == NULL) {
        free_tData(result);
        return NULL;
    }
    return result;
}

/* ---------------------------------------------------------------------
 * reverseAF: construye el AFD que acepta el lenguaje inverso
 * --------------------------------------------------------------------- */
Af reverseAF(const Af af) {
    if (af == NULL) return NULL;

    tData F = getFinals(af);
    if (F == NULL || tData_getFirst(F) == NULL) {
        Af emptyAfd = newEmptyAF();
        if (!emptyAfd) return NULL;
        tData sigmaNode = tData_getFirst(getAlphabet(af));
        while (sigmaNode) {
            AF_addSymbol(emptyAfd, sigmaNode);
            sigmaNode = tData_getNext(sigmaNode);
        }
        Str initName = loadStr2("q0");
        tData initState = newNodeStrHard(initName);
        freeStr(&initName);
        AF_addState(emptyAfd, initState);
        AF_setInitial(emptyAfd, initState);
        return emptyAfd;
    }

    Af afd = newEmptyAF();
    if (!afd) return NULL;
    tData sigmaNode = tData_getFirst(getAlphabet(af));
    while (sigmaNode) {
        AF_addSymbol(afd, sigmaNode);
        sigmaNode = tData_getNext(sigmaNode);
    }

    tData* sets = NULL;
    tData* stateNames = NULL;
    int setCount = 0;

    tData initialSet = copy_tData(F);
    int initIdx = getOrcreate_Index(afd, initialSet, &sets, &stateNames, &setCount);
    AF_setInitial(afd, stateNames[initIdx]);

    for (int i = 0; i < setCount; i++) {
        tData currentSet = sets[i];
        tData currentState = stateNames[i];

        tData symNode = tData_getFirst(getAlphabet(af));
        while (symNode) {
            tData predSet = predecessorStates(af, currentSet, symNode);
            if (predSet != NULL) {
                int destIdx = getOrcreate_Index(afd, predSet, &sets, &stateNames, &setCount);
                tData destSet = newEmptyNodeSet();
                tData_addToSet(destSet, copy_tData(stateNames[destIdx]));
                AF_addTransition(afd, currentState, symNode, destSet);
                free_tData(destSet);
            }
            symNode = tData_getNext(symNode);
        }
    }

    tData q0Original = getInitial(af);
    if (q0Original != NULL) {
        for (int i = 0; i < setCount; i++) {
            if (pertainSet(q0Original, sets[i])) {
                AF_addFinal(afd, stateNames[i]);
                break;
            }
        }
    }

    free(sets);
    free(stateNames);
    return afd;
}
