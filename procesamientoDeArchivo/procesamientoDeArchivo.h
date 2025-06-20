#ifndef PROCESAMIENTODEARCHIVO_H_INCLUDED
#define PROCESAMIENTODEARCHIVO_H_INCLUDED
#include "../structs/fila.h"
#include "../TDAVector/Vector.h"
#include "../structs/registroICC.h"

void decodificar(char *cadena);
void clasificador(Fila *reg, char *campo);
void clasificadorEnItem(Fila *reg);
void desencriptarArchItems(char *campo);
void cargarEstructuraRegistroIcc(void *vec, void *elem);
#endif // PROCESAMIENTODEARCHIVO_H_INCLUDED
