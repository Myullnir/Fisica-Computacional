// En este archivo defino todas las funciones de inicialización, pero no las declaro.
// El <stdio.h>, o <math.h>, o <stdlib.h>, ¿Son necesarios?

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "general.h"
#include "inicializar.h"
#include "avanzar.h"

// Esta función te calcula el primer paso de verlet
int fstep(double *x,double *v,double *f,double L,double h,int N){
	int i,k;
	for(i=0;i<N;i++) for(k=0;k<3;k++) *(x+3*i+k) = *(x+3*i+k)+*(v+3*i+k)*h+(*(f+3*i+k)*h*h)/2.0;
	return 0;
}

// Esta función te calcula el segundo paso de Verlet
int sstep(double *v,double *f,double L,double h,int N){
	int i,k;
	for(i=0;i<N;i++) for(k=0;k<3;k++) *(v+3*i+k) = *(v+3*i+k)+((h/2)*(*(f+3*i+k)));	
	return 0;
}

// Esta función te aplica condiciones de contorno periódicas (¿Podría pasar que se me escape en 2L la partícula?)
int apply_PBC(double *x,double L,double N){
	int i;
	for(i=0;i<3*N;i++){
		if(*(x+i)<0) *(x+i) = *(x+i)+L;
		else if(*(x+i)>L) *(x+i) = *(x+i)-L;
	}
	return 0;
}

// Esta función aplica el Velocity Verlet y evoluciona al sistema
// Consultar porque al final agregué un rho
double velocity_verlet(double *x,double *v,double *f,double *Lut_V,double *Lut_F, double dr,int N,double h,double L,double rc){
	double potot;
	fstep(x,v,f,L,h,N);
	apply_PBC(x,L,N);
	sstep(v,f,L,h,N);
	potot = fuerzas(x,f,Lut_V,Lut_F,dr,rc,N,L);
	sstep(v,f,L,h,N);
	return potot;
}

// Esta función arma las tablas de datos
int build_LUTS(double *Lut_V,double *Lut_F,double dr,double rc){
	int i,tope;
	double rc2;
	tope = (int) ceil(rc/dr);
	rc2 = rc*rc;
	for(i=1;i<=tope;i++) *(Lut_V+i-1) = pair_force(i*dr*i*dr,rc2,(Lut_F+i-1));
	return 0;
}

// Esta funci{on reescala las velocidades para que alcance la temperatura correcta
int correcion_T(double *x,double *v,double *f,double *Lut_V,double *Lut_F, double dr,int N,double h,double L,double rc,double T){
	int i,j,num=0,iteraciones,termal=15;
	double T2,dt=0.002,factor;
	// Esto es para ver que funcione correctamente, después lo saco
	/*char filename[255];
	sprintf(filename,"Datos_Temperatura_Termal%d_MD.txt",k);
	FILE *fp=fopen(filename,"w");*/
	T2 = Temperatura(v,L,N);
	if(T>T2){
		iteraciones = (int) ceil((T-T2)/dt);
		for(i=0;i<iteraciones;i++){
			num++;
			factor = sqrt((T2+dt)/T2);
			for(j=0;j<3*N;j++) *(v+j) = *(v+j)*factor;
			for(j=0;j<termal;j++){
				velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
				T2 = Temperatura(v,L,N);
				//fprintf(fp,"%lf\t",T2);
			}
		}
	}
	else{
		iteraciones = (int) ceil((T2-T)/dt);
		for(i=0;i<iteraciones;i++){
			num++;
			factor = sqrt((T2-dt)/T2);
			for(j=0;j<3*N;j++) *(v+j) = *(v+j)*factor;
			for(j=0;j<termal;j++){
				velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
				T2 = Temperatura(v,L,N);
				//fprintf(fp,"%lf\t",T2);
			}
		}
	}
	//fclose(fp);
	return num;
}

// Esto calcula la energía Cinética
double ener_cinetica(double *v,int N){
	int i;
	double energia=0;
	for(i=0;i<3*N;i++) energia += (*(v+i)*(*(v+i)))/2.0;
	return energia;
}

// Esta función calcula la Presión
double presion(double *x, double *Lut_V, double *Lut_F, double rho, double L,double dr, double rc, double T2, int N){
	int i,j,k,normal=0;
	double presion,fxd=0,r; // fxd es la suma de fuerza por distancia
	double *x1;
	x1 = (double*) malloc(3*sizeof(double));
	double *x2;
	x2 = (double*) malloc(3*sizeof(double));
	double *Dx;
	Dx = (double*) malloc(3*sizeof(double));
	double *f_mod;
	f_mod = (double*) malloc(1*sizeof(double));
	for(i=0;i<N;i++){
		for(k=0;k<3;k++) *(x1+k) = *(x+3*i+k);
		for(j=i+1;j<N;j++){
			for(k=0;k<3;k++) *(x2+k) = *(x+3*j+k);
			normal++;
			delta_x(x1,x2,L,Dx);
			r = Norma(Dx);
			if(r<=rc){
				interpolar_Luts(Lut_V,Lut_F,r,dr,f_mod);
				for(k=0;k<3;k++) fxd += -(*f_mod*(*(Dx+k))*(*(Dx+k)))/r;
			}
		}
	}		
	presion = rho*T2+fxd/(3*normal*L*L*L);
	free(x1);
	free(x2);
	free(Dx);
	free(f_mod);
	return presion;
}


// Esta función te calcula la función de distribución radial g(r)
double distg(double *x,double *gr,double L,double rho,double deltar,int N,int lg){
	int i,k,l,suma1;
	double pi=3.14159,r;
	double *x1;
	x1 = (double*) malloc(3*sizeof(double));
	double *x2;
	x2 = (double*) malloc(3*sizeof(double));
	double *Dx;
	Dx = (double*) malloc(3*sizeof(double));
	for(k=0;k<3;k++) *(x2+k) = L/2;
	for(l=0;l<lg;l++){
		suma1=0;
		for(i=0;i<N;i++){
			for(k=0;k<3;k++) *(x1+k) = *(x+3*i+k);
			delta_x(x1,x2,L,Dx);
			r = Norma(Dx);
			if(deltar*l<r && r<deltar*(l+1)) suma1 += 1;
		}
		*(gr+l) = (double) (suma1*2.0)/ (double) (N*4.0*pi*(l+1)*deltar*(l+1)*deltar*deltar);
	}
	return 0;
}
