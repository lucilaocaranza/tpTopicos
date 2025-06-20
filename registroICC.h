#ifndef REGISTROICC_H_INCLUDED
#define REGISTROICC_H_INCLUDED

// Estructura final para exportar los registros binarios
typedef struct
{
    char periodo[11];
    char clasificador[20]; // "Nivel general", "Capitulos", "Items"
    char nivelGeneralAperturas[40];
    char tipoVariable[15]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroICC;

#endif // REGISTROICC_H_INCLUDED
