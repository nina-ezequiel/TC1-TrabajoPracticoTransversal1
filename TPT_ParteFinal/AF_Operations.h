#ifndef AF_OPERATIONS_H
#define AF_OPERATIONS_H

#include "AF.h"

// Parsea una linea de texto (str) en una estructura DeltaEntry.
// Formato: "origen,simbolo,destino" o "origen,simbolo,dest1,dest2,...,dest_n".
DeltaEntry parseDeltaEntry(Str linea);

// Fusiona una nueva transicion en un arreglo dinamico de transiciones.
void addTransition(DeltaEntry** delta, int* deltaCount, DeltaEntry newEntry);

// Lee interactivamente transiciones y estados finales desde teclado.
void readTransitions(DeltaEntry** delta, int* deltaCount, tData Q_set, tData Sigma_set, State* q0, tData* F);

// Imprime el automata en la salida estandar.
void printAF(const Af af);

// Construye un AFD que acepta el lenguaje inverso del autómata dado.
// El autómata original puede ser AFD o AFND.
// Retorna un nuevo Af (AFD) o NULL en caso de error.
Af reverseAF(const Af af);

#endif
