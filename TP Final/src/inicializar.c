// En este archivo me defino las funciones que inicializan, pero no las declaro

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "general.h"
#include "inicializar.h"


// Esta función me genera la red del sistema, asignándole a cada usuario sus F valores para cada tema
int GenerarR(struct Red Var){
	int i;
	for(i=0;i<Var.N2*Var.N2*Var.F;i++) Var.red[i] = (int) floor(Var.Q*Random()*0.9999); // Inicializo mi red
	return 0;
}