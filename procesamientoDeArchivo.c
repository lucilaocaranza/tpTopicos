#include "procesamientoDeArchivo.h"

void decodificar(char *cadena)
{

    int pos = 0;
    while (*cadena != 0)
    {
        char c = *cadena;
        char base;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            base = (c >= 'a') ? 'a' : 'A';
            int desp = (pos % 2 == 0) ? 4 : 2;

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
        strcpy(reg->clasificador, "Capitulos");
}
void clasificadorEnItem(Fila *reg)
{
    strcpy(reg->clasificador, "Items");
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

    FechaConvertirAGuiones(reg.periodo, &v->periodo);
    strcpy(reg.clasificador, v->clasificador);
    strcpy(reg.nivelGeneralAperturas, v->nivelGeneralAperturas);

    strcpy(reg.tipoVariable, "indice_icc");
    reg.valor = v->indiceICC;
    vectorInsertarAlFinal(vFinal, &reg);

    if (v->varMensual > -101)
    {
        strcpy(reg.tipoVariable, "var_mensual");
        reg.valor = v->varMensual;
        vectorInsertarAlFinal(vFinal, &reg);
    }

    if (v->varInteranual > -101)
    {
        strcpy(reg.tipoVariable, "var_interanual");
        reg.valor = v->varInteranual;
        vectorInsertarAlFinal(vFinal, &reg);
    }
}
