#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "./TDAVector/Vector.h"
#include "./TDAFecha/TDAFecha.h"

#define ARG_INDICE_ICC 1
#define ARG_INDICE_ITEMS 2
#define TODO_OK 0
#define ERR_ARCHIVO 1
#define MAX_REGISTROS 1000

// Estructura final para exportar los registros binarios
typedef struct
{
    Fecha periodo;
    char clasificador[20]; // "Nivel general", "Capitulos", "Items"
    char nivelGeneralAperturas[50];
    char tipoVariable[20]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroICC;

// Estructura intermedia para juntar y calcular todo
typedef struct
{
    Fecha periodo;
    char clasificador[20];
    char nivelGeneralAperturas[50];
    double indiceICC;
    double varMensual, varInteranual;
} Fila;

int compararRegistros(const void* a, const void* b);
void reemplazarComaPorPunto(char *indice);
void guionAespacio(char *cadena);
void primeraMayus(char *cadena);
void decodificar(char *cadena);
void clasificador(Fila *reg, char *campo);
void clasificadorEnItem(Fila *reg);
void desencriptarArchItems(char *campo);
void quitarAnteriorAlPrimerGuion(char *cadena);
void mostrarRegistrosICC(RegistroICC *registros, int total);

void mostrarVector(Vector* regs);
void calcularVarMensual(void* elem, void* datos);
void calcularVarInteranual(void* elem, void* datos);
int compararPorFecha(const void* a, const void* b);
double calcularVarPorc(double indActual, double indPrevio);
void mostrarRegistroVector(void* elem, void* datos);

#endif // MAIN_H_INCLUDED
