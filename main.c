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

        // Campo indice
        reemplazarComaPorPunto(indiceStr);
        double valorNum = strtod(indiceStr, NULL);

        // Se copia a la estructura de Registros
        strcpy(registros_cap[total].tipoVariable, "indice_icc");
        registros_cap[total].valor = valorNum;

        // Solo con fines de ver si va todo bien
        char verFecha[11];
        FechaConvertirAGuiones(verFecha, &(registros_cap[total].periodo));
        printf("Periodo: %s | Nivel: %s | Indice: %f\n", verFecha, nivel, valorNum);
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
