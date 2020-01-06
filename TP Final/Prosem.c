//Este es el archivo para testeos

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// El struct Red tiene los datos que definen mi red, y la info de la red
struct Red{
	int N2; // Largo de la red +2. Ese 2 extra es para tener dos filas y dos columnas que cumplan CCP. 
	int F; // Cantidad de temas sobre los que opinar
	int Q; // Cantidad de opiniones
	int Tpers; // Cantidad total de personas en la red
	int *red; // Array unidimensional que simula la red, con F casilleros para cada individuo
};

// Acá voy a declarar las funciones.------------------------------------

double Random();
int GenerarR(struct Red Var);
double PI(struct Red Var, int p1, int p2);
int imitar(struct Red Var, int p1, int p2);
int Ubicar(struct Red Var, int p1);
int CCP(struct Red Var);
int Visualizar(struct Red Var);




/*####################################################################################
####################################################################################
####################################################################################
####################################################################################
*/


//----------------------------------------------------------------------------------------------------

// Acá voy a probar las funciones PI e imitar
// Para eso voy a usar una red de 3*3, con 4 temas y 3 opiniones posibles

int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int p1,tardanza;
	struct Red datos;
	datos.N2 = 5;
	datos.Q = 3;
	datos.F = 2;
	datos.Tpers = (datos.N2-2)*(datos.N2-2);
	datos.red = (int*) malloc(datos.N2*datos.N2*datos.F*sizeof(int));
	GenerarR(datos);
	CCP(datos);
	// Hasta acá la inicialización base, me generé la red. Ahora a probar las funciones.
	// Primero tomo una persona al azar
	p1 = (int) floor((datos.Tpers)*0.9999*Random()); // Este es el elemento activo. Este convence a la segunda persona
	// Pero eso sería el número de persona. Yo quiero el índice que ocupa en el puntero datos.red
	p1 = Ubicar(datos,p1);
	Visualizar(datos);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar\n",tardanza);
	free(datos.red);
	return 0;
}

//----------------------------------------------------------------------------------------------------

// Esto era la prueba del struct. Ya vimos que funca bárbaro
/*
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,j=0,tardanza;
	for(i=0;i<10;i++) j++;
	struct Prueba papa;
	papa.entero = 10;
	papa.flotante = -13.5;
	papa.puntero=(int*) malloc(10*sizeof(int));
	for(i=0;i<10;i++) papa.puntero[i]=i;
	printf("El valor entero es %d y el flotante es %f\n",papa.entero,papa.flotante);
	for(i=0;i<10;i++) printf("El valor en la posicion %d del puntero es %d\n",i,papa.puntero[i]);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("El valor de j es %d \n",j);
	printf("Tarde %d en terminar\n",tardanza);
	// free(papa.puntero);
	return 0;
}
*/

//----------------------------------------------------------------------------------------------------

// Probé variables y acá funcionan, en el otro lado no
/*
int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,tardanza,N2=10;
	int Prueba[10];
	for(i=0;i<N2;i++) Prueba[i] = i;
	for(i=0;i<N2;i++) printf("Prueba[%d]: %d\n",i,Prueba[i]);
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	return 0;
}
*/

/*####################################################################################
####################################################################################
####################################################################################
####################################################################################
*/

// De acá en adelante van las funciones a declarar

// Esta función me genera un número random
double Random(){
	return ((double) rand()/(double) RAND_MAX);
}

// Esta función me genera la red del sistema, asignándole a cada usuario sus F valores para cada tema
int GenerarR(struct Red Var){
	for(int i=0;i<Var.N2*Var.N2*Var.F;i++) Var.red[i] = (int) floor(Var.Q*Random()*0.9999); // Inicializo mi red
	return 0;
}

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
	int coincidencia=0;
	double proba;
	// Este for se fija en cuantos temas la p1 coincide con la p2.
	for(int i=0;i<Var.F;i++) if(Var.red[p1+i]==Var.red[p2+i]) coincidencia++;
	proba = coincidencia/Var.F;
	return proba;
}

// Esta función me cambia los valores de un individuo con respecto al otro
int imitar(struct Red Var, int p1, int p2){
	int j=-1,iaux,icambio;
	int *indices;
	indices = (int*) malloc(Var.F*sizeof(int));
	for(int i=0;i<Var.F;i++) *(indices+i) = 0; // Inicializo el puntero indices
	// Este for revisa cuales elementos cumplen que la opininón de la p1 es
	// menor que la de p2 en ese tema, y me registra el índice en el puntero indices
	for(int i=0;i<Var.F;i++) if(Var.red[p1+i]<=Var.red[p2+i]){
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

// Esta función se encarga de poner la condición de cadena cerrada en red
int CCP(struct Red Var){
	// Esto se encarga de las filas
	for(int i=0;i<Var.N2*Var.F;i++) {
		Var.red[i] = Var.red[Var.N2*Var.F*(Var.N2-2)+i]; // Pido primer fila fantasma igual a la anteúltima
		Var.red[Var.N2*Var.F*(Var.N2-1)+i] = Var.red[Var.N2*Var.F+i]; //Pido última fila fantasma igual a la segunda
	}
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