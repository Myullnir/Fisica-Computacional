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
	int i,N=1000,F=4,Q=10,tardanza; // N es el largo de mi red. Mi red es de N*N
	int *red;
	red = (int*) malloc(N*N*F*sizeof(int));
	RED(red,Q,F,N);
	char filename[255];
	sprintf(filename,"Red_prueba.txt");
	FILE *fp=fopen(filename,"w");
	for(i=0;i<N*N*F;i++) fprintf(fp,"%d\t",*(red+i));
	fclose(fp);
	time(&fin);
	tardanza = fin-prin;
	printf("Tarde %d segundos\n",tardanza);
	free(red);
	return 0;
}

