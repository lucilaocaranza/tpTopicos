#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED
#include "../TDAVector/Vector.h"
#include <stdio.h>
#include "../TDAFecha/TDAFecha.h"
#include "../structs/fila.h"
#include "../structs/registroICC.h"
void mostrarVector(Vector *registros);
void mostrarVectorFinal(Vector *regs);
void mostrarRegistroVector(void *elem, void *datos);
void mostrarRegistroVectorFinal(void *elem, void *datos);

#endif // IO_H_INCLUDED
