#ifndef TDAFECHA_H_INCLUDED
#define TDAFECHA_H_INCLUDED
#define FECHA_INVALIDA -1
#define TODO_OK 0

#include <stdio.h>
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
int FechaComparar(const Fecha* fechaA, const Fecha* fechaB);
void FechaConvertirAGuiones(char *strDestino, Fecha *origen);

#endif // TDAFECHA_H_INCLUDED
