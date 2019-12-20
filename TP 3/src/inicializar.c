// En este archivo defino todas las funciones de inicialización, pero no las declaro.
// El <stdio.h>, o <math.h>, o <stdlib.h>, ¿Son necesarios?

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "general.h"
#include "inicializar.h"

// Esta función te setea las posiciones de tus partículas inicialmente
double set_x(double *x,double rho, int N){
	int i,j,k,l=0,n;
	double L,a;
	n = (int) ceil(cbrt(N));
	L = cbrt(N/rho); // rho = N/V = N/L^3
	a = L/n;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				*(x+3*l) = a/2+i*a;
				*(x+3*l+1) = a/2+j*a;
				*(x+3*l+2) = a/2+k*a;
				l++;
				if(l>=N){
					i += n;
					j += n;
					k += n;
				}
			}
		}
	}
	return L;
}

// Esta función te setea las velocidades de tus partículas inicialmente
int set_v(double *v,double T, int N){
	int i,k;
	float sigma;//,mu;
	double *vprom;
	vprom = (double*) malloc(3*sizeof(double));
	sigma = sqrt(T);
	//mu = T/3.0;
	//L = (int) round(cbrt(n*n*n/rho));
	//a = L/n;
	for(i=0;i<3;i++) *(vprom+i) = 0;
	for(i=0;i<3*N;i++) *(v+i) = Gaussiana(0,sigma);
	// Esto de acá me resta el valor medio de la velocidad, para que no se estén moviendo
	// Todas juntas como un paquete
	for(k=0;k<3;k++) for(i=0;i<N;i++) *(vprom+k) += *(v+3*i+k)/N;
	for(k=0;k<3;k++) for(i=0;i<N;i++) *(v+3*i+k) -= *(vprom+k);
	return 0;
}

// Esta función te calcula la fuerza entre dos partículas
double pair_force(double r2,double rc2,double *f_mod){
	double potij,r12,r6,rc12,rc6;
	if(r2<=rc2){
		r6 = r2*r2*r2;
		r12 = r6*r6;
		rc6 = rc2*rc2*rc2;
		rc12 = rc6*rc6;
		potij = 4*(1/r12-1/r6-1/rc12+1/rc6);
		*f_mod = 24*(2*(1/(r12*r2))-1/(r6*r2));
		return potij;
	}
	else *f_mod = 0;
	return 0; 
}

// Esta función te calcula la fuerza total que sufre cada partícula. Te returnea el potencial
double fuerzas(double *x, double *f,double *Lut_V, double *Lut_F,double dr, double rc, int N, double L){
	int i,j,k;
	double r,potij,potot=0;
	double *f_mod;
	f_mod = (double*) malloc(1*sizeof(double));
	double *Dx;
	Dx = (double*) malloc(3*sizeof(double));
	double *x1;
	x1 = (double*) malloc(3*sizeof(double));
	double *x2;
	x2 = (double*) malloc(3*sizeof(double));
	for(i=0;i<3*N;i++) *(f+i) = 0;
	for(i=0;i<N;i++){
		for(k=0;k<3;k++) *(x1+k) = *(x+3*i+k);
		for(j=i+1;j<N;j++){
			for(k=0;k<3;k++) *(x2+k) = *(x+3*j+k);
			delta_x(x1,x2,L,Dx);
			r = Norma(Dx);
			if(r<=rc){
				potij = interpolar_Luts(Lut_V,Lut_F,r,dr,f_mod);
				potot += potij;
				for(k=0;k<3;k++){
					*(f+i*3+k) += *(Dx+k)*(*f_mod);
					*(f+j*3+k) += *(Dx+k)*(-(*f_mod));
				}
			}
		}
	}
	return potot;
}

// Esta función calcula la Temperatura del sistema
// Acordate que trabajamos con unidades reducidas
double Temperatura(double *v,double L,int N){
	int i;
	double T,promvx=0,promvy=0,promvz=0;
	for(i=0;i<N;i++){
		promvx += (*(v+3*i)*(*(v+3*i)))/(float)N;
		promvy += (*(v+3*i+1)*(*(v+3*i+1)))/(float)N;
		promvz += (*(v+3*i+2)*(*(v+3*i+2)))/(float)N;
	}
	T = (1.0/3.0)*(promvx+promvy+promvz);
	return T;
}

// Esta función toma datos de las tablas y calcula las fuerzas y el potencial interpolándolas
double interpolar_Luts(double *Lut_V,double *Lut_F,double r,double dr,double *f_mod){
	int i;
	double potij,k,vi,vj,fi,fj,ri,m,b;
	k = r/dr;
	if(k>=1){
		i = (int) floor(k);
		ri = i*dr;
		vi = *(Lut_V+i-1);
		vj = *(Lut_V+i);
		m = (vj-vi)/dr;
		b = vi-m*ri;
		potij = m*r+b;
		//Lo de arriba es potencial, lo de abajo fuerza
		fi = *(Lut_F+i-1); //Acordate que la fuerza y el potencial en el radio están corridos
		fj = *(Lut_F+i); // para ri = 0.001, les corresponde la ubicación cero
		m = (fj-fi)/dr;
		b = fi-m*ri;
		*f_mod = m*r+b;
	}
	else{
		potij = *Lut_V;
		*f_mod = *Lut_F;
	}
	return potij;
}

int correlacion(double *energia,int pasos,int j){
	double vmc,vcm,vik,suma1,suma2,suma3;
	int i,k,medcorr;
	medcorr = pasos/4;
	double *corr;
	corr = (double*) malloc(medcorr*sizeof(double));
	for(i=0;i<medcorr;i++) *(corr+i)=0;
	vmc = 0;
	vcm = 0;
	suma1 = 0;
	suma2 = 0;
	for(i=0;i<pasos;i++) suma1 += *(energia+i);
	for(i=0;i<pasos;i++) suma2 += (*(energia+i))*(*(energia+i));
	vmc = (1.0/(float) pasos)*(1.0/(float) pasos)*suma1*suma1;
	vcm = (1.0/(float) pasos)*suma2;
	for(k=0;k<medcorr;k++){
        vik = 0;
		suma3 = 0;
		for(i=0;i<pasos-k;i++) suma3 += (*(energia+i+k))*(*(energia+i));
		vik = (1.0/(float) (pasos-k))*suma3;
		*(corr+k) = (vik-vmc)/(vcm-vmc);
	}
	char filename[255];
	if(j==1) sprintf(filename,"Correlacion_EnergiaC.txt");
	if(j==0) sprintf(filename,"Correlacion_EnergiaP.txt");
	FILE *fp=fopen(filename,"w");
	for(k=0;k<medcorr;k++) fprintf(fp,"%lf\t",*(corr+k));
	fclose(fp);
	free(corr);
	return 0;
}


//Esto es un esquema de como usar el lammpstraj para visualizar cosas
// Tenemos hasta el viernes que viene. Gotta get rolling baby

/*
inicializas(...)
save_lammpstrj(....., 0) // Inicial
for(f=0; f<Nframs;f++){
for(s=0;s<Nskip;s++){
avanzas(..)
}
save_lammpstrj(....., (f+1)*Nskip)
}
*/

