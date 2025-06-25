#include "procesamientoDeArchivo.h"

void decodificar(char *cadena)
{
    int pos = 0, desp;
    char c, base;

    while (*cadena != 0)
    {
        c = *cadena;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            base = (c >= 'a') ? 'a' : 'A';
            desp = (pos % 2 == 0) ? 4 : 2;

            *cadena = base + ((c - base + desp) % 26);
        }
        pos++;
        cadena++;
    }
}
void clasificador(Fila *reg, char *campo)
{
    if (strcmp(campo, "Nivel general") == 0)
        strcpy(reg->clasificador, "Nivel general");
    else
        strcpy(reg->clasificador, "Cap�tulos");
}
void clasificadorEnItem(Fila *reg)
{
    strcpy(reg->clasificador, "�tems");
}
void desencriptarArchItems(char *campo)
{
    while (*campo)
    {
        switch (*campo)
        {
        case '@':
            *campo = 'a';
            break;
        case '8':
            *campo = 'b';
            break;
        case '3':
            *campo = 'e';
            break;
        case '1':
            *campo = 'i';
            break;
        case '0':
            *campo = 'o';
            break;
        case '$':
            *campo = 's';
            break;
        case '7':
            *campo = 't';
            break;
        case '|':
            *campo = 'l';
            break;
        case '5':
            *campo = 'm';
            break;
        case '9':
            *campo = 'n';
            break;
        }
        campo++;
    }
}

void cargarEstructuraRegistroIcc(void *vec, void *elem)
{
    Vector *vFinal = (Vector *)elem;
    Fila *v = (Fila *)vec;
    RegistroICC reg;

    // Pasar fecha, clasificador y nivel general aperturas
    FechaConvertirAGuiones(reg.periodo, &v->periodo);
    strcpy(reg.clasificador, v->clasificador);
    strcpy(reg.nivelGeneralAperturas, v->nivelGeneralAperturas);

    // Pasar indice_icc como string
    strcpy(reg.tipoVariable, "indice_icc");
    sprintf(reg.valor, "%-10f", v->indiceICC);
    *(reg.valor + 10) = '\0';
    vectorInsertarAlFinal(vFinal, &reg);

    strcpy(reg.tipoVariable, "var_mensual");

    // Pasar var_mensual seg�n si es NA o no
    if (v->varMensual > -101)
        sprintf(reg.valor, "%-.2f", v->varMensual);
    else
        strcpy(reg.valor, "NA");

    vectorInsertarAlFinal(vFinal, &reg);

    strcpy(reg.tipoVariable, "var_interanual");

    // Pasar var_interanual seg�n si es NA o no
    if (v->varInteranual > -101)
        sprintf(reg.valor, "%-.2f", v->varInteranual);
    else
        strcpy(reg.valor, "NA");

    vectorInsertarAlFinal(vFinal, &reg);
}
