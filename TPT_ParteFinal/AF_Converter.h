#ifndef AF_CONVERTER_H
#define AF_CONVERTER_H

#include "AF.h"

// Convierte un AFND en un AFD equivalente.
// Los estados del AFD se renombran como p0, p1, p2, ...
// Retorna NULL si hay error.
Af AFNDtoAFD(const Af afnd);

// Calcula el conjunto de estados alcanzables desde un conjunto dado con un s�mbolo.
// Retorna un nuevo conjunto (tData) o NULL si no hay transiciones.
tData computeStates(const Af af, tData currentStates, Symbol sym);

int obtenerOCrearIndice(Af afd, tData destSet, tData** subConjuntos, tData** estadosAFD, int* numConjuntos);

Str getStateName(int id);

#endif
