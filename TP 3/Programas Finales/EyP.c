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
	int i,j,k,l,iteraciones,intentos,N=512,pasos=300,termal=500,desco=20,Nlut,tardanza;
	double rho,T,T2,crit,rc=2.5,h=0.001,dr=0.001,L,cinet,potot,presi,Tp,Ep,Pp;
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
	for(l=0;l<5;l++){ //Estoy haciendo pruebas de tiempo, pero el l tiene que ser menor a cinco
		rho = 0.4+l*0.1;
		sprintf(filename,"Datos_Energiapresion_D=%lf.txt",rho);
		FILE *fp=fopen(filename,"w");
		fprintf(fp,"Temperatura\tEnergia\tPresion\n");
		for(k=0;k<33;k++){
			T = 0.4+k*0.05;
			Tp = 0;
			Ep = 0;
			Pp = 0;
			for(i=0;i<3*N;i++) *(x+i) = 0;
			for(i=0;i<3*N;i++) *(v+i) = 0;
			for(i=0;i<3*N;i++) *(f+i) = 0;
			// Ahora seteo las condiciones iniciales
			L = set_x(x,rho,N);
			set_v(v,T,N);
			// Lo siguiente es termalizar
			fuerzas(x,f,Lut_V,Lut_F,dr,rc,N,L);
			for(i=0;i<termal;i++) velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
			// Acá corrijo la temperatura para llevarla a la T deseada
			T2 = Temperatura(v,L,N);
			crit = T-T2;
			if(crit<0) crit = -crit;
			//fprintf(fp,"Estoy deseando llegar a la temperatura %lf\n",T);
			intentos = 1;
			//Con esto corrijo la T para tener el valor esperado
			while(crit>0.01){
				iteraciones = correcion_T(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc,T);
				//fprintf(fp,"En el intento %d hice %d iteraciones\n",intentos,iteraciones);
				intentos++;
				T2 = Temperatura(v,L,N);
				crit = T-T2;
				if(crit<0) crit = -crit;
			}
			//fprintf(fp,"Al final fueron %d intentos\n",intentos); 
			//intentos = 0;
			// Ahora sí mido
			for(i=0;i<pasos;i++){
				for(j=0;j<desco;j++) potot = velocity_verlet(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc);
				cinet = ener_cinetica(v,N);
				T2 = Temperatura(v,L,N);
				presi = presion(x,Lut_V,Lut_F,rho,L,dr,rc,T2,N);
				Tp += T2/pasos;
				Ep += (potot+cinet)/pasos;
				Pp += presi/pasos;
				crit = T-T2;
				if(crit<0) crit = -crit;
				//Con esto corrijo la T para tener el valor esperado
				if(crit>0.01) correcion_T(x,v,f,Lut_V,Lut_F,dr,N,h,L,rc,T);
			}
			//fprintf(fp,"Durante la medicion hizo %d correcciones extra\n",intentos);
			fprintf(fp,"%lf\t%lf\t%lf\n",Tp,Ep,Pp);
		}
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
	return 0;
}


