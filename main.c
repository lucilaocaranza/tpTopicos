#include "main.h"

// tpTopicos.exe indices_icc_general_capitulos.csv Indices_items_obra.csv Indices_Binario.dat
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

    //qsort(registros, total, sizeof(RegistroICC), compararRegistros);

    // 10 - Ordenar
    vectorOrdenar(&registros, QSORT, compararPorFecha);

    //mostrarRegistrosICC(registros, total);

    // 11 y 12 - Calcular
    vectorRecorrer(&registros, calcularVarMensual, &registros);
    vectorRecorrer(&registros, calcularVarInteranual, &registros);
    /*
        Vector exportar;
        vectorCrear(&vec, sizeof(RegistroICC));

        RegistroICC fila;

        vectorRecorrer(&registros, pasarAExportar, &vec);
    */
    mostrarVector(&registros);


    Vector vEstrFinal;
    vectorCrear(&vEstrFinal,sizeof(RegistroICC));

    vectorRecorrer(&registros, cargarEstructuraRegistroIcc, &vEstrFinal);
    vectorOrdenar(&vEstrFinal, QSORT, compararRegistros);

    mostrarVectorFinal(&vEstrFinal);

    vectorGrabar(&vEstrFinal,argv[3]);
    //cargarEstructuraRegistroIcc(reg,&registros);

    return TODO_OK;
}

int compararRegistros(const void *a, const void *b)
{
    const RegistroICC *regA = a;
    const RegistroICC *regB = b;

    // Comparar por periodo
    int cmp = FechaComparar(&regA->periodo, &regB->periodo);
    if (cmp)
        return cmp;

    // Si son iguales, comparar por clasificador

    char clasifA = *regA->clasificador;
    char clasifB = *regB->clasificador;

    // Si difiere en clasificaci�n, retornar ac�
    if (clasifA != clasifB) {
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

    // IS la clasificaci�n es igual, comparar por tipo_variable
    // Ordenar por tipo_var

    char* tipoA = regA->tipoVariable;
    char* tipoB = regB->tipoVariable;

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

void mostrarVector(Vector* registros)
{
    printf("== Mostrar vector ==\n\n");
    printf("%-12s | %-60s | %-16s | %-15s | %-16s | %-16s\n", "Periodo", "Nivel", "Indice", "Clasificador", "var_mensual", "var_interanual");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    vectorRecorrer(registros, mostrarRegistroVector, registros);
}
void mostrarVectorFinal(Vector* regs)
{
printf("== Mostrar vector Final ==\n\n");
    printf("%-12s | %-16s | %-14s | %-60s | %-16s\n", "Periodo", "Clasificador", "Tipo_variable", "Nivel General Aperuras", "Valor");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    vectorRecorrer(regs,mostrarRegistroVectorFinal,regs);
}
void mostrarRegistroVectorFinal(void* elem,void* datos)
{
    RegistroICC *registro = elem;

    char fechaStr[11];
    FechaConvertirAGuiones(fechaStr, &(registro->periodo));

    printf("%-12s | %-16s | %-14s | %-60s | %-16f\n",
           fechaStr,
           registro->clasificador,
           registro->tipoVariable,
           registro->nivelGeneralAperturas,
           registro->valor);
}
void calcularVarMensual(void* elem, void* datos)
{
    Fila *fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 1);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != -1)
    {
        double porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
        fila->varMensual = floor(porcentaje * 100) / 100;
    }
}

void calcularVarInteranual(void* elem, void* datos)
{
    Fila *fila = elem, filaPrev = *fila;
    Vector* vec = datos;
    int res;

    filaPrev.periodo = fechaRestarMeses(&fila->periodo, 12);
    res = vectorOrdBuscar(vec, &filaPrev, compararPorFecha);
    if (res != -1)
    {
        double porcentaje = calcularVarPorc(fila->indiceICC, filaPrev.indiceICC);
        fila->varInteranual = floor(porcentaje * 100) / 100;
    }
}

int compararPorFecha(const void* a, const void* b)
{
    const RegistroICC *filaA = a;
    const RegistroICC *filaB = b;

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

double calcularVarPorc(double indActual, double indPrevio)
{
    return (indActual/ indPrevio - 1) * 100;
}

void mostrarRegistroVector(void* elem, void* datos)
{
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

void cargarEstructuraRegistroIcc(void* vec,void* elem)
{
    Vector* vFinal = (Vector*) elem;
    Fila* v = (Fila*) vec;
    RegistroICC  reg;

    reg.periodo = v->periodo;
    strcpy(reg.clasificador,v->clasificador);
    strcpy(reg.nivelGeneralAperturas, v->nivelGeneralAperturas);

    strcpy(reg.tipoVariable,"indice_icc");
    reg.valor = v->indiceICC;
    vectorInsertarAlFinal(vFinal,&reg);

    strcpy(reg.tipoVariable,"var_mensual");
    reg.valor = v->indiceICC;
    vectorInsertarAlFinal(vFinal,&reg);

    strcpy(reg.tipoVariable,"var_interanual");
    reg.valor = v->indiceICC;
    vectorInsertarAlFinal(vFinal,&reg);

    // total++;
}
void cargarEstructuraRegistroIccVarMen(void* vec,void* elem)
{
    Vector *vFinal= (Vector*)elem;
    Fila *v = (Fila*)vec;
    RegistroICC reg;

    reg.periodo.anio= v->periodo.anio;
    reg.periodo.mes= v->periodo.mes;
    reg.periodo.dia= v->periodo.dia;
    strcpy(reg.clasificador,v->clasificador);
    strcpy(reg.nivelGeneralAperturas, v->nivelGeneralAperturas);
    strcpy(reg.tipoVariable,"Var_Mensual");
    reg.valor = v->varMensual;

    vectorInsertarAlFinal(vFinal,&reg);

}
void cargarEstructuraRegistroIccVarA(void* vec,void* elem)
{
    Vector *vFinal = (Vector*)elem;
    Fila *v =(Fila*)vec;
    RegistroICC reg;

    reg.periodo.anio= v->periodo.anio;
    reg.periodo.mes= v->periodo.mes;
    reg.periodo.dia= v->periodo.dia;
    strcpy(reg.clasificador,v->clasificador);
    strcpy(reg.nivelGeneralAperturas, v->nivelGeneralAperturas);
    strcpy(reg.tipoVariable,"Var_Anual");
    reg.valor = v->varInteranual;
    vectorInsertarAlFinal(vFinal,&reg);
}
