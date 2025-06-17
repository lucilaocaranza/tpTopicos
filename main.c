#include "main.h"

// tpTopicos.exe indices_icc_general_capitulos.csv Indices_items_obra.csv
/*typedef struct
{
    Fecha periodo;
    char clasificador[20]; // "Nivel general", "Capitulos", "items"
    char nivelGeneralAperturas[50];
    char tipoVariable[20]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroICC;*/

int main(int argc, char *argv[])
{
    Vector registros;
    vectorCrear(&registros, sizeof(Fila));
    //RegistroICC registros[MAX_REGISTROS];
    //int total = 0;

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
        //strcpy(fila.tipoVariable, "indice_icc");
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
        //strcpy(registros[total].tipoVariable, "indice_icc");
        fila.indiceICC = valorNum;

        vectorInsertarAlFinal(&registros, &fila);
    }

    fclose(archCapitulos);
    fclose(archItems);

    // 10 - Ordenar
    //qsort(registros, total, sizeof(RegistroICC), compararRegistros);

    vectorOrdenar(&registros, QSORT, compararPorFecha);

    //mostrarRegistrosICC(registros, total);

    vectorRecorrer(&registros, calcularVarMensual, &registros);
    vectorRecorrer(&registros, calcularVarInteranual, &registros);
/*
    Vector exportar;
    vectorCrear(&vec, sizeof(RegistroICC));

    RegistroICC fila;

    vectorRecorrer(&registros, pasarAExportar, &vec);
*/
    probarVector(&registros);

    return TODO_OK;
}

int compararRegistros(const void *a, const void *b)
{
    const RegistroICC *regA = a;
    const RegistroICC *regB = b;

    int cmp = FechaComparar(&regA->periodo, &regB->periodo);
    if (cmp)
        return cmp;

    char clasifA = *regA->clasificador;
    char clasifB = *regB->clasificador;

    if (clasifA == clasifB)
        return 0;

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

void reemplazarComaPorPunto(char *indice)
{
    while (*indice)
    {
        if (*indice == ',')
        {
            *indice = '.';
        }
        indice++;
    }
};
void guionAespacio(char *cadena)
{
    while (*cadena != '\0')
    {
        if (*cadena == '_')
        {
            *cadena = ' ';
        }
        cadena++;
    }
}
void primeraMayus(char *cadena)
{
    int letra1 = 1;

    while (*cadena != '\0')
    {
        if (isalpha(*cadena))
        {
            if (letra1)
            {
                *cadena = toupper(*cadena);
                letra1 = 0;
            }
            else
            {
                *cadena = tolower(*cadena);
            }
        }
        else if (*cadena == ' ')
        {
            letra1 = 0;
        }
        cadena++;
    }
}
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
void quitarAnteriorAlPrimerGuion(char *cadena)
{
    char *act = strchr(cadena, '_');
    act++;
    strcpy(cadena, act);
}

void mostrarRegistrosICC(RegistroICC *registros, int total)
{
    printf("%-12s | %-60s | %-20s | %-15s\n", "Periodo", "Nivel", "Indice", "Clasificador");
    printf("---------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < total; i++)
    {
        char fechaStr[11];
        FechaConvertirAGuiones(fechaStr, &(registros[i].periodo));

        printf("%-12s | %-60s | %-20f | %-15s\n",
               fechaStr,
               registros[i].nivelGeneralAperturas,
               registros[i].valor,
               registros[i].clasificador);
    }
}

void probarVector(Vector* registros) {
    /*
    Vector vec;
    vectorCrear(&vec, sizeof(Fila));

    RegistroICC fila;

    for (int i = 0; i < total; i++) {
        //fila = registros[i];
        vectorInsertarAlFinal(&vec, &fila);
    }

    vectorOrdenar(&vec, QSORT, compararPorFecha);

    vectorRecorrer(&vec, calcularVarMensual, &vec);
    vectorRecorrer(&vec, calcularVarInteranual, &vec);
*/
    printf("== Mostrar vector ==\n\n");
    printf("%-12s | %-60s | %-16s | %-15s | %-16s | %-16s\n", "Periodo", "Nivel", "Indice", "Clasificador", "var_mensual", "var_interanual");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    vectorRecorrer(registros, mostrarRegistroVector, registros);
}

void calcularVarMensual(void* elem, void* datos) {
    Fila *fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 1);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != 0) {
        double porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
        fila->varMensual = floor(porcentaje * 100) / 100;
    }
}

void calcularVarInteranual(void* elem, void* datos) {
    Fila *fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 12);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != 0) {
        double porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
        fila->varInteranual = floor(porcentaje * 100) / 100;
    }
}

int compararPorFecha(const void* a, const void* b) {
    const RegistroICC *filaA = a;
    const RegistroICC *filaB = b;

    int cmp = FechaComparar(&filaA->periodo, &filaB->periodo);
    if (cmp != 0) {
        return cmp;
    }

    char clasifA = *filaA->clasificador;
    char clasifB = *filaB->clasificador;

    if (clasifA != clasifB) {
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

double calcularVarPorc(double indActual, double indPrevio) {
    return (indActual/ indPrevio - 1) * 100;
}

void mostrarRegistroVector(void* elem, void* datos) {
    Fila *registro = elem;

    char fechaStr[11];
    FechaConvertirAGuiones(fechaStr, &(registro->periodo));

    printf("%-12s | %-60s | %-16f | %-15s | %-16f | %-16f\n",
           fechaStr,
           registro->nivelGeneralAperturas,
           registro->indiceICC,
           registro->clasificador,
           registro->varMensual,
           registro->varInteranual);
}
