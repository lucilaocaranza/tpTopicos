#include "variaciones.h"

void calcularVarMensual(void *elem, void *datos)
{
    Fila *fila = elem, filaPrev = *fila;
    Vector *vec = datos;
    int res;
    double porcentaje;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 1);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFechayClasificador);
    if (res != -1)
        porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
    else
        porcentaje = -101;

    fila->varMensual = floor(porcentaje * 100) / 100;
}

void calcularVarInteranual(void *elem, void *datos)
{
    Fila *fila = elem, filaPrev = *fila;
    Vector *vec = datos;
    int res;
    double porcentaje;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 12);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFechayClasificador);
    if (res != -1)
        porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
    else
        porcentaje = -101;

    fila->varInteranual = floor(porcentaje * 100) / 100;
}
double calcularVarPorc(double indActual, double indPrevio)
{
    return (indActual / indPrevio - 1) * 100;
}
