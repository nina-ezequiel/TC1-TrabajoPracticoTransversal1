#ifndef AF_H
#define AF_H

#include "Tree.h"

typedef tData State;   // Estado = STR (ej: "q0")
typedef tData Symbol;  // Simbolo = STR de un caracter (ej: "0")

// Entrada de la funcion de transicion d
typedef struct {
	State from;           // estado origen
	Symbol symbol;        // simbolo leido
	tData destinations;   // conjunto (SET) de estados destino
} DeltaEntry;

// Automata finito (AFD o AFND)
typedef struct {
	tData Q;              // conjunto de estados (SET)
	tData Sigma;          // alfabeto (SET)
	DeltaEntry* delta;    // arreglo dinamico de transiciones
	int deltaCount;       // numero de transiciones
	State q0;             // estado inicial
	tData F;              // conjunto de estados finales (SET)
} Automata;

typedef Automata* Af;

// Creacion y destruccion
Af newEmptyAF(void);
Af createAF(void);
void freeAF(Af af);

// Consulta de los componentes de la 5-upla (solo lectura)
tData getStates(const Af af);
tData getAlphabet(const Af af);
State getInitial(const Af af);
tData getFinals(const Af af);
const DeltaEntry* getDelta(const Af af);
int getDeltaCount(const Af af);
tData getDestinations(const Af af, State from, Symbol sym);

// Consultar determinismo y aceptacion
int isDeterministic(const Af af);
int acceptString(const Af af, Str cadena);
int acceptHardcoded(const Af af, const char* cadena);
int acceptFromConsole(const Af af);

// Funciones de modificacion (encapsulamiento)
void AF_addState(Af af, State s);
void AF_addSymbol(Af af, Symbol s);
void AF_setInitial(Af af, State s);
void AF_addFinal(Af af, State s);
void AF_addTransition(Af af, State from, Symbol sym, tData dests);

#endif
