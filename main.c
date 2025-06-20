#include "main.h"

// tpTopicos.exe indices_icc_general_capitulos.csv Indices_items_obra.csv Indices_Binario.dat

int main(int argc, char *argv[])
{
    Vector registros;
    vectorCrear(&registros, sizeof(Fila));

    FILE *archCapitulos = fopen(argv[1], "r");
    FILE *archItems = fopen(argv[2], "r");

    if (!archCapitulos || !archItems)
    {
        printf("Error abriendo archivos CSV.\n");
        return ERR_ARCHIVO;
    }

    char registroData[256];

    //////////////////// Archivo 1///////////////////////////////////////////

    fgets(registroData, sizeof(registroData), archCapitulos); // Salto la cabecera
    while (fgets(registroData, sizeof(registroData), archCapitulos))
    {
        char *periodo = strtok(registroData, ";\"");
        char *nivel = strtok(NULL, ";\"");
        char *indiceStr = strtok(NULL, ";\"\n");

        Fila fila;

        // Campo fecha
        Fecha nuevaFecha;
        FechaCrearDesdeCadena(&nuevaFecha, periodo);
        fila.periodo = nuevaFecha;

        // Campo nivel
        decodificar(nivel);
        guionAespacio(nivel);
        primeraMayus(nivel);

        // Clasificador
        clasificador(&fila, nivel);

        // Campo indice
        reemplazarComaPorPunto(indiceStr);
        double valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(fila.nivelGeneralAperturas, nivel);
        fila.indiceICC = valorNum;

        vectorInsertarAlFinal(&registros, &fila);
    }

    //////////Archivo 2/////////////////////////

    fgets(registroData, sizeof(registroData), archItems); // Salto la cabecera
    while (fgets(registroData, sizeof(registroData), archItems))
    {
        char *periodo = strtok(registroData, ";\"");
        char *nivel = strtok(NULL, ";\"");
        char *indiceStr = strtok(NULL, ";\"\n");

        Fila fila;

        // Campo fecha
        Fecha nuevaFecha;
        FechaCrearDesdeCadena(&nuevaFecha, periodo);
        fila.periodo = nuevaFecha;

        // Campo nivel
        desencriptarArchItems(nivel);
        quitarAnteriorAlPrimerGuion(nivel);
        guionAespacio(nivel);
        primeraMayus(nivel);

        // Clasificador
        clasificadorEnItem(&fila);

        // Campo indice
        reemplazarComaPorPunto(indiceStr);
        double valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(fila.nivelGeneralAperturas, nivel);
        fila.indiceICC = valorNum;

        vectorInsertarAlFinal(&registros, &fila);
    }

    fclose(archCapitulos);
    fclose(archItems);

    // 10 - Ordenar
    vectorOrdenar(&registros, QSORT, compararPorFechayClasificador);

    // 11 y 12 - Calcular
    vectorRecorrer(&registros, calcularVarMensual, &registros);
    vectorRecorrer(&registros, calcularVarInteranual, &registros);

    mostrarVector(&registros);

    Vector vEstrFinal;
    vectorCrear(&vEstrFinal, sizeof(RegistroICC));

    vectorRecorrer(&registros, cargarEstructuraRegistroIcc, &vEstrFinal);
    vectorOrdenar(&vEstrFinal, QSORT, compararRegistros);

    mostrarVectorFinal(&vEstrFinal);

    vectorGrabar(&vEstrFinal, argv[3]);

    return TODO_OK;
}

