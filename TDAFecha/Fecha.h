#include <stdio.h>
#include <stdbool.h>
#define TFecha Fecha

typedef struct {
    int d;
    int m;
    int a;
} TFecha;

bool fechaSet(Fecha*, int d, int m, int a);
void fechaGet(Fecha*, int* d, int* m, int* a);
void fechaACadena(Fecha*, char* str);
bool fechaDeCadena(Fecha*, const char* str);
bool esFechaValida(int d, int m, int a);
int cantDiasMes(int m, int a);
bool esBisiesto(int a);
Fecha fechaSumarDias(const Fecha*, int dias);
Fecha fechaRestarDias(const Fecha*, int dias);
Fecha fechaSumarMeses(const Fecha*, int meses);
Fecha fechaRestarMeses(const Fecha*, int meses);
Fecha fechaSiguiente(const Fecha*);
int fechaComparar(const Fecha* f1, const Fecha* f2);
int fechaDiasEntreFechas(const Fecha*, const Fecha*);
int fechaGetDiaSemana(const Fecha*);
