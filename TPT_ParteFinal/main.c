
#include "AF.h"
#include "AF_Operations.h"   // para reverseAF
#include "AF_Converter.h"    // para getStateName, getOrcreate_Index (si se usan)

// Prototipos de las funciones auxiliares de construcción
Af construirAFD1();
Af construirAFD2();
Af construirAFND1();
Af construirAFND2();

// Prototipos de las funciones de ejemplo originales
void ejemploAFD1();
void ejemploAFD2();
void ejemploAFND1();
void ejemploConversionPenultimaA();
void ejemploConversionAFND2();

// Nuevas funciones para los inversos
void ejemploReverseAFD1();
void ejemploReverseAFD2();
void ejemploReverseInteractivo();

int main() {
    int opcion;
    do {
        printf("\n========== MENU ==========\n");
        printf("1. Probar AFD (cantidad impar de ceros)\n");
        printf("2. Probar AFD (divisible por 3)\n");
        printf("3. Probar AFND (penultima letra 'a')\n");
        printf("4. Convertir AFND -> AFD (penultima 'a')\n");
        printf("5. Convertir AFND -> AFD (ejemplo TP)\n");
        printf("6. Inverso del AFD (impar de ceros)\n");
        printf("7. Inverso del AFD (divisible por 3)\n");
        printf("8. Crear automata interactivamente y mostrar su inverso\n");
        printf("9. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
        case 1:
            printf("\n========== AFD (impar de ceros) ==========\n");
            ejemploAFD1();
            break;
        case 2:
            printf("\n========== AFD (divisible por 3) ==========\n");
            ejemploAFD2();
            break;
        case 3:
            printf("\n========== AFND (penultima 'a') ==========\n");
            ejemploAFND1();
            break;
        case 4:
            printf("\n=== Conversion AFND -> AFD (penultima 'a') ===\n");
            ejemploConversionPenultimaA();
            break;
        case 5:
            printf("\n=== Conversion AFND -> AFD (ejemplo TP) ===\n");
            ejemploConversionAFND2();
            break;
        case 6:
            printf("\n=== Inverso del AFD (impar de ceros) ===\n");
            ejemploReverseAFD1();
            break;
        case 7:
            printf("\n=== Inverso del AFD (divisible por 3) ===\n");
            ejemploReverseAFD2();
            break;
        case 8:
            printf("\n=== Automata inverso interactivo ===\n");
            ejemploReverseInteractivo();
            break;
        case 9:
            printf("Saliendo...\n");
            break;
        default:
            printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 9);
    return 0;
}



/* ==================== FUNCIONES AUXILIARES DE CONSTRUCCIÓN ==================== */




// Construye el AFD para cantidad impar de ceros (devuelve el AF, no imprime)
Af construirAFD1() {
    Af afd = newEmptyAF();
    if (!afd) return NULL;

    State q0 = newNodeStrHard(loadStr2("q0"));
    State q1 = newNodeStrHard(loadStr2("q1"));
    Symbol sym0 = newNodeStrHard(loadStr2("0"));
    Symbol sym1 = newNodeStrHard(loadStr2("1"));

    AF_addState(afd, q0);
    AF_addState(afd, q1);
    AF_addSymbol(afd, sym0);
    AF_addSymbol(afd, sym1);
    AF_setInitial(afd, q0);
    AF_addFinal(afd, q1);

    tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
    tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));

    AF_addTransition(afd, q0, sym0, dest_q1);
    AF_addTransition(afd, q0, sym1, dest_q0);
    AF_addTransition(afd, q1, sym0, dest_q0);
    AF_addTransition(afd, q1, sym1, dest_q1);

    free_tData(dest_q0);
    free_tData(dest_q1);
    free_tData(q0); free_tData(q1); free_tData(sym0); free_tData(sym1);

    return afd;
}

// Construye el AFD para números binarios divisibles por 3 (devuelve el AF)
Af construirAFD2() {
    Af afd = newEmptyAF();
    if (!afd) return NULL;

    State q0 = newNodeStrHard(loadStr2("q0"));
    State q1 = newNodeStrHard(loadStr2("q1"));
    State q2 = newNodeStrHard(loadStr2("q2"));
    Symbol sym0 = newNodeStrHard(loadStr2("0"));
    Symbol sym1 = newNodeStrHard(loadStr2("1"));

    AF_addState(afd, q0);
    AF_addState(afd, q1);
    AF_addState(afd, q2);
    AF_addSymbol(afd, sym0);
    AF_addSymbol(afd, sym1);
    AF_setInitial(afd, q0);
    AF_addFinal(afd, q0);   // q0 es final

    tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
    tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
    tData dest_q2 = newEmptyNodeSet(); tData_addToSet(dest_q2, copy_tData(q2));

    AF_addTransition(afd, q0, sym0, dest_q0);
    AF_addTransition(afd, q0, sym1, dest_q1);
    AF_addTransition(afd, q1, sym0, dest_q2);
    AF_addTransition(afd, q1, sym1, dest_q0);
    AF_addTransition(afd, q2, sym0, dest_q1);
    AF_addTransition(afd, q2, sym1, dest_q2);

    free_tData(dest_q0);
    free_tData(dest_q1);
    free_tData(dest_q2);
    free_tData(q0); free_tData(q1); free_tData(q2); free_tData(sym0); free_tData(sym1);

    return afd;
}

// Construye el AFND para penúltima letra 'a' (devuelve el AF)
Af construirAFND1() {
    Af afnd = newEmptyAF();
    if (!afnd) return NULL;

    State q0 = newNodeStrHard(loadStr2("q0"));
    State q1 = newNodeStrHard(loadStr2("q1"));
    State q2 = newNodeStrHard(loadStr2("q2"));
    Symbol sym_a = newNodeStrHard(loadStr2("a"));
    Symbol sym_b = newNodeStrHard(loadStr2("b"));

    AF_addState(afnd, q0);
    AF_addState(afnd, q1);
    AF_addState(afnd, q2);
    AF_addSymbol(afnd, sym_a);
    AF_addSymbol(afnd, sym_b);
    AF_setInitial(afnd, q0);
    AF_addFinal(afnd, q2);

    tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
    tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
    tData dest_q2 = newEmptyNodeSet(); tData_addToSet(dest_q2, copy_tData(q2));
    tData dest_q0_q1 = newEmptyNodeSet();
    tData_addToSet(dest_q0_q1, copy_tData(q0));
    tData_addToSet(dest_q0_q1, copy_tData(q1));

    AF_addTransition(afnd, q0, sym_a, dest_q0_q1);
    AF_addTransition(afnd, q0, sym_b, dest_q0);
    AF_addTransition(afnd, q1, sym_a, dest_q2);
    AF_addTransition(afnd, q1, sym_b, dest_q2);
    AF_addTransition(afnd, q2, sym_a, dest_q0);
    AF_addTransition(afnd, q2, sym_b, dest_q0);

    free_tData(dest_q0); free_tData(dest_q1); free_tData(dest_q2); free_tData(dest_q0_q1);
    free_tData(q0); free_tData(q1); free_tData(q2); free_tData(sym_a); free_tData(sym_b);

    return afnd;
}

// Construye el AFND de 6 estados (ejemplo TP) (devuelve el AF)
Af construirAFND2() {
    Af afnd = newEmptyAF();
    if (!afnd) return NULL;

    State q0 = newNodeStrHard(loadStr2("q0"));
    State q1 = newNodeStrHard(loadStr2("q1"));
    State q2 = newNodeStrHard(loadStr2("q2"));
    State q3 = newNodeStrHard(loadStr2("q3"));
    State q4 = newNodeStrHard(loadStr2("q4"));
    State q5 = newNodeStrHard(loadStr2("q5"));
    Symbol sym0 = newNodeStrHard(loadStr2("0"));
    Symbol sym1 = newNodeStrHard(loadStr2("1"));

    AF_addState(afnd, q0);
    AF_addState(afnd, q1);
    AF_addState(afnd, q2);
    AF_addState(afnd, q3);
    AF_addState(afnd, q4);
    AF_addState(afnd, q5);
    AF_addSymbol(afnd, sym0);
    AF_addSymbol(afnd, sym1);
    AF_setInitial(afnd, q0);
    AF_addFinal(afnd, q5);

    tData set_q0 = newEmptyNodeSet(); tData_addToSet(set_q0, copy_tData(q0));
    tData set_q0_q1_q2 = newEmptyNodeSet();
    tData_addToSet(set_q0_q1_q2, copy_tData(q0));
    tData_addToSet(set_q0_q1_q2, copy_tData(q1));
    tData_addToSet(set_q0_q1_q2, copy_tData(q2));
    tData set_q2_q3 = newEmptyNodeSet();
    tData_addToSet(set_q2_q3, copy_tData(q2));
    tData_addToSet(set_q2_q3, copy_tData(q3));
    tData set_q4 = newEmptyNodeSet(); tData_addToSet(set_q4, copy_tData(q4));
    tData set_q5 = newEmptyNodeSet(); tData_addToSet(set_q5, copy_tData(q5));

    AF_addTransition(afnd, q0, sym0, set_q0);
    AF_addTransition(afnd, q0, sym1, set_q0_q1_q2);
    AF_addTransition(afnd, q1, sym0, set_q2_q3);
    AF_addTransition(afnd, q2, sym1, set_q4);
    AF_addTransition(afnd, q3, sym0, set_q5);
    AF_addTransition(afnd, q4, sym1, set_q5);
    AF_addTransition(afnd, q5, sym0, set_q5);
    AF_addTransition(afnd, q5, sym1, set_q5);

    free_tData(set_q0);
    free_tData(set_q0_q1_q2);
    free_tData(set_q2_q3);
    free_tData(set_q4);
    free_tData(set_q5);
    free_tData(q0); free_tData(q1); free_tData(q2); free_tData(q3); free_tData(q4); free_tData(q5);
    free_tData(sym0); free_tData(sym1);

    return afnd;
}



/* ==================== FUNCIONES DE EJEMPLO ORIGINALES ==================== */



void ejemploAFD1() {
    Af afd = construirAFD1();
    if (!afd) return;
    printAF(afd);
    printf("\n--- Pruebas de aceptacion (Cantidad Impar de Ceros) ---\n");
    printf("\"\"      : %s\n", acceptHardcoded(afd, "") ? "Aceptada" : "Rechazada");
    printf("\"0\"     : %s\n", acceptHardcoded(afd, "0") ? "Aceptada" : "Rechazada");
    printf("\"00\"    : %s\n", acceptHardcoded(afd, "00") ? "Aceptada" : "Rechazada");
    printf("\"1\"     : %s\n", acceptHardcoded(afd, "1") ? "Aceptada" : "Rechazada");
    printf("\"010\"   : %s\n", acceptHardcoded(afd, "010") ? "Aceptada" : "Rechazada");
    printf("\"10100\" : %s\n", acceptHardcoded(afd, "10100") ? "Aceptada" : "Rechazada");
    freeAF(afd);
}

void ejemploAFD2() {
    Af afd = construirAFD2();
    if (!afd) return;
    printAF(afd);
    printf("\n--- Pruebas de aceptacion (Numero Binario Divisible por 3)---\n");
    printf("\"11 (3 decimal)\"      : %s\n", acceptHardcoded(afd, "11") ? "Aceptada" : "Rechazada");
    printf("\"110 (6 decimal)\"     : %s\n", acceptHardcoded(afd, "110") ? "Aceptada" : "Rechazada");
    printf("\"1001 (9 decimal)\"    : %s\n", acceptHardcoded(afd, "1001") ? "Aceptada" : "Rechazada");
    printf("\"10 (2 decimal)\"      : %s\n", acceptHardcoded(afd, "10") ? "Aceptada" : "Rechazada");
    printf("\"100 (4 decimal)\"     : %s\n", acceptHardcoded(afd, "100") ? "Aceptada" : "Rechazada");
    printf("\"111 (7 decimal)\"     : %s\n", acceptHardcoded(afd, "111") ? "Aceptada" : "Rechazada");
    freeAF(afd);
}

void ejemploAFND1() {
    Af afnd = construirAFND1();
    if (!afnd) return;
    printAF(afnd);
    printf("\n--- Pruebas AFND (penultima letra = 'a') ---\n");
    printf("\"bab\"   : %s\n", acceptHardcoded(afnd, "bab") ? "Aceptada" : "Rechazada");
    printf("\"ab\"    : %s\n", acceptHardcoded(afnd, "ab") ? "Aceptada" : "Rechazada");
    printf("\"abaa\"  : %s\n", acceptHardcoded(afnd, "abaa") ? "Aceptada" : "Rechazada");
    printf("\"abbab\" : %s\n", acceptHardcoded(afnd, "abbab") ? "Aceptada" : "Rechazada");
    printf("\"ba\"    : %s\n", acceptHardcoded(afnd, "ba") ? "Aceptada" : "Rechazada");
    printf("\"a\"     : %s\n", acceptHardcoded(afnd, "a") ? "Aceptada" : "Rechazada");
    printf("\"bb\"    : %s\n", acceptHardcoded(afnd, "bb") ? "Aceptada" : "Rechazada");
    printf("\"aba\"   : %s\n", acceptHardcoded(afnd, "aba") ? "Aceptada" : "Rechazada");
    freeAF(afnd);
}

void ejemploConversionPenultimaA() {
    Af afnd = construirAFND1();
    if (!afnd) return;
    printf("\n--- AFND original ---\n");
    printAF(afnd);
    // pruebas del AFND
    printf("\n--- Pruebas AFND (penultima letra = 'a') ---\n");
    printf("\"bab\"   : %s\n", acceptHardcoded(afnd, "bab") ? "Aceptada" : "Rechazada");
    printf("\"ab\"    : %s\n", acceptHardcoded(afnd, "ab") ? "Aceptada" : "Rechazada");
    printf("\"abaa\"  : %s\n", acceptHardcoded(afnd, "abaa") ? "Aceptada" : "Rechazada");
    printf("\"abbab\" : %s\n", acceptHardcoded(afnd, "abbab") ? "Aceptada" : "Rechazada");
    printf("\"ba\"    : %s\n", acceptHardcoded(afnd, "ba") ? "Aceptada" : "Rechazada");
    printf("\"a\"     : %s\n", acceptHardcoded(afnd, "a") ? "Aceptada" : "Rechazada");
    printf("\"bb\"    : %s\n", acceptHardcoded(afnd, "bb") ? "Aceptada" : "Rechazada");
    printf("\"aba\"   : %s\n", acceptHardcoded(afnd, "aba") ? "Aceptada" : "Rechazada");
    // conversión
    Af afd = AFNDtoAFD(afnd);
    if (afd) {
        printf("\n--- AFD convertido (renombrado) ---\n");
        printAF(afd);
        printf("\n--- Pruebas AFD (penultima letra = 'a') ---\n");
        printf("\"bab\"   : %s\n", acceptHardcoded(afd, "bab") ? "Aceptada" : "Rechazada");
        printf("\"ab\"    : %s\n", acceptHardcoded(afd, "ab") ? "Aceptada" : "Rechazada");
        printf("\"abaa\"  : %s\n", acceptHardcoded(afd, "abaa") ? "Aceptada" : "Rechazada");
        printf("\"abbab\" : %s\n", acceptHardcoded(afd, "abbab") ? "Aceptada" : "Rechazada");
        printf("\"ba\"    : %s\n", acceptHardcoded(afd, "ba") ? "Aceptada" : "Rechazada");
        printf("\"a\"     : %s\n", acceptHardcoded(afd, "a") ? "Aceptada" : "Rechazada");
        printf("\"bb\"    : %s\n", acceptHardcoded(afd, "bb") ? "Aceptada" : "Rechazada");
        printf("\"aba\"   : %s\n", acceptHardcoded(afd, "aba") ? "Aceptada" : "Rechazada");
        freeAF(afd);
    } else {
        printf("Error en la conversion.\n");
    }
    freeAF(afnd);
}

void ejemploConversionAFND2() {
    Af afnd = construirAFND2();
    if (!afnd) return;
    printAF(afnd);
    printf("\n--- Pruebas de aceptacion (ejemplo TP) ---\n");
    printf("\"111\"     : %s\n", acceptHardcoded(afnd, "111")? "Aceptada" : "Rechazada");
    printf("\"0\"       : %s\n", acceptHardcoded(afnd, "0")? "Aceptada" : "Rechazada");
    printf("\"00\"      : %s\n", acceptHardcoded(afnd, "00")? "Aceptada" : "Rechazada");
    printf("\"1011\"    : %s\n", acceptHardcoded(afnd, "1011")? "Aceptada" : "Rechazada");
    printf("\"010\"     : %s\n", acceptHardcoded(afnd, "010")? "Aceptada" : "Rechazada");
    printf("\"10100\"   : %s\n", acceptHardcoded(afnd, "10100")? "Aceptada" : "Rechazada");
    Af afd = AFNDtoAFD(afnd);
    if (afd) {
        printf("\n--- AFD convertido (renombrado) ---\n");
        printAF(afd);
        printf("\n--- Pruebas de aceptacion (ejemplo TP) ---\n");
        printf("\"111\"   : %s\n", acceptHardcoded(afd, "111") ? "Aceptada" : "Rechazada");
        printf("\"0\"     : %s\n", acceptHardcoded(afd, "0") ? "Aceptada" : "Rechazada");
        printf("\"00\"    : %s\n", acceptHardcoded(afd, "00") ? "Aceptada" : "Rechazada");
        printf("\"1011\"  : %s\n", acceptHardcoded(afd, "1011") ? "Aceptada" : "Rechazada");
        printf("\"010\"   : %s\n", acceptHardcoded(afd, "010") ? "Aceptada" : "Rechazada");
        printf("\"10100\" : %s\n", acceptHardcoded(afd, "10100") ? "Aceptada" : "Rechazada");
        freeAF(afd);
    } else {
        printf("Error en la conversion.\n");
    }
    freeAF(afnd);
}




/* ==================== NUEVAS FUNCIONES PARA LOS INVERSOS ==================== */



// Función auxiliar para invertir una cadena (str) y devolver una nueva str
static Str reverseStr(Str s) {
    if (s == NULL) return NULL;
    // Contar longitud
    int len = 0;
    Str temp = s;
    while (temp != NULL) { len++; temp = temp->next; }
    // Construir nueva cadena al revés
    Str result = createStr();
    temp = s;

    char* arr = (char*)malloc((len + 1) * sizeof(char));
    int i = 0;
    while (temp != NULL) { arr[i++] = temp->car; temp = temp->next; }
    arr[len] = '\0';
    // Invertir
    for (int j = 0; j < len/2; j++) {
        char c = arr[j];
        arr[j] = arr[len-1-j];
        arr[len-1-j] = c;
    }
    result = loadStr2(arr);
    free(arr);
    return result;
}

void ejemploReverseAFD1() {
    Af original = construirAFD1();
    if (!original) return;
    printf("\n--- AFD original (impar de ceros) ---\n");
    printAF(original);

    Af inverso = reverseAF(original);
    if (!inverso) {
        printf("Error al calcular el inverso.\n");
        freeAF(original);
        return;
    }
    printf("\n--- AFD inverso (reconoce el reverso del lenguaje) ---\n");
    printAF(inverso);

    printf("\n--- Pruebas del inverso (con los reversos de las cadenas del ejemplo) ---\n");
    // Reversos de: "", "0", "00", "1", "010", "10100"
    // → "", "0", "00", "1", "010", "00101"
    printf("\"\"      : %s\n", acceptHardcoded(inverso, "") ? "Aceptada" : "Rechazada");
    printf("\"0\"     : %s\n", acceptHardcoded(inverso, "0") ? "Aceptada" : "Rechazada");
    printf("\"00\"    : %s\n", acceptHardcoded(inverso, "00") ? "Aceptada" : "Rechazada");
    printf("\"1\"     : %s\n", acceptHardcoded(inverso, "1") ? "Aceptada" : "Rechazada");
    printf("\"010\"   : %s\n", acceptHardcoded(inverso, "010") ? "Aceptada" : "Rechazada");
    printf("\"00101\" : %s\n", acceptHardcoded(inverso, "00101") ? "Aceptada" : "Rechazada");

    freeAF(inverso);
    freeAF(original);
}

void ejemploReverseAFD2() {
    Af original = construirAFD2();
    if (!original) return;
    printf("\n--- AFD original (divisible por 3) ---\n");
    printAF(original);

    Af inverso = reverseAF(original);
    if (!inverso) {
        printf("Error al calcular el inverso.\n");
        freeAF(original);
        return;
    }
    printf("\n--- AFD inverso (reconoce el reverso del lenguaje) ---\n");
    printAF(inverso);

    printf("\n--- Pruebas del inverso (con los reversos de las cadenas del ejemplo) ---\n");
    // Reversos de: "11", "110", "1001", "10", "100", "111"
    // → "11", "011", "1001", "01", "001", "111"
    printf("\"11\"   : %s\n", acceptHardcoded(inverso, "11") ? "Aceptada" : "Rechazada");
    printf("\"011\"  : %s\n", acceptHardcoded(inverso, "011") ? "Aceptada" : "Rechazada");
    printf("\"1001\" : %s\n", acceptHardcoded(inverso, "1001") ? "Aceptada" : "Rechazada");
    printf("\"01\"   : %s\n", acceptHardcoded(inverso, "01") ? "Aceptada" : "Rechazada");
    printf("\"001\"  : %s\n", acceptHardcoded(inverso, "001") ? "Aceptada" : "Rechazada");
    printf("\"111\"  : %s\n", acceptHardcoded(inverso, "111") ? "Aceptada" : "Rechazada");

    freeAF(inverso);
    freeAF(original);
}

void ejemploReverseInteractivo() {
    Af original = createAF();
    if (!original) {
        printf("No se pudo crear el automata.\n");
        return;
    }
    printf("\n--- Automata original ---\n");
    printAF(original);

    Af inverso = reverseAF(original);
    if (!inverso) {
        printf("Error al calcular el inverso.\n");
        freeAF(original);
        return;
    }
    printf("\n--- Automata inverso (AFD) ---\n");
    printAF(inverso);

    printf("\n--- Prueba de aceptacion en el inverso ---\n");
    printf("Ingrese una cadena para evaluar (o Enter para saltar): ");
    Str s = loadStr();
    if (s != NULL) {
        int res = acceptString(inverso, s);
        printf("Cadena ");
        printStr(s);
        printf(" %s\n", res ? "ACEPTADA" : "RECHAZADA");
        freeStr(&s);
    }
    freeAF(inverso);
    freeAF(original);
}



/* 

gcc String.c Tree.c AF.c AF_Operations.c AF_Converter.c main.c -o mi_programa 
.\mi_programa.exe

*/