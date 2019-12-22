// Este es el header para el módulo general

#ifndef General_H
#define General_H
#include <math.h>

double Random();
double Gaussiana(float mu, float sigma);
double Norma(double *x);
int delta_x(double *x1, double *x2, double L, double *Dx);

#endif
