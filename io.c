#include "io.h"

void mostrarVector(Vector *registros)
{
    printf("== Mostrar vector ==\n\n");
    printf("%-12s | %-60s | %-16s | %-15s | %-16s | %-16s\n", "Periodo", "Nivel", "Indice", "Clasificador", "var_mensual", "var_interanual");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    vectorRecorrer(registros, mostrarRegistroVector, registros);
}

void mostrarVectorFinal(Vector *regs)
{
    printf("== Mostrar vector Final ==\n\n");
    printf("%-12s | %-16s | %-14s | %-60s | %-16s\n", "Periodo", "Clasificador", "Tipo_variable", "Nivel General Aperuras", "Valor");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    vectorRecorrer(regs, mostrarRegistroVectorFinal, regs);
}
void mostrarRegistroVectorFinal(void *elem, void *datos)
{
    RegistroICC *registro = elem;

    printf("%-12s | %-16s | %-14s | %-60s | %-16f\n",
           registro->periodo,
           registro->clasificador,
           registro->tipoVariable,
           registro->nivelGeneralAperturas,
           registro->valor);
}

void mostrarRegistroVector(void *elem, void *datos)
{
    Fila *registro = elem;

    char periodo[11];
    FechaConvertirAGuiones(periodo, &registro->periodo);

    printf("%-12s | %-60s | %-16f | %-15s | %-16f | %-16f\n",
           periodo,
           registro->nivelGeneralAperturas,
           registro->indiceICC,
           registro->clasificador,
           registro->varMensual,
           registro->varInteranual);
}
