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
	int i,j,k,N=512,pasos=1000,termal=1000,desco=20,Nlut,tardanza;
	double rho=0.8442,potot,T,T2,Tp,crit,rc=2.5,h=0.001,dr=0.001,L,cinet;
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
	double *energiac;
	energiac = (double*) malloc(pasos*sizeof(double));
	double *energiap;
	energiap = (double*) malloc(pasos*sizeof(double));
	char filename[255];
	for(k=0;k<41;k++){
		T = 0.6+k*0.005;
		Tp = 0;
		for(i=0;i<3*N;i++) *(x+i) = 0;
		for(i=0;i<3*N;i++) *(v+i) = 0;
		for(i=0;i<3*N;i++) *(f+i) = 0;
		for(i=0;i<Nlut;i++) *(Lut_V+i) = 0;
		for(i=0;i<Nlut;i++) *(Lut_F+i) = 0;
		for(i=0;i<pasos;i++) *(energiac+i) = 0;
		for(i=0;i<pasos;i++) *(energiap+i) = 0;
		// Ahora seteo las condiciones iniciales
		L = set_x(x,rho,N);
		set_v(v,T,N);
		build_LUTS(Lut_V,Lut_F,dr,rc);
		// Lo siguiente es termalizar
		fuerzas(x,f,Lut_V,Lut_F,dr,rc,N,L);
		for(i=0;i<termal;i++) velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
		// Ahora sí mido
		sprintf(filename,"Datos_Fluctuaciones_Energias_MD_T=%lf.txt",T);
		FILE *fp=fopen(filename,"w");
		fprintf(fp,"Potencial\tCinetica\n");
		for(i=0;i<pasos;i++){
			for(j=0;j<desco;j++) potot = velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
			cinet = ener_cinetica(v,N);
			T2 = Temperatura(v,L,N);
			fprintf(fp,"%lf\t%lf\n",potot,cinet);
			*(energiac+i) = cinet;
			*(energiap+i) = potot;
			Tp += T2/pasos;
		}
		fprintf(fp,"%lf",Tp);
		fclose(fp);
	}
	printf("La energía potencial del sistema es %lf\n",potot);
	time(&fin);
	tardanza = fin-prin;
	printf("Tarde %d segundos en hacer MD con %d pasos\n",tardanza,pasos);
	free(x);
	free(v);
	free(f);
	free(Lut_V);
	free(Lut_F);
	free(energiac);
	free(energiap);
	return 0;
}
