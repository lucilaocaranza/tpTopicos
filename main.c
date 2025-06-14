#include "main.h"

// tpTopicos.exe indices_icc_general_capitulos.csv Indices_items_obra.csv
/*typedef struct
{
    char periodo[11];      // "aaaa-mm-dd"
    char clasificador[20]; // "Nivel general", "Cap�tulos", "�tems"
    char nivelGeneralAperturas[50];
    char tipoVariable[20]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroICC;*/

void reemplazarComaPorPunto(char *indice);

int main(int argc, char *argv[])
{
    // RegistroICC registros_cap[MAX_REGISTROS];
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

        reemplazarComaPorPunto(indiceStr);
        double valor = strtod(indiceStr, NULL);
        // indice

        // Se aplican las modificaciones a los campos.
        // Se copia al array de registros.
        printf("Periodo: %s | Nivel: %s | Indice: %f\n", periodo, nivel, valor);
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
