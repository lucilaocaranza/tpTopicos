#include "Fecha.h"

int min(int a, int b);

void fechaGet(Fecha* fecha, int* d, int* m, int* a) {
    *d = fecha -> d;
    *m = fecha -> m;
    *a = fecha -> a;
}

bool fechaSet(Fecha* f, int d, int m, int a) {
    if (!esFechaValida(d, m, a)) {
        return false;
    }

    f->d = d;
    f->m = m;
    f->a = a;

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

Fecha fechaSumarDias(const Fecha* f, int dias) {
    Fecha fSumar = *f;
    int cdm;
    fSumar.d += dias;

    while (fSumar.d > (cdm = cantDiasMes(fSumar.m, fSumar.a))) {
        fSumar.d -= cdm;
        fSumar.m++;
        if (fSumar.m > 12) {
            fSumar.m = 1;
            fSumar.a++;
        }
    }

    return fSumar;
}

Fecha fechaSumarMeses(const Fecha* f, int meses) {
    Fecha fSumar = *f;
    fSumar.m += meses;

    while (fSumar.m > 12) {
        fSumar.m -= 12;
        fSumar.a++;
    }

    fSumar.d = min(fSumar.d, cantDiasMes(fSumar.m, fSumar.a));

    return fSumar;
}

Fecha fechaRestarMeses(const Fecha* f, int meses) {
    Fecha fSumar = *f;
    fSumar.m -= meses;

    while (fSumar.m < 1) {
        fSumar.m += 12;
        fSumar.a--;
    }

    fSumar.d = min(fSumar.d, cantDiasMes(fSumar.m, fSumar.a));

    return fSumar;
}

/*4.2 - Desarrollar una función que a partir de una fecha obtenga la correpondiente al dia siguinte. */
Fecha fechaSiguiente(const Fecha* t) {
    Fecha nFecha = *t;
    nFecha.d++;
    if (nFecha.d > cantDiasMes(nFecha.m, nFecha.a)) {
        nFecha.d = 1;
        nFecha.m++;
        if (nFecha.m > 12) {
            nFecha.m = 1;
            nFecha.a++;
        }
    }

    return nFecha;
}

/* 4.4 - Desarrollar una funcion que a partir de una fecha obtenga la que resulte de restarle N dias */
Fecha fechaRestarDias(const Fecha* fecha, int dias) {
    Fecha f = *fecha;
    //int cdm;

    f.d -= dias;
    while (f.d < 1) {
        f.m--;
        if (f.m < 1) {
            f.a--;
            f.m = 12;
        }
        f.d += cantDiasMes(f.m, f.a);
    }

    return f;
}

/* 4.5 - Desarrollar una funcion que a patir de dos fechas obtenga la cantidad de dias que hay entre ellas */
int fechaDiasEntreFechas(const Fecha* fecha1, const Fecha* fecha2) {
    Fecha menor = *fecha1, mayor = *fecha2;
    int dias = 0;

    // Clasificar la menor fecha
    if (mayor.a < menor.a || (mayor.a == menor.a && (mayor.m < menor.m || (mayor.m == menor.m && mayor.d < menor.d)))) {
        menor = *fecha2;
        mayor = *fecha1;
    }

    if (menor.a < mayor.a) {
        dias -= menor.d - 1;
        menor.d = 1;

        while (menor.m <= 12) {
            dias += cantDiasMes(menor.m, menor.a);
            menor.m++;
        }

        menor.m = 1;
        menor.a++;

        while (menor.a < mayor.a) {
            if (esBisiesto(menor.a)) {
                dias += 366;
            }
            else {
                dias += 365;
            }
            menor.a++;
        }
    }
    else {
        dias -= menor.d - 1;
        menor.d = 1;
    }

    while (menor.m < mayor.m) { // Siempre estaremos en el 1 del mes, asi que nunca ocurrira que menor.d > mayor.d dado un mismo mes
        dias += cantDiasMes(menor.m, menor.a);
        menor.m++;
    }

    dias += mayor.d - menor.d;

    return dias;
}

int fechaComparar(const Fecha* fecha1, const Fecha* fecha2) {
    Fecha f1 = *fecha1, f2 = *fecha2;

    return f1.a - f2.a != 0 ? f1.a - f2.a : (f1.m - f2.m != 0 ? f1.m - f2.m : f1.d - f2.d);
}

/* 4.6 - Desarrollar una funcion que a partir de una fecha devuelva un entero que representa el dia de la semana que le corresponde (0: domingo, 1: lunes, 2: martes, etc.) */
int fechaGetDiaSemana(const Fecha* fecha) {
    Fecha inicio;
    int dias;
    fechaSet(&inicio, 1, 1, 1601);

    dias = (fechaDiasEntreFechas(fecha, &inicio) + 1) % 7;
    return dias;
}

int min(int a, int b) {
    return a < b ? a : b;
}
