#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../TDA/TDA2/TDAVector/Vector.h"
#include "../../TDA/TDAFecha/Fecha.h"

typedef struct {
    char periodo[11];
    char nivel_general_aperturas[41];
    double indice_icc;
    char Clasificador[14];
    double var_mensual, var_interanual;
} Fila;

void calcularVarMensual(void* elem, void* datos);
void calcularVarInteranual(void* elem, void* datos);
int compararPorFecha(const void* a, const void* b);
double calcularVarPorc(double indActual, double indPrevio);

int main()
{
    Vector vec;
    vectorCrear(&vec, sizeof(Fila));
    vectorRecorrer(&vec, calcularVarMensual, &vec);
    vectorRecorrer(&vec, calcularVarInteranual, &vec);

    printf("Hello world!\n");
    return 0;
}

/**
    Pasar NULL a filaPrev significa que es el primer mes y no debería ser calculado.
    La función que llame deberá comprobar si es una fecha válida.
*/
void calcularVarMensual(void* elem, void* datos) {
    Fecha fechaAct, fechaPrev;
    Fila* fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    fechaDeCadena(&fechaAct, fila->periodo);
    fechaPrev = fechaRestarMeses(&fechaAct, 1);
    fechaACadena(&fechaPrev, filaPrev.periodo);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != 0) {
        double porcentaje = calcularVarPorc(fila->indice_icc, filaPrev.indice_icc);
        fila->var_mensual = floor(porcentaje * 100) / 100;
    }
}

/**
    Pasar NULL a filaPrev significa que no paso un año y no debería ser calculado.
    La función que llame deberá comprobar si es una fecha válida.
*/
void calcularVarInteranual(void* elem, void* datos) {
    Fecha fechaAct, fechaPrev;
    Fila* fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    fechaDeCadena(&fechaAct, fila->periodo);
    fechaPrev = fechaRestarMeses(&fechaAct, 12);
    fechaACadena(&fechaPrev, filaPrev.periodo);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != 0) {
        double porcentaje = calcularVarPorc(fila->indice_icc, filaPrev.indice_icc);
        fila->var_mensual = floor(porcentaje * 100) / 100;
    }
}

int compararPorFecha(const void* a, const void* b) {
    const Fila* filaA = a;
    const Fila* filaB = b;

    Fecha fAct, fPrev;
    fechaDeCadena(&fAct, filaA->periodo);
    fechaDeCadena(&fPrev, filaB->periodo);

    int cmp = fechaComparar(&fAct, &fPrev);
    if (cmp != 0) {
        return cmp;
    }

    if ((cmp = strcmp(filaA->Clasificador, filaB->Clasificador)) != 0) {
        return cmp;
    }

    return strcmp(filaA->nivel_general_aperturas, filaB->nivel_general_aperturas);
}

double calcularVarPorc(double indActual, double indPrevio) {
    return (indActual/ indPrevio - 1) * 100;
}
