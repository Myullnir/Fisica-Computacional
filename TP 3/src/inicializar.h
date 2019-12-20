// Este es el header para el módulo inicializar

#ifndef Inicializar_H
#define Inicializar_H
#include <math.h>

double set_x(double *x,double rho, int N);
int set_v(double *v,double T, int N);
double pair_force(double r2,double rc2,double *f_mod);
double fuerzas(double *x, double *f,double *Lut_v, double *Lut_F,double dr, double rc,int N,double L);
double Temperatura(double *v,double L,int N);
double interpolar_Luts(double *Lut_V,double *Lut_F,double r,double dr,double *f_mod);
int correlacion(double *energia,int pasos,int j);

#endif
