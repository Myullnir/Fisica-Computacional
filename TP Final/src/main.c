// Acá viene el TP FInal. Voy a tener que empezar con un modelo de Axelrod simple

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"general.h"
#include"inicializar.h"
#include"avanzar.h"


int main(){
	// Empecemos con la base. Defino variables de tiempo para medir cuanto tardo y cosas básicas
	time_t prin,fin;
	time(&prin);
	srand(time(NULL));
	// Tengo que armar una red bidimensional cuadrada. Partamos de algo fácil, 10*10
	// También tengo que definir el largo de mi vector cultural F y el espectro de valores Q
	int p1,p2,tardanza; // N2 es el largo de mi red + 2. Mi red es de N2*N2. La idea es tener filas y columnas extra que cumplan CCP.
	double proba_int;
	struct Red datos;
	datos.N2 = 50; // Largo de la red
	datos.Q = 5; // Cantidad de opiniones
	datos.F = 4; // Cantidad de temas sobre los que opinar
	datos.Tpers = (datos.N2-2)*(datos.N2-2);
	datos.red = (int*) malloc(datos.N2*datos.N2*datos.F*sizeof(int)); // Array unidimensional que simula la red, con F casilleros para cada individuo
	GenerarR(datos);
	CCP(datos);
	// printf("Esta es la red inicial \n");
	// Visualizar(datos);
	for(int i = 0; i<datos.N2*datos.N2*datos.F*500; i++){
		p1 = (int) floor((datos.Tpers)*0.9999*Random()); // Este es el elemento activo. Este convence a la segunda persona
		// Pero eso sería el número de persona. Yo quiero el índice que ocupa en el puntero datos.red
		p1 = Ubicar(datos,p1);
		p2 = Vecino(datos,p1); // Con esto elijo un vecino al azar de p1. Esto es la posicion del primer elemento del vecino
		proba_int = PI(datos,p1,p2);
		if(Random() < proba_int) Imitar(datos,p1,p2);
		CCP(datos);
	}
	// printf("Esta es la red al final del proceso \n");
	// Visualizar(datos);
	/*
	Esta sección es la que me arma un archivo y me printea los datos.
	Tendría que ver si puedo armar un programa que haga esto y listo
	Aunque creo que la última vez que lo intenté no resultó muy bien.
	*/
	//################################################################
	char filename[255];
	sprintf(filename,"Red_F=%d_Q=%d.txt",datos.F,datos.Q);
	FILE *fp=fopen(filename,"w");
	// No tomemos toda la red, sólo la parte real
	for(int i=0; i<datos.N2-2; i++){
		for(int j=0; j<(datos.N2-2)*datos.F; j++) fprintf(fp,"%d\t",datos.red[j+(i+1)*datos.N2*datos.F+datos.F]);
		fprintf(fp,"\n");
	}
	fclose(fp);
	//#################################################################
	time(&fin);
	tardanza = fin-prin;
	printf("Tarde %d segundos\n",tardanza);
	free(datos.red);
	return 0;
}

