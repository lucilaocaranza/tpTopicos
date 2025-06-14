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
        // Se aplican las modificaciones a los campos.
        // Se copia al array de registros.
        total++;
    }

    fclose(archCapitulos);
    fclose(archItems);
    return TODO_OK;
}
