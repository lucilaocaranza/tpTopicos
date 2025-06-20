#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED
#include "../TDAFecha/TDAFecha.h"
// Estructura intermedia para juntar y calcular todo
typedef struct
{
    Fecha periodo;
    char clasificador[20];
    char nivelGeneralAperturas[40];
    double indiceICC;
    double varMensual, varInteranual;
} Fila;

#endif // FILA_H_INCLUDED
