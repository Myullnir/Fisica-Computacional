// Acá viene el TP FInal. Voy a tener que empezar con un modelo de Axelrod simple

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"general.h"
#include"inicializar.h"


int main(){
	// Empecemos con la base. Defino variables de tiempo para medir cuanto tardo y cosas básicas
	time_t prin,fin;
	time(&prin);
	srand(time(NULL));
	// Tengo que armar una red bidimensional cuadrada. Partamos de algo fácil, 10*10
	// También tengo que definir el largo de mi vector cultural F y el espectro de valores Q
	int i,tardanza; // N2 es el largo de mi red + 2. Mi red es de N2*N2. La idea es tener filas y columnas extra que cumplan CCP.
	struct Red datos;
	datos.N2 = 502; // Largo de la red
	datos.Q = 9; // Cantidad de opiniones
	datos.F = 5; // Cantidad de temas sobre los que opinar
	datos.red = (int*) malloc(datos.N2*datos.N2*datos.F*sizeof(int)); // Array unidimensional que simula la red, con F casilleros para cada individuo
	GenerarR(datos);
	char filename[255];
	sprintf(filename,"Red_prueba.txt");
	FILE *fp=fopen(filename,"w");
	for(i=0;i<datos.N2*datos.N2*datos.F;i++) fprintf(fp,"%d\t",datos.red[i]);
	fclose(fp);
	time(&fin);
	tardanza = fin-prin;
	printf("Tarde %d segundos\n",tardanza);
	free(datos.red);
	return 0;
}

