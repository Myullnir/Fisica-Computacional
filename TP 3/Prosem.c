//Este es el archivo para testeos

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

float Random();
float Gaussiana(float mu, float sigma);
double Norma(double *x);
int delta_x(double *x1, double *x2, double L, double *Dx);
double set_x(double *x,float rho, int N);
int set_v(double *v,float T, int N);
double pair_force(double r2,double rc2,double *f_mod);
double fuerzas(double *x, double *f, double rc2, int N, double L);
double fuerzas2(double *x, double *f,double *Lut_V, double *Lut_F,double dr, double rc2, int N, double L);
int fstep(double *x,double *v,double *f,double L,double rho,double h,int N);
int sstep(double *v,double *f,double L,double rho,double h,int N);
double velocity_verlet(double *x,double *v,double *f,int N,double h,double L,double rho,double rc2);
int apply_PBC(double *x,double L,double N);
int build_LUTS(double *Lut_V,double *Lut_F,double dr,double rc2,int tope);
double interpolar_Luts(double *Lut_V,double *Lut_F,double r,double dr,double *f_mod);
double ener_cinetica(double *v,int N);
double presion(double *x, double *Lut_V, double *Lut_F, double rho, double L,double dr, double rc, double T2, int N);
double Temperatura(double *v,double L,int N);
double distg(double *x,double *gr,double L,double deltar,int N,int lg);


// Probemos una boludez sobre operaciones lógicas
/*
int main(){
	int i;
	for(i=0;i<20;i++) if(7<i && i<15) printf("%d\n",i);
	return 0;
}*/




//Vamos a probar la distg. Vamos a hacerlo sobre el sistema en condiciones iniciales

int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,tardanza,lg;
	double L,rho=0.8442,N=512,deltar=0.01;
	double *x;
	x = (double*) malloc(3*N*sizeof(double));
	for(i=0;i<3*N;i++) *(x+i) = 0;
	L = set_x(x,rho,N);
	lg = (int) ceil(L/(2.0*deltar));
	double *gr;
	gr = (double*) malloc(lg*sizeof(double));
	distg(x,gr,L,deltar,N,lg);
	char filename[255];
	sprintf(filename,"Testeo_gr.txt");
	FILE *fp=fopen(filename,"w");
	fprintf(fp,"Radio\tg(r)\n");
	for(i=0;i<lg;i++) fprintf(fp,"%lf\t%lf\n",deltar*(i+1),*(gr+i));
	fclose(fp);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	free(x);
	free(gr);
	return 0;
}




//Quiero probar el delta_x. Ok, definitivamente va desde la segunda partícula hacia la primera
/*
int main(){
	int i,L=10;
	double *x1;
	x1 = (double*) malloc(3*sizeof(double));
	double *x2;
	x2 = (double*) malloc(3*sizeof(double));
	double *Dx;
	Dx = (double*) malloc(3*sizeof(double));
	for(i=0;i<3;i++){
		*(x1+i) = i;
		*(x2+i) = i;
	}
	*x1 = 2;
	delta_x(x1,x2,L,Dx);
	printf("%.0lf\t%.0lf\t%.0lf",*Dx,*(Dx+1),*(Dx+2));	
	return 0;
}*/


// Vamos a probar el que mide la energía Cinética, porque me da exactamente igual siempre
/*int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,tardanza,tope;
	double dr=0.001,rc=2.5,rc2,T=0.728,L,rho=0.8442,N=512,pasos=20000,h=0.001,potot;
	rc2 = rc*rc;
	tope = (int) ceil(rc/dr);
	double *x;
	x = (double*) malloc(3*N*sizeof(double));
	double *v;
	v = (double*) malloc(3*N*sizeof(double));
	double *f;
	f = (double*) malloc(3*N*sizeof(double));
	double *Lut_V;
	Lut_V = (double*) malloc(tope*sizeof(double));
	double *Lut_F;
	Lut_F = (double*) malloc(tope*sizeof(double));
	for(i=0;i<3*N;i++) *(x+i) = 0;
	for(i=0;i<3*N;i++) *(v+i) = 0;
	for(i=0;i<tope;i++) *(Lut_V+i) = 0;
	for(i=0;i<tope;i++) *(Lut_F+i) = 0;
	L = set_x(x,rho,N);
	set_v(v,T,N);
	build_LUTS(Lut_V,Lut_F,dr,rc2,tope);
	for(i=0;i<pasos;i++){
		velocity_verlet(x,v,f,N,h,L,rho,rc2);
		
	}
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	free(Lut_V);
	free(Lut_F);
	free(x);
	free(f);
	return 0;
}*/


// Voy a probar ahora el interpolar_Lut. Para eso tengo que armar una segunda función fuerzas2
// Para empezar dan lo mismo. Debería ver que si la posición no es tan simétrica, dan lo mismo
// Funciona perfecto. El fuerzas 2 va a ser el furzas que voy a usar con las LUTS.
// Este mismo test lo voy a usar para probar la funcion presion

/*
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,tardanza,tope;
	double dr=0.001,rc=2.5,T2,rc2,T=0.728,L,rho=0.8442,N=512,termal=1000,pasos=500,h=0.001,presi;
	rc2 = rc*rc;
	tope = (int) ceil(rc/dr);
	double *x;
	x = (double*) malloc(3*N*sizeof(double));
	double *v;
	v = (double*) malloc(3*N*sizeof(double));
	double *f;
	f = (double*) malloc(3*N*sizeof(double));
	double *Lut_V;
	Lut_V = (double*) malloc(tope*sizeof(double));
	double *Lut_F;
	Lut_F = (double*) malloc(tope*sizeof(double));
	for(i=0;i<3*N;i++) *(x+i) = 0;
	for(i=0;i<3*N;i++) *(v+i) = 0;
	for(i=0;i<tope;i++) *(Lut_V+i) = 0;
	for(i=0;i<tope;i++) *(Lut_F+i) = 0;
	L = set_x(x,rho,N);
	set_v(v,T,N);
	build_LUTS(Lut_V,Lut_F,dr,rc2,tope);
	for(i=0;i<termal;i++) velocity_verlet(x,v,f,N,h,L,rho,rc2);
	char filename[255];
	sprintf(filename,"Testeo_presion.txt");
	FILE *fp=fopen(filename,"w");
	for(i=0;i<pasos;i++){
		velocity_verlet(x,v,f,N,h,L,rho,rc2);
		T2 = Temperatura(v,L,N);
		presi = presion(x,Lut_V,Lut_F,rho,L,dr,rc,T2,N);
		fprintf(fp,"%lf\t",presi);
	}
	//potot = fuerzas(x,f,rc2,N,L);
	//fprintf(fp,"Potencial\t%lf\n",potot);
	//fprintf(fp,"Fuerzas\t");
	//for(i=0;i<3*N;i++) fprintf(fp,"%lf\t",*(f+i));
	//fprintf(fp,"\n");
	//potot = fuerzas2(x,f,Lut_V,Lut_F,dr,rc,N,L);
	//fprintf(fp,"Potencial\t%lf\n",potot);
	//fprintf(fp,"Fuerzas\t");
	//for(i=0;i<3*N;i++) fprintf(fp,"%lf\t",*(f+i));
	fclose(fp);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	free(Lut_V);
	free(Lut_F);
	free(x);
	free(f);
	return 0;
}
*/

// Voy a probar los LUTS. Bien, funcionan bárbaro
/*
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,tardanza,tope;
	double dr=0.001,rc=2.5,rc2=6.25;
	tope = (int) ceil(rc/dr);
	double *Lut_V;
	Lut_V = (double*) malloc(tope*sizeof(double));
	double *Lut_F;
	Lut_F = (double*) malloc(tope*sizeof(double));
	for(i=0;i<tope;i++) *(Lut_V+i) = 0;
	for(i=0;i<tope;i++) *(Lut_F+i) = 0;
	build_LUTS(Lut_V,Lut_F,dr,rc2,tope);
	char filename[255];
	sprintf(filename,"Testeo_LUTS.txt");
	FILE *fp=fopen(filename,"w");
	fprintf(fp,"Potencial\n");
	for(i=0;i<tope;i++) fprintf(fp,"%lf\t",*(Lut_V+i));
	fprintf(fp,"\n");
	fprintf(fp,"Fuerza\n");
	for(i=0;i<tope;i++) fprintf(fp,"%lf\t",*(Lut_F+i));
	fclose(fp);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	free(Lut_V);
	free(Lut_F);
	return 0;
}
*/


// Vamos a probar el verlet, porque algo mal está haciendo
// Al final el problema parece que estaba en el sstep, porque había una operación mal

/*
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,tardanza,N=512;
	double rc2=6.25,L,T=0.728,rho=0.8442,h=0.001,pasos=50000;
	double *x;
	x = (double*) malloc(3*N*sizeof(double));
	double *v;
	v = (double*) malloc(3*N*sizeof(double));
	double *f;
	f = (double*) malloc(3*N*sizeof(double));
	for(i=0;i<3*N;i++) *(x+i) = 0;
	for(i=0;i<3*N;i++) *(v+i) = 0;
	for(i=0;i<3*N;i++) *(f+i) = 0;
	char filename[255];
	sprintf(filename,"Testeo_pasos_verlet.txt");
	FILE *fp=fopen(filename,"w");
	L = set_x(x,rho,N);
	set_v(v,T,N);
	fprintf(fp,"Posicion inicial \n");
	for(i=0;i<3*N;i++) fprintf(fp,"%lf\t",*(x+i));
	fprintf(fp,"\n");
	fprintf(fp,"Velocidad inicial \n");
	for(i=0;i<3*N;i++) fprintf(fp,"%lf\t",*(v+i));
	fprintf(fp,"\n");
	for(i=0;i<pasos;i++) velocity_verlet(x,v,f,N,h,L,rho,rc2);
	fprintf(fp,"Posicion final \n");
	for(i=0;i<3*N;i++) fprintf(fp,"%lf\t",*(x+i));
	fprintf(fp,"\n");
	fprintf(fp,"Velocidad final \n");
	for(i=0;i<3*N;i++) fprintf(fp,"%lf\t",*(v+i));
	fclose(fp);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	free(x);
	free(v);
	free(f);
	return 0;
}
*/


// Quiero probar el +=
/*
int main(){
	int i,pp=0;
	for(i=0;i<10;i++) pp += i;
	printf("%d",pp);
	return 0;
}
Funciona perfecto*/


// Acá probé el pair_force
/* 
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	int i,j,k,l,tardanza,N=512,n=8;
	double r2,rc2=6.25,potij,L;
	double *f_mod;
	f_mod = (double*) malloc(1*sizeof(double));
	double *muestreo;
	muestreo = (double*) malloc(3*N*sizeof(double));
	double *x1;
	x1 = (double*) malloc(3*sizeof(double));
	double *x2;
	x2 = (double*) malloc(3*sizeof(double));
	double *Dx;
	Dx = (double*) malloc(3*sizeof(double));
	double *fuerzas;
	fuerzas = (double*) malloc(3*N*sizeof(double));
	semilla = time(NULL);
	srand(semilla);
	for(i=0;i<3;i++) *(Dx+i) = 0;
	for(l=0;l<3*N;l++) *(muestreo+l) = 0;
	for(l=0;l<3*N;l++) *(fuerzas+l) = 0;
	L = set_x(muestreo,0.85,N);
	char filename[255];
	sprintf(filename,"Testeo_pair_force1.txt");
	FILE *fp=fopen(filename,"w");
	fprintf(fp,"Fuerza\tPotencial\n");
	for(i=0;i<N;i++){
		for(k=0;k<3;k++) *(x1+k) = *(muestreo+3*i+k);
		for(j=i+1;j<N;j++){
			for(k=0;k<3;k++) *(x2+k) = *(muestreo+3*j+k);
			delta_x(x1,x2,L,Dx);
			r2 = Norma(Dx);
			potij = pair_force(r2,rc2,f_mod);
			fprintf(fp,"%lf\t",*f_mod);
			fprintf(fp,"%lf\t",potij);
			for(k=0;k<3;k++){
				*(fuerzas+i*3+k) += *(Dx+k)*(*f_mod);
				*(fuerzas+j*3+k) += *(Dx+k)*(-(*f_mod));
			}
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	// Con esto vamo a ver las fuerzas, dadas las condiciones periódicas, todo esto deberìa ser uniforme, porque empieza uniforme
	//char filename[255];
	sprintf(filename,"Testeo_pair_force2.txt");
	FILE *ft=fopen(filename,"w");
	for(i=0;i<n;i++){
		fprintf(ft,"Cara %d\n",i);
		for(j=0;j<n;j++){
			for(k=0;k<n;k++) fprintf(ft,"%lf\t",*(fuerzas+64*i+8*j+k));
			fprintf(ft,"\n");
		}
	}
	fclose(ft);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	free(muestreo);
	free(fuerzas);
	free(x1);
	free(x2);
	free(Dx);
	free(f_mod);
	return 0;
}*/
//---------------------------------------------------------------------------------------------------------

//Esto lo usé para probar el pair_force. Funciona bien, creo
/*
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	int i,j,k,l,tardanza,N=512,n=8;
	double r2,rc2=6.25,potij,L;
	double *f_mod;
	f_mod = (double*) malloc(1*sizeof(double));
	double *muestreo;
	muestreo = (double*) malloc(3*N*sizeof(double));
	double *x1;
	x1 = (double*) malloc(3*sizeof(double));
	double *x2;
	x2 = (double*) malloc(3*sizeof(double));
	double *Dx;
	Dx = (double*) malloc(3*sizeof(double));
	double *fuerzas;
	fuerzas = (double*) malloc(3*N*sizeof(double));
	semilla = time(NULL);
	srand(semilla);
	for(i=0;i<3;i++) *(Dx+i) = 0;
	for(l=0;l<3*N;l++) *(muestreo+l) = 0;
	for(l=0;l<3*N;l++) *(fuerzas+l) = 0;
	L = set_x(muestreo,0.85,N);
	char filename[255];
	sprintf(filename,"Testeo_pair_force1.txt");
	FILE *fp=fopen(filename,"w");
	fprintf(fp,"Fuerza\tPotencial\n");
	for(i=0;i<N;i++){
		for(k=0;k<3;k++) *(x1+k) = *(muestreo+3*i+k);
		for(j=i+1;j<N;j++){
			for(k=0;k<3;k++) *(x2+k) = *(muestreo+3*j+k);
			delta_x(x1,x2,L,Dx);
			r2 = Norma(Dx);
			potij = pair_force(r2,rc2,f_mod);
			fprintf(fp,"%lf\t",*f_mod);
			fprintf(fp,"%lf\t",potij);
			for(k=0;k<3;k++){
				*(fuerzas+i*3+k) += *(Dx+k)*(*f_mod);
				*(fuerzas+j*3+k) += *(Dx+k)*(-(*f_mod));
			}
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	// Con esto vamo a ver las fuerzas, dadas las condiciones periódicas, todo esto deberìa ser uniforme, porque empieza uniforme
	//char filename[255];
	sprintf(filename,"Testeo_pair_force2.txt");
	FILE *ft=fopen(filename,"w");
	for(i=0;i<n;i++){
		fprintf(ft,"Cara %d\n",i);
		for(j=0;j<n;j++){
			for(k=0;k<n;k++) fprintf(ft,"%lf\t",*(fuerzas+64*i+8*j+k));
			fprintf(ft,"\n");
		}
	}
	fclose(ft);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	free(muestreo);
	free(fuerzas);
	free(x1);
	free(x2);
	free(Dx);
	free(f_mod);
	return 0;
}
*/




//---------------------------------------------------------------------------------------------------------

/* Esto lo usé para probar el set_x después de cambiarlo. Funciona bien
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	int l,tardanza,N=512;
	double L;
	double *muestreo;
	muestreo = (double*) malloc(3*N*sizeof(double));
	semilla = time(NULL);
	srand(semilla);
	for(l=0;l<3*N;l++) *(muestreo+l) = 0;
	L = set_x(muestreo,0.85,N);
	char filename[255];
	sprintf(filename,"Testeo_set_x.txt");
	FILE *fp=fopen(filename,"w");
	for(l=0;l<3*N;l++) fprintf(fp,"%lf\t",*(muestreo+l));
	fclose(fp);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	printf("El largo de la red es %lf\n",L);
	free(muestreo);
	return 0;
}
*/







//--------------------------------------------------------------------------------------------------------

// Esta función me genera un número random
float Random(){
	return ((float) rand()/(float) RAND_MAX);
}

// Esta función me da un valor tomado de una distribución gaussiana con valor medio mu y desviación sigma
float Gaussiana(float mu, float sigma){
	int n=10,i;
	float z=0;
	for(i=0;i<n;i++) z += Random();
	z = sqrt(12*n) * (z/n-0.5);
	return z*sigma+mu;
}

// Esta función me calcula la norma de la distancia entre dos partículas
double Norma(double *x){
	double norm;
	norm = sqrt((*x)*(*x)+(*(x+1))*(*(x+1))+(*(x+2))*(*(x+2)));
	return norm;
}


// Esta función me calcula el vector desplazamiento entredos partículas
int delta_x(double *x1, double *x2, double L, double *Dx){
	int i;
	double *diferencias;
	diferencias = (double*) malloc(3*sizeof(double));
	for(i=0;i<3;i++) *(diferencias+i) = *(x1+i)-*(x2+i);
	for(i=0;i<3;i++) if(*(diferencias+i)<-0.5*L) *(diferencias+i) = *(diferencias+i)+L;
	for(i=0;i<3;i++) if(*(diferencias+i)>0.5*L) *(diferencias+i) = *(diferencias+i)-L;
	for(i=0;i<3;i++) *(Dx+i) = *(diferencias+i);
	free(diferencias);
	return 0;
}

// Esta función te setea las posiciones de ts partículas inicialmente
double set_x(double *x,float rho, int N){
	int i,j,k,l=0,n;
	double L,a;
	n = (int) floor(cbrt(N));
	L = cbrt(n*n*n/rho);
	a = L/n;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				*(x+3*l) = a/2+i*a;
				*(x+3*l+1) = a/2+j*a;
				*(x+3*l+2) = a/2+k*a;
				l++;
			}
		}
	}
	return L;
}

// Esta función te setea las velocidades de tus partículas inicialmente
int set_v(double *v,float T, int N){
	int i,j,k,l=0,n;
	float sigma;
	n = (int) floor(cbrt(N));
	sigma = sqrt(T/n);
	//L = (int) round(cbrt(n*n*n/rho));
	//a = L/n;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				*(v+3*l) = Gaussiana(0,sigma);
				*(v+3*l+1) = Gaussiana(0,sigma);
				*(v+3*l+2) = Gaussiana(0,sigma);
				l++;
			}
		}
	}
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
double fuerzas(double *x, double *f, double rc2, int N, double L){
	int i,j,k;
	double r,r2,potij,potot=0;
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
			r2 = r*r;
			potij = pair_force(r2,rc2,f_mod);
			potot += potij; //(Preguntar si tomar este al doble está bien)
			for(k=0;k<3;k++){
				*(f+i*3+k) += *(Dx+k)*(*f_mod);
				*(f+j*3+k) += *(Dx+k)*(-(*f_mod));
			}
		}
	}
	return potot;
}

// Esta función te calcula la fuerza total que sufre cada partícula. Te returnea el potencial. Ahora con las LUTS
double fuerzas2(double *x, double *f,double *Lut_V, double *Lut_F,double dr, double rc, int N, double L){
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
				potot += potij; //(Preguntar si tomar este al doble está bien)
				for(k=0;k<3;k++){
					*(f+i*3+k) += *(Dx+k)*(*f_mod);
					*(f+j*3+k) += *(Dx+k)*(-(*f_mod));
				}
			}
		}
	}
	return potot;
}

// Esta función te calcula el primer paso de verlet
int fstep(double *x,double *v,double *f,double L,double rho,double h,int N){
	int i,k;
	double m;
	m = rho*L*L*L;
	for(i=0;i<N;i++) for(k=0;k<3;k++) *(x+3*i+k) = *(x+3*i+k)+*(v+3*i+k)*h+(*(f+3*i+k)*h*h)/(2*m);
	return 0;
}

// Esta función te calcula el segundo paso de Verlet
int sstep(double *v,double *f,double L,double rho,double h,int N){
	int i,k;
	double m;
	m = rho*L*L*L;
	for(i=0;i<N;i++) for(k=0;k<3;k++) *(v+3*i+k) = *(v+3*i+k)+((h/2)*(*(f+3*i+k)/m));	
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
double velocity_verlet(double *x,double *v,double *f,int N,double h,double L,double rho,double rc2){
	double potot;
	fstep(x,v,f,L,rho,h,N);
	apply_PBC(x,L,N);
	sstep(v,f,L,rho,h,N);
	potot = fuerzas(x,f,rc2,N,L);
	sstep(v,f,L,rho,h,N);
	return potot;
}

// Esta función arma las tablas de datos
int build_LUTS(double *Lut_V,double *Lut_F,double dr2,double rc2,int tope){
	int i;
	for(i=1;i<=tope;i++) *(Lut_V+i-1) = pair_force(i*dr2*i*dr2,rc2,(Lut_F+i-1));
	return 0;
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

// Esta función te calcula la función de distribución radial g(r)
double distg(double *x,double *gr,double L,double deltar,int N,int lg){
	int i,k,l,suma1=0;
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


