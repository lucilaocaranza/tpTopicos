#include "main.h"

// tpTopicos.exe indices_icc_general_capitulos.csv Indices_items_obra.csv Indices_Binario.dat

int main(int argc, char *argv[])
{
    Vector registros, vEstrFinal;
    FILE *archCapitulos, *archItems;
    Fila fila;

    char *periodo, *nivel, *indiceStr;
    char registroData[256];
    double valorNum;

    system("chcp 1252 > nul");

    vectorCrear(&registros, sizeof(Fila));

    archCapitulos = fopen(argv[ARG_ARCH_CAPITULOS], "r");
    archItems = fopen(argv[ARG_ARCH_ITEMS], "r");

    if (!archCapitulos || !archItems)
    {
        printf("Error abriendo archivos CSV.\n");
        return ERR_ARCHIVO;
    }


    //////////////////// Archivo 1///////////////////////////////////////////

    fgets(registroData, sizeof(registroData), archCapitulos); // Salto la cabecera
    while (fgets(registroData, sizeof(registroData), archCapitulos))
    {
        periodo = strtok(registroData, ";\"");
        nivel = strtok(NULL, ";\"");
        indiceStr = strtok(NULL, ";\"\n");

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
        valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(fila.nivelGeneralAperturas, nivel);
        fila.indiceICC = valorNum;

        vectorInsertarAlFinal(&registros, &fila);
    }

    //////////Archivo 2/////////////////////////

    fgets(registroData, sizeof(registroData), archItems); // Salto la cabecera
    while (fgets(registroData, sizeof(registroData), archItems))
    {
        periodo = strtok(registroData, ";\"");
        nivel = strtok(NULL, ";\"");
        indiceStr = strtok(NULL, ";\"\n");

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
        valorNum = strtod(indiceStr, NULL);

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

    //mostrarVectorInicial(&registros, "Vector inicial ordenado (hasta p. 10)");

    // Final - Reorganizar, reordenar
    vectorCrear(&vEstrFinal, sizeof(RegistroICC));

    vectorRecorrer(&registros, cargarEstructuraRegistroIcc, &vEstrFinal);
    vectorDestruir(&registros);

    vectorOrdenar(&vEstrFinal, QSORT, compararRegistros);

    //mostrarVectorFinal(&vEstrFinal, "Vector final exportado");

    // Final - Guardar
    vectorGrabar(&vEstrFinal, argv[ARG_ARCH_BIN]);
    vectorDestruir(&vEstrFinal);

    // Releer
    vectorCrearDeArchivo(&vEstrFinal, sizeof(RegistroICC), argv[ARG_ARCH_BIN]);
    mostrarVectorFinal(&vEstrFinal, "Vector leído de binario");

    return TODO_OK;
}

