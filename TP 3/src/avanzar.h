// Este es el header para el módulo avanzar

#ifndef Avanzar_H
#define Avanzar_H
#include <math.h>

int fstep(double *x,double *v,double *f,double L,double h,int N);
int sstep(double *v,double *f,double L,double h,int N);
int apply_PBC(double *x,double L,double N);
double velocity_verlet(double *x,double *v,double *f,double *Lut_V,double *Lut_F, double dr,int N,double h,double L,double rc);
int build_LUTS(double *Lut_V,double *Lut_F,double dr,double rc);
int correcion_T(double *x,double *v,double *f,double *Lut_V,double *Lut_F, double dr,int N,double h,double L,double rc,double T);
double ener_cinetica(double *v,int N);
double presion(double *x, double *Lut_V, double *Lut_F, double rho, double L,double dr, double rc, double T2, int N);
double distg(double *x,double *gr,double L,double rho,double deltar,int N,int lg);


#endif
