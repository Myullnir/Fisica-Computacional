// En este archivo me defino las funciones que inicializan, pero no las declaro

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "general.h"
#include "inicializar.h"


// Esta función me genera la red del sistema, asignándole a cada usuario sus F valores para cada tema
int GenerarR(struct Red Var){
	for(int i=0;i<Var.N2*Var.N2*Var.F;i++) Var.red[i] = (int) floor(Var.Q*Random()*0.9999); // Inicializo mi red
	return 0;
}

// Esta función se encarga de poner la condición de cadena cerrada en red
int CCP(struct Red Var){
	// Esto se encarga de las filas
	for(int i=0;i<Var.N2*Var.F;i++) {
		Var.red[i] = Var.red[Var.N2*Var.F*(Var.N2-2)+i]; // Pido primer fila fantasma igual a la anteúltima
		Var.red[Var.N2*Var.F*(Var.N2-1)+i] = Var.red[Var.N2*Var.F+i]; //Pido última fila fantasma igual a la segunda
	}
	// Esto se encarga de las columnas
	for(int i=0;i<Var.N2;i++){
		for(int j=0;j<Var.F;j++){
			// Pido que las F primeras columnas fantasma sean iguales a las Anteúltimas F Columnas.
			Var.red[Var.N2*Var.F*i+j] = Var.red[Var.N2*Var.F*(i+1)-2*Var.F+j];
			// Pido que las F últimas columnas sean iguales a las segundas F Columnas.
			Var.red[Var.N2*Var.F*(i+1)-Var.F+j] = Var.red[Var.N2*Var.F*i+Var.F+j];
		}
	}
	return 0;
}

// Esta función es para observar la red, en caso de ser necesario
int Visualizar(struct Red Var){
	for(int i=0;i<Var.N2;i++){
		for(int j=0;j<Var.N2*Var.F;j++) printf("%d ",Var.red[i*Var.N2*Var.F+j]);
		printf("\n");
	}
	return 0;
}

