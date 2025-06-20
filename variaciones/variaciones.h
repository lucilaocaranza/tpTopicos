#ifndef VARIACIONES_H_INCLUDED
#define VARIACIONES_H_INCLUDED

#include <math.h>
#include "../TDAVector/Vector.h"
#include "../comparators/comparators.h"

void calcularVarMensual(void *elem, void *datos);
void calcularVarInteranual(void *elem, void *datos);
double calcularVarPorc(double indActual, double indPrevio);

#endif // VARIACIONES_H_INCLUDED
