// Vamos a armar una tabla con los ns acá

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


/* Acá van las funciones necesarias para armar la red, clasificar los clusters y ver si percola
Así como también los parámetros que vas a necesitar*/
int poblar(int *red, float p, int dim, int semilla);
int clasificar(int *red,int *historial, int dim);
int etiqueta_falsa(int *red, int dim, int *historial, int s1, int s2, int i, int j);
int repaso(int *red, int *historial, int dim);
int conteo(int *red, int *historial,int *taman,int *canti, int dim);
int percolaP(int *red, int *taman,int *percolantes,int dim);


// Acá está la función principal del programa

int main(){
	time_t prin,fin,semilla; //Me defino mis variables que van a tomar el tiempo de la máquina
	int dim=128,nmed,i,j,k; //Defino mis parámetros del problema, como la dimensión o valores iterables
	float p; //Esta es la probabilidad
	nmed = (0.6200-0.5399)*10000; //Este es el número de probabilidades que voy a analizar, yendo desde 0.54 hasta 0.62 de a 0.0001
    char filename[255]; //Este es el handle de mi archivo para guardar datos
    //for(dim=8;dim<=32;dim=dim*2){
    	sprintf(filename,"Datos_ns_L=%d.txt",dim);
    	FILE *fp=fopen(filename,"w");
    	fprintf(fp,"prob\t");
    	for(j=1;j<dim*dim;j++) fprintf(fp,"s%d\t",j); //Esto me pone los números en la primer fila
    	fprintf(fp,"\n");
        for(i=0;i<nmed;i++){ //Estas son todas la probabilidades que voy a analizar
			float *ns;
			ns = (float*) malloc(dim*dim*sizeof(float));
    		time(&prin);
            p = 0.54+i*0.0001; 
    		fprintf(fp,"%f\t",p);
    		for(j=0;j<dim*dim;j++) *(ns+j)=0; // Esto me vuelve a cero los ns
    		for(k=0;k<30000;k++){
    			semilla = time(NULL)+i+k;
    			int *red;
    			red = (int*) malloc(dim*dim*sizeof(int));
    			int *historial;
    			historial = (int*) malloc((((dim*dim)/2)+1)*sizeof(int));
    			int *taman;
    			taman = (int*) malloc((((dim*dim)/2)+1)*sizeof(int));
    			int *canti;
    			canti = (int*) malloc(dim*dim*sizeof(int));
    			int *percolantes;
    			percolantes = (int*) malloc((dim/2+1)*sizeof(int));
    			for(j=0;j<dim/2+1;j++) *(percolantes+j)=0;
    			for(j=0;j<(((dim*dim)/2)+1);j++){
    				*(historial+j)=j;
    				*(taman+j)=0;
    			}
    			for(j=0;j<dim*dim;j++) *(canti+j)=0; // Excepto por el vector red, inicialice todos mis vectores
    			poblar(red,p,dim,semilla);
    			clasificar(red,historial,dim);
    			repaso(red,historial,dim);	
    			conteo(red,historial,taman,canti,dim);
    			percolaP(red,taman,percolantes,dim);
    			for(j=2;j<dim/2+1;j++) if(*(percolantes+j)) (*(canti+*(taman+j)))--; //Este tipo me resta TODOS los clusters que hayan percolado
    			for(j=1;j<dim*dim;j++) *(ns+j)=*(ns+j)+(float)*(canti+j)/30000.0;
    			free(red);
    			free(historial);
    			free(taman);
    			free(canti);
    			free(percolantes);
    		}
    		for(j=1;j<dim*dim;j++) fprintf(fp,"%f\t",*(ns+j));
    		fprintf(fp,"\n");
    		time(&fin);
    		printf("Tarde %ld segundos en completar la probabilidad %f \n",(fin-prin),p);
    		free(ns);
    	}
	fclose(fp);
    //}
	system("PAUSE");
	return 0;
}

/* Definamos poblar*/
int poblar(int *red, float p, int dim, int semilla)
	{int i;
	float alea;
	srand(semilla);
	for(i=0; i<dim*dim; i++){
		alea = (float)rand()/(float) RAND_MAX;
		if(alea<p) *(red+i)=1; /*Esto me puebla los lugares con probabilidad p*/
		else *(red+i)=0; //Esto me coloca un cero si no pudo poner un 1
	}
	return 0;
}

/*Definamos una funcion para clasificar la red*/
int clasificar(int *red,int *historial, int dim){
	int frag,s1,s2,i,j;
	frag = 2;
	// Resolvamos primer elemento
	if (*red) {
		*red=frag;
		frag++;
	}
	// Resolvamos la primer fila
	for(i=1; i<dim; i++) {
		if (*(red+i)) {
			s2 = *(red+i-1);
			if (s2>0) *(red+i) = s2;
			else {
				*(red+i)=frag;
				frag++;
			}
		}
	}
	//Listo la primera, veamos el resto
	//Hagamos un doble for para resolver la primer columna
	for(i=1;i<dim;i++) {
		//las i son las filas, la j las columnas
		for(j=0;j<dim;j++) {
			if(*(red+i*dim+j)) {
				if (j==0) {
					s1=*(red+dim*(i-1)); // El de arriba
					if(s1>0) *(red+i*dim)=s1;
					else {
						*(red+i*dim)=frag;
						frag++;
					}
				}//Acá termina el if para estar sobre la primer columna
				else {	
					// Izquierda no-cero, arriba cero				
					s1=*(red+(i-1)*dim+j);// El de arriba
					s2=*(red+i*dim+j-1); // El de la izquierda
					if(s1==0 && s2>0) *(red+i*dim+j)=s2;
					// Izquierda cero, Ariba no-cero
					if(s2==0 && s1>0) *(red+i*dim+j)=s1;
					// Izquierda cero, arriba cero
					if(s1==0 && s2==0) {
						*(red+i*dim+j)=frag;
						frag++;
					}
					// Caso de conflicto
					if(s1>0 && s2>0) etiqueta_falsa(red, dim, historial, s1, s2, i, j);
				} // Esto es el trabajo fuera de la columna
			} //Este es el if para ver si hay un 1 en la red
		} //Acá terminan las j
	} //Acá terminan las i
	return 0;
} //Acá termina clasificar

// Armemos el programa que resuelve los casos de conflicto
int etiqueta_falsa(int *red, int dim, int *historial, int s1, int s2, int i, int j){
	int minimo,maximo;
	while (*(historial + s1)<0) s1 = - (*(historial+s1)); // Esto revisa el historial y lleva el s1 al valor que realmente le corresponde a ese cluster
	while (*(historial + s2)<0) s2 = - (*(historial+s2)); // Esto revisa el historial y lleva el s2 al valor que realmente le corresponde a ese cluster
	// El de la arriba es el más chico		
	if (s1<s2) {
		minimo = s1;
		maximo = s2;
	}
	else {
		minimo = s2;
		maximo = s1;
	}
	*(red+i*dim+j) = minimo;
	*(historial+maximo) = -minimo;
	*(historial+minimo) = minimo;
	return 0;
} // Acá termina la etiqueta falsa

// Armemos el programa que repasa la red y recoloca las etiquetas correctas
int repaso (int *red, int *historial, int dim){
	int i;
	// Primero revisemos el historial y pongámosle el número que le corresponde
	for(i=2;i<((dim*dim)/2)+1;i++) if (*(historial+i)<0) *(historial+i) = *(historial-(*(historial+i))); // Si el nùmero es negativo, armo a cambio, que es el tipo que voy a meter para cambiar la etiqueta
	// Ya revisé el historial y le reajusté todos los numeros. Ahora revisemos mi matriz
	for(i=0;i<dim*dim;i++) *(red+i) = *(historial+*(red+i)); //Esto a cada tipo le coloca el que le corresponde segun el historial
	return 0;
} //Acá termina la funciòn Repaso

// Armemos una función que cuente la cantidad de clusters de cada tamaño que tengo
int conteo(int *red, int *historial,int *taman,int *canti, int dim){
	int i;
	for(i=0;i<(dim*dim/2+1);i++) *(taman+i)=0;
    for(i=0;i<dim*dim;i++) (*(taman+(*(red+i))))++; // Esto te arma un vector con el tamaño de cada cluster
	// Y fijate que el cluster cero, que son los vacios, te va a dar algo muy grande seguro
	*taman = 0; // Con esto corregís eso, para no contar como cluster a los vacíos
    for(i=0;i<(dim*dim/2+1);i++) (*(canti+(*(taman+i))))++;
	return 0;
}

// Esta función que revisa si percola devuelve directamente el valor de P, la intesidad del cluster percolante
int percolaP(int *red,int *taman,int *percolantes, int dim){
	int i,j,k=0,largo,clusterperc=0;
	for(i=dim-1;i>-1;i--) if(*(red+i)>0){
			largo = *(red+i)+1;
			break;
		}
		else if(i==0) return 0; // La idea es que si no hay cluster en la primer fila, directamente no percola
	for(i=dim*(dim-1);i<dim*dim;i++) for(j=2;j<largo;j++) if(*(red+i)==j) (*(percolantes+j))++;
	return 0;
}

