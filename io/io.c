#include "io.h"

void mostrarVectorInicial(Vector *registros, const char *titulo)
{
    printf("\n== %s ==\n\n", titulo);
    printf("%-10s | %-42s | %-12s | %-14s | %-12s | %-14s\n", "Periodo", "Nivel", "Indice", "Clasificador", "var_mensual", "var_interanual");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    vectorRecorrer(registros, mostrarRegistroVecInicial, registros);
}

void mostrarVectorFinal(Vector *regs, const char *titulo)
{
    printf("\n== %s ==\n\n", titulo);
    printf("%-10s | %-14s | %-50s | %-10s | %-16s\n", "Periodo", "Clasificador", "Nivel General Aperuras", "Tipo_variable", "Valor");
    printf("----------------------------------------------------------------------------------------------------------------\n");

    vectorRecorrer(regs, mostrarRegistroVecFinal, regs);
}

void mostrarRegistroVecInicial(void *elem, void *datos)
{
    Fila *registro = elem;
    char periodo[11];

    FechaConvertirAGuiones(periodo, &registro->periodo);

    printf("%-10s | %-42s | %-12f | %-14s | %-12f | %-14f\n",
           periodo,
           registro->nivelGeneralAperturas,
           registro->indiceICC,
           registro->clasificador,
           registro->varMensual,
           registro->varInteranual);
}

void mostrarRegistroVecFinal(void *elem, void *datos)
{
    RegistroICC *registro = elem;

    printf("%-10s | %-14s | %-50s | %-14s | %-10s\n",
           registro->periodo,
           registro->clasificador,
           registro->nivelGeneralAperturas,
           registro->tipoVariable,
           registro->valor);
}
