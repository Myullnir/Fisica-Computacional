// Vamos a resolver el punto 1 acá

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

/*#define MU 0 //Esto es la media
#define VAR 3 //Esto es la varianza
#define PI 3.14159 //Esto es el valor de pi*/

float Distnormal(float x, float MU, float VAR,float PI);
float Nuevopaso(float x, float d);
float Metropolis(float xi, float xf, float pi, float pf,float *muestreo);
int Correlacion(float *muestreo, int tope, int lcacho, int medcorr, float d);

int main(){
	time_t semilla,prin,fin;
	float xi=1.0,xf,pi,pf,d,PI=3.14159,MU=0.0,VAR=1.0;
	int i,j,k,lcacho,nmed=2000000,tope,medcorr=1000,nd=80,tardanza;
	lcacho = medcorr*4;
	if(nmed/lcacho<100) tope = nmed/lcacho;
	else tope=100; 
	float *muestreo;
	muestreo = (float*) malloc(nmed*sizeof(float));
	float *distancias;
	distancias = (float*) malloc(nd*sizeof(float));
	for(i=0;i<nd;i++) *(distancias+i) = 0.05*(i+1);
	char filename[255];
	for(k=0;k<nd;k++){
		time(&prin);
		semilla = time(NULL);
		srand(semilla);
		d = *(distancias+k);
		sprintf(filename,"Datos_muestreo_d=%f.txt",d);
		FILE *fp=fopen(filename,"w");
		for(j=0;j<nmed;j++) *(muestreo+j)=0;
		for(i=0;i<nmed;i++){
			xf = Nuevopaso(xi,d);
			pi = Distnormal(xi,MU,VAR,PI);
			pf = Distnormal(xf,MU,VAR,PI);
			xi = Metropolis(xi,xf,pi,pf,muestreo);
			*(muestreo+i) = xi;
			//for(j=0;j<i;j++) if(*(muestreo+j)==xi) i--; Esto era para no tomar en cuenta cuando el tipo rechazaba el paso
		}
		for(j=0;j<nmed;j++) fprintf(fp,"%f\t",*(muestreo+j));
		fclose(fp);
		/*for(j=0;j<5;j++){
 			for(i=0;i<5;i++) printf("%f\t",*(muestreo+i+j*5));
			printf("\n");
		}*/
		Correlacion(muestreo,tope,lcacho,medcorr,d);
		time(&fin);
		tardanza= (int) fin-prin;
		printf("Tarde %d segundos en armar los datos de muestreo con d=%f \n",tardanza,d);
	}
	free(distancias);
	free(muestreo);
	time(&fin);
	tardanza= (int) fin-prin;
	printf("Tarde %d segundos en terminar el programa \n",tardanza);
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

//-----------------------------------------------------------------------
// Esta funcion arma el archivo con datos de correlacion

int Correlacion(float *muestreo, int tope,int lcacho,int medcorr, float d){
	float vmc,vcm,vik,suma1,suma2,suma3;
	int i,k,m;
	char filename[255];
	sprintf(filename,"Datos_Correlacion_d=%f.txt",d);
	FILE *fp=fopen(filename,"w");
	/*fprintf(fp,"med\t");
	for(k=0;k<medcorr;k++) fprintf(fp,"k%d\t",k);
	fprintf(fp,"\n");
	float *corr;
	corr = (float*) malloc(medcorr*sizeof(float));*/
	for(m=0;m<tope;m++){
		fprintf(fp,"%d\t",m);
		//for(i=0;i<medcorr;i++) *(corr+i)=0;
		vmc = 0;
		vcm = 0;
		suma1 = 0;
		suma2 = 0;
		for(i=m*lcacho;i<(m+1)*lcacho;i++) suma1 = suma1+*(muestreo+i);
		for(i=m*lcacho;i<(m+1)*lcacho;i++) suma2 = suma2+(*(muestreo+i))*(*(muestreo+i));
		vmc = (1.0/lcacho)*(1.0/lcacho)*suma1*suma1;
		vcm = (1.0/lcacho)*suma2;
		for(k=0;k<medcorr;k++){
			vik = 0;
			suma3 = 0;
			for(i=m*lcacho;i<(m+1)*lcacho-k;i++) suma3 = suma3+(*(muestreo+i+k))*(*(muestreo+i));
			vik = (1.0/(lcacho-k))*suma3;
			//*(corr+k) = (vik-vmc)/(vcm-vmc);
			fprintf(fp,"%f\t",(vik-vmc)/(vcm-vmc));
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 0;
}


