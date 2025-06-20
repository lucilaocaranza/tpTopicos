#include "comparators.h"
int compararPorFechayClasificador(const void *a, const void *b)
{
    const Fila *filaA = a;
    const Fila *filaB = b;

    int cmp = FechaComparar(&filaA->periodo, &filaB->periodo);
    if (cmp != 0)
    {
        return cmp;
    }

    char clasifA = *filaA->clasificador;
    char clasifB = *filaB->clasificador;

    if (clasifA != clasifB)
    {
        if (clasifA == 'N') // Nivel general
            return -1;
        else if (clasifA == 'I') // Items
            return 1;

        // clasifA == Capitulos

        if (clasifB == 'N')
            return 1;
        else
            return -1;
    }

    return strcmp(filaA->nivelGeneralAperturas, filaB->nivelGeneralAperturas);
}

int compararRegistros(const void *a, const void *b)
{
    const RegistroICC *regA = a;
    const RegistroICC *regB = b;

    Fecha fechaA, fechaB;

    fechaDeCadena(&fechaA, regA->periodo);
    fechaDeCadena(&fechaB, regB->periodo);

    // Comparar por periodo
    int cmp = FechaComparar(&fechaA, &fechaB);
    if (cmp)
        return cmp;

    // Si son iguales, comparar por clasificador

    char clasifA = *regA->clasificador;
    char clasifB = *regB->clasificador;

    // Si difiere en clasificacion, retornar aca
    if (clasifA != clasifB)
    {
        if (clasifA == 'N') // A = Nivel general -> B es menor
            return -1;
        else if (clasifA == 'I') // A = Items -> B es mayor
            return 1;

        // clasifA == Capitulos (el del medio)

        if (clasifB == 'N') // B = Nivel general -> B > A
            return 1;
        else
            return -1; // B = Items -> A > B
    }

    // IS la clasificacion es igual, comparar por tipo_variable
    // Ordenar por tipo_var

    const char *tipoA = regA->tipoVariable;
    const char *tipoB = regB->tipoVariable;

    if (strcmp(tipoA, tipoB) == 0) // Son iguales
        return 0;

    if (strcmp(tipoA, "indice_icc") == 0) // indice_icc
        return -1;
    else if (strcmp(tipoA, "var_interanual") == 0) // var_interanual
        return 1;

    // clasifA == var_mensual

    if (strcmp(tipoB, "indice_icc") == 0)
        return 1;
    else
        return -1;
}
