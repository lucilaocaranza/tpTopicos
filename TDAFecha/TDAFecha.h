#ifndef TDAFECHA_H_INCLUDED
#define TDAFECHA_H_INCLUDED
#define FECHA_INVALIDA -1
#define TODO_OK 0

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int dia,
        mes,
        anio;

} Fecha;

int es_fecha_valida(const char *fecha);
int FechaCrearDesdeCadena(Fecha *fechaNueva, const char *fecha);
void FechaConvertirAGuiones(char *strDestino, Fecha *origen);

bool fechaSet(Fecha*, int d, int m, int a);
void fechaGet(Fecha*, int* d, int* m, int* a);
void fechaACadena(Fecha*, char* str);
bool fechaDeCadena(Fecha*, const char* str);
bool esFechaValida(int d, int m, int a);
int cantDiasMes(int m, int a);
bool esBisiesto(int a);
Fecha fechaRestarMeses(const Fecha*, int meses);
int FechaComparar(const Fecha* fechaA, const Fecha* fechaB);

#endif // TDAFECHA_H_INCLUDED
