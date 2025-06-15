#include "main.h"

// tpTopicos.exe indices_icc_general_capitulos.csv Indices_items_obra.csv
/*typedef struct
{
    Fecha periodo;
    char clasificador[20]; // "Nivel general", "Cap�tulos", "�tems"
    char nivelGeneralAperturas[50];
    char tipoVariable[20]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroICC;*/

void reemplazarComaPorPunto(char *indice);
void guionAespacio(char *cadena);
void primeraMayus(char *cadena);
void decodificar(char *cadena);
void clasificador(RegistroICC* reg, char* campo);
void desencriptarArchItems(char* campo);
void quitarAnteriorAlPrimerGuion(char* cadena);

int main(int argc, char *argv[])
{
    RegistroICC registros_cap[MAX_REGISTROS];
    int total = 0;

    FILE *archCapitulos = fopen(argv[1], "r");
    FILE *archItems = fopen(argv[2], "r");

    if (!archCapitulos || !archItems)
    {
        printf("Error abriendo archivos CSV.\n");
        return ERR_ARCHIVO;
    }

    char registroData[256];
    fgets(registroData, sizeof(registroData), archCapitulos); // Salto la cabecera
    while (fgets(registroData, sizeof(registroData), archCapitulos))
    {
        char *periodo = strtok(registroData, ";\"");
        char *nivel = strtok(NULL, ";\"");
        char *indiceStr = strtok(NULL, ";\"\n");
        // Campo fecha
        Fecha nuevaFecha;
        FechaCrearDesdeCadena(&nuevaFecha, periodo);
        registros_cap[total].periodo = nuevaFecha;

        // Campo nivel
        decodificar(nivel);
        guionAespacio(nivel);
        primeraMayus(nivel);
        clasificador(&registros_cap[total],nivel);

        //estas son para el archivo Items
//        desencriptarArchItems(nivel);
//        quitarAnteriorAlPrimerGuion(nivel);
//        guionAespacio(nivel);
//        primeraMayus(nivel);

        // Campo indice
        reemplazarComaPorPunto(indiceStr);
        double valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(registros_cap[total].nivelGeneralAperturas, nivel);
        strcpy(registros_cap[total].tipoVariable, "indice_icc");
        registros_cap[total].valor = valorNum;

        // Solo con fines de ver si va todo bien
        char verFecha[11];
        FechaConvertirAGuiones(verFecha, &(registros_cap[total].periodo));
        printf("Periodo: %s | Nivel: %s | Indice: %f |Clasificador: %s\n", verFecha, registros_cap[total].nivelGeneralAperturas, registros_cap[total].valor, registros_cap[total].clasificador);
        total++;
    }
    fgets(registroData, sizeof(registroData), archItems); // Salto la cabecera
    while (fgets(registroData, sizeof(registroData), archItems))
    {
        char *periodo = strtok(registroData, ";\"");
        char *nivel = strtok(NULL, ";\"");
        char *indiceStr = strtok(NULL, ";\"\n");
        // Campo fecha
        Fecha nuevaFecha;
        FechaCrearDesdeCadena(&nuevaFecha, periodo);
        registros_cap[total].periodo = nuevaFecha;

        // Campo nivel

        // Campo indice
        reemplazarComaPorPunto(indiceStr);
        double valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(registros_cap[total].nivelGeneralAperturas, nivel);
        strcpy(registros_cap[total].tipoVariable, "indice_icc");
        registros_cap[total].valor = valorNum;

        // Solo con fines de ver si va todo bien
        char verFecha[11];
        FechaConvertirAGuiones(verFecha, &(registros_cap[total].periodo));
        printf("Periodo: %s | Nivel: %s | Indice: %f\n", verFecha, registros_cap[total].nivelGeneralAperturas, registros_cap[total].valor);
        total++;
    }

    fclose(archCapitulos);
    fclose(archItems);
    return TODO_OK;
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
void clasificador(RegistroICC* reg, char* campo)
{
    if (strcmp(campo, "Nivel general") == 0)
        strcpy(reg->clasificador, "Nivel general");
    else
        strcpy(reg->clasificador, "Capitulos");
}
void desencriptarArchItems(char* campo)
{
    while(*campo)
    {
        switch(*campo)
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
void quitarAnteriorAlPrimerGuion(char* cadena)
{
    char* act = strchr(cadena,'_');
    strcpy(cadena,act);
}
