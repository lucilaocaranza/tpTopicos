#include "main.h"

int main(int argc, char *argv[]) {
    RegistroICC registros[MAX_REGISTROS];
    int total = 0;

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
        // Campo fecha
        Fecha nuevaFecha;
        FechaCrearDesdeCadena(&nuevaFecha, periodo);
        registros[total].periodo = nuevaFecha;

        // Campo nivel
        decodificar(nivel);
        guionAespacio(nivel);
        primeraMayus(nivel);

        // Clasificador
        clasificador(&registros[total], nivel);

        // Campo indice
        reemplazarComaPorPunto(indiceStr);
        double valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(registros[total].nivelGeneralAperturas, nivel);
        strcpy(registros[total].tipoVariable, "indice_icc");
        registros[total].valor = valorNum;

        total++;
    }

    //////////Archivo 2/////////////////////////

    fgets(registroData, sizeof(registroData), archItems); // Salto la cabecera
    while (fgets(registroData, sizeof(registroData), archItems))
    {
        char *periodo = strtok(registroData, ";\"");
        char *nivel = strtok(NULL, ";\"");
        char *indiceStr = strtok(NULL, ";\"\n");
        // Campo fecha
        Fecha nuevaFecha;
        FechaCrearDesdeCadena(&nuevaFecha, periodo);
        registros[total].periodo = nuevaFecha;

        // Campo nivel
        desencriptarArchItems(nivel);
        quitarAnteriorAlPrimerGuion(nivel);
        guionAespacio(nivel);
        primeraMayus(nivel);

        // Clasificador
        clasificadorEnItem(&registros[total]);

        // Campo indice
        reemplazarComaPorPunto(indiceStr);
        double valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(registros[total].nivelGeneralAperturas, nivel);
        strcpy(registros[total].tipoVariable, "indice_icc");
        registros[total].valor = valorNum;

        total++;
    }
    mostrarRegistrosICC(registros, total);
    probarVector(registros, total);

    fclose(archCapitulos);
    fclose(archItems);
    return TODO_OK;
}

void probarVector(RegistroICC* registros, int total) {
    Vector vec;
    vectorCrear(&vec, sizeof(Fila));

    Fila fila;

    for (int i = 0; i < total; i++) {
        fila.periodo = registros[i].periodo;
        strcpy(fila.nivelGeneralAperturas, registros[i].nivelGeneralAperturas);
        strcpy(fila.clasificador, registros[i].clasificador);
        fila.indiceICC = registros[i].valor;

        vectorInsertarAlFinal(&vec, &fila);
    }

    vectorRecorrer(&vec, calcularVarMensual, &vec);
    vectorRecorrer(&vec, calcularVarInteranual, &vec);
}

/**
    Pasar NULL a filaPrev significa que es el primer mes y no debería ser calculado.
    La función que llame deberá comprobar si es una fecha válida.
*/
void calcularVarMensual(void* elem, void* datos) {
    Fila* fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 1);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != 0) {
        double porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
        fila->varMensual = floor(porcentaje * 100) / 100;
    }
}

/**
    Pasar NULL a filaPrev significa que no paso un año y no debería ser calculado.
    La función que llame deberá comprobar si es una fecha válida.
*/
void calcularVarInteranual(void* elem, void* datos) {
    Fila* fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 12);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != 0) {
        double porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
        fila->varMensual = floor(porcentaje * 100) / 100;
    }
}

int compararPorFecha(const void* a, const void* b) {
    const Fila* filaA = a;
    const Fila* filaB = b;

    int cmp = fechaComparar(&filaA->periodo, &filaB->periodo);
    if (cmp != 0) {
        return cmp;
    }

    if ((cmp = strcmp(filaA->clasificador, filaB->clasificador)) != 0) {
        return cmp;
    }

    return strcmp(filaA->nivelGeneralAperturas, filaB->nivelGeneralAperturas);
}

double calcularVarPorc(double indActual, double indPrevio) {
    return (indActual/ indPrevio - 1) * 100;
}

// Resto del proyecto

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
void clasificador(RegistroICC *reg, char *campo)
{
    if (strcmp(campo, "Nivel general") == 0)
        strcpy(reg->clasificador, "Nivel general");
    else
        strcpy(reg->clasificador, "Capitulos");
}
void clasificadorEnItem(RegistroICC *reg)
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
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

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
