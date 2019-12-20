// Voy a hacer el punto 1c de MD acá


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"general.h"
#include"inicializar.h"
#include"avanzar.h"


int main(){
	// En esta parte defino variables e inicializo vectores
	time_t prin,fin;
	time(&prin);
	int i,j,l,N=512,pasos=2500,termal=1000,desco=20,Nlut,lg,tardanza;
	double rho,T=1.5,T2,crit,rc=2.5,h=0.001,dr=0.001,deltar=0.05,L;
	Nlut = (int) ceil(rc/dr);
	double *x;
	x = (double*) malloc(3*N*sizeof(double));
	double *v;
	v = (double*) malloc(3*N*sizeof(double));
	double *f;
	f = (double*) malloc(3*N*sizeof(double));
	double *Lut_V;
	Lut_V = (double*) malloc(Nlut*sizeof(double));
	double *Lut_F;
	Lut_F = (double*) malloc(Nlut*sizeof(double));
	for(i=0;i<Nlut;i++) *(Lut_V+i) = 0;
	for(i=0;i<Nlut;i++) *(Lut_F+i) = 0;
	build_LUTS(Lut_V,Lut_F,dr,rc);
	char filename[255];
	for(l=0;l<3;l++){ 
		rho = 0.6+l*0.2;
		sprintf(filename,"Datos_gr_D=%lf.txt",rho);
		FILE *fp=fopen(filename,"w");
		fprintf(fp,"Radio\tG(r)\n");
		// Está pasando con la posición
		for(i=0;i<3*N;i++) *(x+i) = 0;
		for(i=0;i<3*N;i++) *(v+i) = 0;
		for(i=0;i<3*N;i++) *(f+i) = 0;
		// Ahora seteo las condiciones iniciales
		L = set_x(x,rho,N);
		set_v(v,T,N);
		// Como el gr lo calculo hasta L/2, recién acá puedo declararlo
		lg = (int) ceil(L/(2.0*deltar));
		double *gr;
		gr = (double*) malloc(lg*sizeof(double));
		double *grP;
		grP = (double*) malloc(lg*sizeof(double));
		for(i=0;i<lg;i++) *(gr+i) = 0;
		for(i=0;i<lg;i++) *(grP+i) = 0;
		// Lo siguiente es termalizar
		fuerzas(x,f,Lut_V,Lut_F,dr,rc,N,L);
		for(i=0;i<termal;i++) velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
		// Acá corrijo la temperatura para llevarla a la T deseada
		T2 = Temperatura(v,L,N);
		crit = T-T2;
		if(crit<0) crit = -crit;
		//Con esto corrijo la T para tener el valor esperado
		while(crit>0.01){
			correcion_T(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc,T);
			T2 = Temperatura(v,L,N);
			crit = T-T2;
			if(crit<0) crit = -crit;
		}
		// Ahora sí mido
		for(i=0;i<pasos;i++){
			for(j=0;j<desco;j++) velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
			T2 = Temperatura(v,L,N);
			crit = T-T2;
			if(crit<0) crit = -crit;
			//Con esto corrijo la T para tener el valor esperado
			if(crit>0.01) correcion_T(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc,T);
			distg(x,gr,L,rho,deltar,N,lg);
			for(j=0;j<lg;j++) *(grP+j) += *(gr+j)/pasos;
		}
		for(i=0;i<lg;i++) fprintf(fp,"%lf\t%lf\n",(i+1)*deltar,*(grP+i));
		free(gr);
		free(grP);
		sprintf(filename,"Datos_posicion_gr_D=%lf.txt",rho);
		FILE *ft=fopen(filename,"w");
		fprintf(ft,"X\tY\tZ\n");
		for(i=0;i<N;i++) fprintf(ft,"%lf\t%lf\t%lf\n",*(x+i*3),*(x+i*3+1),*(x+i*3+2));
		fclose(ft);
		fclose(fp);
	}
	//printf("La energía potencial del sistema es %lf\n",potot);
	time(&fin);
	tardanza = fin-prin;
	printf("Tarde %d segundos en hacer MD con %d pasos\n",tardanza,pasos);
	free(x);
	free(v);
	free(f);
	free(Lut_V);
	free(Lut_F);
	return 0;
}

