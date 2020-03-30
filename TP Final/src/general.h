// Este es el header para el módulo general

#ifndef General_H
#define General_H
#include <math.h>

double Random();
double Gaussiana(float mu, float sigma);
double Norma(double *x);
int delta_x(double *x1, double *x2, double L, double *Dx);

//################################################################################################

// Acá vienen los structs.
// El struct Red tiene los datos que definen mi red, y la info de la red
struct Red{
	int N2; // Largo de la red
	int F; // Cantidad de temas sobre los que opinar
	int Q; // Cantidad de opiniones
	int Tpers; // Cantidad total de personas en la red
	int *red; // Array unidimensional que simula la red, con F casilleros para cada individuo
	int L; // Factor que determina el crecimiento de las potencias de 10 en el etiquetado.
};


#endif
