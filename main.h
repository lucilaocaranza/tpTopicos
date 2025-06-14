#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#define ARG_INDICE_ICC 1
#define ARG_INDICE_ITEMS 2
#define TODO_OK 0
#define ERR_ARCHIVO 1
#define MAX_REGISTROS 1000

typedef struct
{
    char periodo[11];      // "aaaa-mm-dd"
    char clasificador[20]; // "Nivel general", "Cap�tulos", "�tems"
    char nivelGeneralAperturas[50];
    char tipoVariable[20]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroICC;

#endif // MAIN_H_INCLUDED
