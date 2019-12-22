// En este archivo me defino las funciones que inicializan, pero no las declaro

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "general.h"
#include "inicializar.h"

int RED(int *red, int Q, int F, int N){
	int i;
	for(i=0;i<N*N*F;i++) *(red+i) = round((Q-0.000001)*Random()-0.4999994); // Inicializo mi red
	// Voy a armar un txt con los datos y los voy a cargar en Python para analizarlos
	return 0;
}