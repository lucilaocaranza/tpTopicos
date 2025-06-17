#include "TDAFecha.h"

int min(int a, int b);

int es_fecha_valida(const char *fecha)
{
    int largo = 0;
    const char *p = fecha;
    while (*p++)
        largo++;
    if (largo != 10)
        return FECHA_INVALIDA;

    if (*(fecha + 2) != '/' || *(fecha + 5) != '/')
        return FECHA_INVALIDA;

    int posiciones_digito[] = {0, 1, 3, 4, 6, 7, 8, 9};
    for (int i = 0; i < 8; i++)
    {
        char c = *(fecha + posiciones_digito[i]);
        if (c < '0' || c > '9')
            return FECHA_INVALIDA;
    }

    return TODO_OK;
}

int FechaCrearDesdeCadena(Fecha *fechaNueva, const char *fechaStr)
{
    int d, m, a;

    if (sscanf(fechaStr, "%d/%d/%d", &d, &m, &a) != 3)
    {
        printf("Formato inválido.\n");
        return FECHA_INVALIDA;
    }

    fechaNueva->dia = d;
    fechaNueva->mes = m;
    fechaNueva->anio = a;

    return TODO_OK;
}

void FechaConvertirAGuiones(char *strDestino, Fecha *origen)
{
    int dia = origen->dia;
    int mes = origen->mes;
    int anio = origen->anio;

    *(strDestino++) = '0' + (anio / 1000) % 10;
    *(strDestino++) = '0' + (anio / 100) % 10;
    *(strDestino++) = '0' + (anio / 10) % 10;
    *(strDestino++) = '0' + anio % 10;

    *(strDestino++) = '-';

    *(strDestino++) = '0' + mes / 10;
    *(strDestino++) = '0' + mes % 10;

    *(strDestino++) = '-';

    *(strDestino++) = '0' + dia / 10;
    *(strDestino++) = '0' + dia % 10;

    *strDestino = '\0';
}

void fechaGet(Fecha* fecha, int* d, int* m, int* a) {
    *d = fecha -> dia;
    *m = fecha -> mes;
    *a = fecha -> anio;
}

bool fechaSet(Fecha* f, int d, int m, int a) {
    if (!esFechaValida(d, m, a)) {
        return false;
    }

    f->dia = d;
    f->mes = m;
    f->anio = a;

    return true;
}

bool fechaDeCadena(Fecha* fecha, const char* str) {
    int a, m, d;

    sscanf(str, "%4d-%2d-%2d", &a, &m, &d);
    return fechaSet(fecha, d, m, a);
}

void fechaACadena(Fecha* fecha, char* str) {
    int a, m, d;

    fechaGet(fecha, &d, &m, &a);
    sprintf(str, "%4d-%2d-%2d", a, m, d);
}

bool esFechaValida(int d, int m, int a) {
    if (a < 1601) {
        return false;
    }
    if (m < 1 || m > 12) {
        return false;
    }
    if (d < 1 || d > cantDiasMes(m, a)) {
        return false;
    }
    return true;
}

int cantDiasMes(int m, int a) {
    int diasMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (m == 2 && esBisiesto(a)) {
        return 29;
    }

    return diasMes[m];
}

bool esBisiesto(int a) {
    return a % 4 == 0 && (a % 100 != 0 || a % 400 == 0);
}

Fecha fechaRestarMeses(const Fecha* f, int meses) {
    Fecha fSumar = *f;
    fSumar.mes -= meses;

    while (fSumar.mes < 1) {
        fSumar.mes += 12;
        fSumar.anio--;
    }

    fSumar.dia = min(fSumar.dia, cantDiasMes(fSumar.mes, fSumar.anio));

    return fSumar;
}

int FechaComparar(const Fecha* fechaA, const Fecha* fechaB)
{
    int cmp;
    if ((cmp = fechaA->anio - fechaB->anio) != 0)
        return cmp;

    if ((cmp = fechaA->mes - fechaB->mes) != 0)
        return cmp;

    return fechaA->dia - fechaB->dia;
}

int min(int a, int b) {
    return a < b ? a : b;
}
