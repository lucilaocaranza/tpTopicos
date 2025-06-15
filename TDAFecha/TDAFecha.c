#include "TDAFecha.h"

int es_fecha_valida(const char *fecha)
{

  int largo = 0;
  const char *p = fecha;
  while (*p++)
    largo++;
  if (largo != 10)
    return FECHA_INVALIDA;

  if (*(fecha + 2) != '/' || *(fecha + 5) != '/')
    return FECHA_INVALIDA;

  int posiciones_digito[] = {0, 1, 3, 4, 6, 7, 8, 9};
  for (int i = 0; i < 8; i++)
  {
    char c = *(fecha + posiciones_digito[i]);
    if (c < '0' || c > '9')
      return FECHA_INVALIDA;
  }

  return TODO_OK;
}

int FechaCrearDesdeCadena(Fecha *fechaNueva, const char *fechaStr)
{
  int d, m, a;

  if (sscanf(fechaStr, "%d/%d/%d", &d, &m, &a) != 3)
  {
    printf("Formato inválido.\n");
    return FECHA_INVALIDA;
  }

  fechaNueva->dia = d;
  fechaNueva->mes = m;
  fechaNueva->anio = a;

  return TODO_OK;
}
void FechaConvertirAGuiones(char *strDestino, Fecha *origen)
{
  int dia = origen->dia;
  int mes = origen->mes;
  int anio = origen->anio;

  *(strDestino++) = '0' + (anio / 1000) % 10;
  *(strDestino++) = '0' + (anio / 100) % 10;
  *(strDestino++) = '0' + (anio / 10) % 10;
  *(strDestino++) = '0' + anio % 10;

  *(strDestino++) = '-';

  *(strDestino++) = '0' + mes / 10;
  *(strDestino++) = '0' + mes % 10;

  *(strDestino++) = '-';

  *(strDestino++) = '0' + dia / 10;
  *(strDestino++) = '0' + dia % 10;

  *strDestino = '\0';
}
