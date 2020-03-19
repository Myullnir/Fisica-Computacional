// En este archivo defino todas las funciones de que evolucionan el sistema, pero no las declaro.
// El <stdio.h>, o <math.h>, o <stdlib.h>, ¿Son necesarios?

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "general.h"
#include "inicializar.h"
#include "avanzar.h"


// Esta función ubica a la persona 1. Es decir, me da la ubicación en la red del elemento activo.
int Ubicar(struct Red Var, int p1){
	int ubicacion,filas_completas;
	filas_completas = p1/(Var.N2-2); // Esto es la cantidad de filas que avancé hasta llegar a la de p1
	/* Esta cuenta parece rara, la obtuve analizando la red que se me forma, el detalle está en el cuaderno.
	En resumen, el primer término son las posiciones necesarias para recorrer las filas completas. El Var.F
	Es por las primeras F columnas fantasma. El segundo término es la cantidad de lugares que hay que moverse
	En la fila donde está p1 luego de haber cruzado la primer columna fantasma.*/
	ubicacion = Var.N2*Var.F*(filas_completas+1)+(p1-filas_completas*(Var.N2-2))*Var.F+Var.F;
	return ubicacion;
}


// Esta función me calcula la probabilidad de interacción de dos individuos
double PI(struct Red Var, int p1, int p2){
	double proba,coincidencia = 0;
	// Este for se fija en cuantos temas la p1 coincide con la p2.
	for(int i=0;i<Var.F;i++) if(Var.red[p1+i]==Var.red[p2+i]) coincidencia++;
	proba = coincidencia/Var.F;
	return proba;
}

// Esta función me cambia los valores de un individuo con respecto al otro
int Imitar(struct Red Var, int p1, int p2){
	int j=-1,iaux,icambio;
	int *indices;
	indices = (int*) malloc(Var.F*sizeof(int));
	for(int i=0;i<Var.F;i++) *(indices+i) = 0; // Inicializo el puntero indices
	// Este for revisa cuales elementos cumplen que la opininón de la p1 es
	// menor que la de p2 en ese tema, y me registra el índice en el puntero indices
	for(int i=0;i<Var.F;i++) if(Var.red[p1+i]<Var.red[p2+i]){
		j++;
		*(indices+j) = i;
	}
	// Si algun valor cumple, entonces hago el cambio. Sino, sigo de largo.
	if(j!=-1) {
		iaux = (int) floor((j+1)*0.9999*Random()); // Esta cosa rara es para que sea aleatorio.
		icambio = *(indices+iaux); 
		// Preguntar una forma más fácil de hacer eso.
		Var.red[p1+icambio] = Var.red[p2+icambio];
	}
	return 0;
}

// Esta función me elige un vecino del elemento activo
int Vecino(struct Red Var, int p1){
	int p2;
	// Voy a elegir un vecino. Como la red es cuadrada, tengo 4 primeros vecinos.
	// Voy a asignarle un número a cada vecino. Arriba es 0, Abajo es 1, Izquierda es 2 y Derecha es 3.
	int decision = (int) floor(4*0.9999*Random());
	switch(decision){
		case 0 :
			// printf("Elegi el vecino de Arriba \n");
			p2 = p1-Var.N2*Var.F;
			break;
		case 1 :
			// printf("Elegi el vecino de Abajo \n");
			p2 = p1+Var.N2*Var.F;
			break;
		case 2 :
			// printf("Elegi el vecino de Izquierda \n");
			p2 = p1-Var.F;
			break;
		case 3 :
			// printf("Elegi el vecino de Derecha \n");
			p2 = p1+Var.F;
			break;
		default :
			p2 = 0;
	}
	return p2;
}