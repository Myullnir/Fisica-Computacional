// Vamos a resolver el punto 1 acá

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

float Distnormal(float x, float MU, float VAR,float PI);
float Nuevopaso(float x, float d);
float Metropolis(float xi, float xf, float pi, float pf,float *muestreo);

int main(){
	time_t semilla=time(NULL),prin,fin;
	time(&prin);
	float xi=1.0,xf,pi,pf,d=1,PI=3.14159,MU=0,VAR=1;
	int i,j,nmed=100000;
	float *muestreo;
	muestreo = (float*) malloc(nmed*sizeof(float));
	srand(semilla);
	char filename[255];
	sprintf(filename,"Datos_muestreo_xi=%f_d=%f.txt",xi,d);
	FILE *fp=fopen(filename,"w");
	for(j=0;j<nmed;j++) *(muestreo+j)=0;
	for(i=0;i<nmed;i++){
		xf = Nuevopaso(xi,d);
		pi = Distnormal(xi,MU,VAR,PI);
		pf = Distnormal(xf,MU,VAR,PI);
		xi = Metropolis(xi,xf,pi,pf,muestreo);
		*(muestreo+i) = xi;
		//for(j=0;j<i;j++) if(*(muestreo+j)==xi) i--;
	}
	for(j=0;j<nmed;j++) fprintf(fp,"%f\t",*(muestreo+j));
	fclose(fp);
	/*for(j=0;j<10;j++){
 		for(i=0;i<10;i++) printf("%f\t",*(muestreo+i+j*10));
		printf("\n");
	}*/ // Esto es para printear los datos y ver lo que está haciendo el programa
	free(muestreo);
	time(&fin);
	printf("Tardé %ld segundos en terminar el programa \n",fin-prin);
	return 0;
}

// Con eso tengo hecho mi main, ahora necesito hacer mis funciones
//---------------------------------------------------------------

// Esta función es la que me elige el nuevo valor que voy a mirar
float Nuevopaso(float x,float d){
	float xf,alea;
	alea = ((float)rand()/(float) RAND_MAX)-0.5; // Como alea va entre 0 y 1, de esta manera va entre -0.5 y 0.5
	// Lo quiero simétrico para que se pueda mover tanto a izquierda como a derecha, sin favorecer ninguna dirección
	xf = x+alea*2*d; //El 2 es porque alea sólo llega a 0,5 y no sería entonces la distancia d que yo pretendo
	return xf;
}

//---------------------------------------------------------------
// Esta función le asigna la probabilidad de distribución normal a un valor de x

float Distnormal(float x,float MU,float VAR,float PI){
	float p;
	p = (1/(VAR*sqrtf(2*PI)))*expf(-((x-MU)*(x-MU))/(2*VAR*VAR));
	return p;
}

//---------------------------------------------------------------------
// Esta función aplica el método metrópolis

float Metropolis(float xi, float xf, float pi, float pf, float *muestreo){
	float pa,alea,xn;
	pa = pf/pi;
	if(pa>=1) xn=xf;
	else{
		alea = ((float)rand()/(float)RAND_MAX);
		if (alea<pa) xn=xf;
		else xn=xi;
	}
	return xn;
}

