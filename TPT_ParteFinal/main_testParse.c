#include "Tree.h"
#include <stdio.h>

int main() {
    const char* input = "{{q0,q1,q2,q3},{a,b},[[q0,a,q1,q2],[q0,b,q2],[q1,a,q0,q1,q2],[q1,b,q2]],q0,{q2,q3}}";
    /*
    tData root = parseTree(input);
    if (root) {
        printf("Tipo raiz: %d\n", root->nodeType);
        printf("Cardinalidad: %d\n", cardinalitySet(root));
        printf("Arbol:\n");
        printData(root);
        printf("\n");
        free_tData(root);
    } else {
        printf("Error al parsear.\n");
    }
    */
   tData root = parseTree(input);
    if (root) {
        printf("Tipo raíz: %d\n", root->nodeType);
        printf("Cardinalidad: %d\n", cardinalitySet(root));
        // Inspeccionar manualmente el primer elemento
        if (root->data != NULL) {
            printf("Primer elemento: ");
            printData(tData_getFirst(root));
            printf("\n");
        } else {
            printf("root->data es NULL\n");
        }
        printf("\n");
        printData(root);
        printf("\n");
        free_tData(root);
    }
    return 0;
}