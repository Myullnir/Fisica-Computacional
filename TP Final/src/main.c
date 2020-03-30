// Acá viene el TP FInal. Voy a tener que empezar con un modelo de Axelrod simple

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"general.h"
#include"inicializar.h"
#include"avanzar.h"


int main(int argc, char *argv[]){
	// Empecemos con la base. Defino variables de tiempo para medir cuanto tardo y cosas básicas
	time_t prin,fin;
	time(&prin);
	srand(time(NULL));
	// Tengo que armar una red bidimensional cuadrada. Partamos de algo fácil, 10*10
	// También tengo que definir el largo de mi vector cultural F y el espectro de valores Q
	int p1,p2,tardanza,posi; // N2 es el largo de mi red + 2. Mi red es de N2*N2. La idea es tener filas y columnas extra que cumplan CCP.
	double proba_int,S;
	struct Red datos;
	datos.Q = 5; // Cantidad de opiniones
	datos.F = 4; // Cantidad de temas sobre los que opinar
	datos.L = (int) floor(log10(datos.Q-1));
	datos.N2 = strtol(argv[1],NULL,10)+2; // Largo de la red, ahora lo hice para que tome input de la línea de comando
	datos.Tpers = (datos.N2-2)*(datos.N2-2);
	datos.red = (int*) malloc(datos.N2*datos.N2*datos.F*sizeof(int)); // Array unidimensional que simula la red, con F casilleros para cada individuo
	GenerarR(datos);
	CCP(datos);
	// Con esto armé mi nueva red de tamaño d+2.
	char filename[255];
	sprintf(filename,"Termalizacion_Red_N=%d.txt",datos.N2-2);
	FILE *fp=fopen(filename,"w");
	S = 0;
	for(int j=0; j<datos.N2-2; j++){
		for(int l=0; l<datos.N2-2; l++){
			posi = (j+1)*datos.N2*datos.F+datos.F+l*datos.F;
			S = S + Etiquetar(datos,posi);
		}
	}
	fprintf(fp,"%lf\t",S);
	for(int i = 0; i<datos.N2*datos.F*500; i++){
		for(int k = 0; k<datos.N2; k++){
			p1 = (int) floor((datos.Tpers)*0.9999*Random()); // Este es el elemento activo. Este convence a la segunda persona
			// Pero eso sería el número de persona. Yo quiero el índice que ocupa en el puntero datos.red
			p1 = Ubicar(datos,p1);
			p2 = Vecino(datos,p1); // Con esto elijo un vecino al azar de p1. Esto es la posicion del primer elemento del vecino
			proba_int = PI(datos,p1,p2);
			if(Random() < proba_int) Imitar(datos,p1,p2);
			CCP(datos);
		}
		S = 0;
		for(int j=0; j<datos.N2-2; j++){
			for(int l=0; l<datos.N2-2; l++){
				posi = (j+1)*datos.N2*datos.F+datos.F+l*datos.F;
				S = S + Etiquetar(datos,posi);
			}
		}
		fprintf(fp,"%lf\t",S);
	}
	fclose(fp);
	free(datos.red);
	time(&fin);
	tardanza = fin-prin;
	printf("Tarde %d segundos en guardar los datos de la red de tamano %d\n",tardanza,datos.N2-2);
	return 0;
}

