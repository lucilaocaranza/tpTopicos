#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED
#include "../TDAVector/Vector.h"
#include <stdio.h>
#include "../TDAFecha/TDAFecha.h"
#include "../structs/fila.h"
#include "../structs/registroICC.h"
void mostrarVectorInicial(Vector *registros, const char *titulo);
void mostrarVectorFinal(Vector *regs, const char *titulo);
void mostrarRegistroVecInicial(void *elem, void *datos);
void mostrarRegistroVecFinal(void *elem, void *datos);

#endif // IO_H_INCLUDED
