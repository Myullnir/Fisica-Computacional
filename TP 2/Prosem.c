#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


int poblar(int *grilla, int l);
int Concon(int *grilla, int l);
int imprimir(int *red, int dim);
int Nuevospin(int *grilla,int l,int *li);


int main(){
	int i;
	int *li;
	li = (int*) malloc(1*sizeof(int));
	for(i=0;i<1000;i++) *(li+i) = 0;
	printf("Todo bien");
	return 0;
}

//----------------------------------------------------------
// Con esta funciÃ³n se puebla la red

int poblar(int *grilla, int l){
	int i;
	float alea;
	for(i=0;i<l*l;i++){
		alea = ((float)rand()/(float) RAND_MAX);
		if(alea<=0.5) *(grilla+i)=1;
		else *(grilla+i)=-1;
	}
	return 0;
}

//---------------------------------------------------------------------------
// Esta funcion me aplica condiciones de contorno a mi red. Son condiciones periódicas
// l es el tamaño de la grilla con filas y columnas fantasma, no es el de la grilla que deseo simular

int Concon(int *grilla, int l){
	int i;
	for(i=1;i<l-1;i++){
		*(grilla+i) = *(grilla+(l-2)*l+i); //En primer fila pongo la anteúltima
		*(grilla+(l-1)*l+i) = *(grilla+l+i); //En última fila pongo la segunda
		*(grilla+l*i) = *(grilla+l-2+l*i); // En primera columna pongo la anteúltima
		*(grilla+l-1+l*i) = *(grilla+1+l*i); // En última columna pongo la segunda
	}
	return 0;
}

/* Definamos una función para visualizar la red*/
int imprimir(int *red, int dim){
	int i,j;
	//printf("Estoy imprimiendo\n");
	for(i=0;i<dim;i=i+1){
		for(j=0; j<dim; j=j+1) printf("%d\t",*(red+dim*i+j));
		printf("\n");
	}
	return 0;
}

//-----------------------------------------------
// Con esta funciÃ³n elijo un nuevo spin

int Nuevospin(int *grilla,int l,int *li){
	int s,f=0,c=0;
	float alea;
	while(f==0){
		alea = ((float)rand()/(float) RAND_MAX);
		f = round(alea*(l-2));
	}
	while(c==0){
		alea = ((float)rand()/(float) RAND_MAX);
		c = round(alea*(l-2));
	}
	*li = f*l+c+1;
	s = *(grilla+*li);
	return s;
}