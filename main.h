#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./TDAFecha/TDAFecha.h"

#define ARG_INDICE_ICC 1
#define ARG_INDICE_ITEMS 2
#define TODO_OK 0
#define ERR_ARCHIVO 1
#define MAX_REGISTROS 1000

typedef struct
{
    Fecha periodo;
    char clasificador[20]; // "Nivel general", "Cap�tulos", "�tems"
    char nivelGeneralAperturas[50];
    char tipoVariable[20]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroICC;

int comparar(const void* a, const void* b);
void reemplazarComaPorPunto(char *indice);
void guionAespacio(char *cadena);
void primeraMayus(char *cadena);
void decodificar(char *cadena);
void clasificador(RegistroICC *reg, char *campo);
void clasificadorEnItem(RegistroICC *reg);
void desencriptarArchItems(char *campo);
void quitarAnteriorAlPrimerGuion(char *cadena);
void mostrarRegistrosICC(RegistroICC *registros, int total);
#endif // MAIN_H_INCLUDED
