// Vamos a resolver el punto 1 acá

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
/* Acá van las funciones necesarias para armar la red, clasificar los clusters y ver si percola
Así como también los parámetros que vas a necesitar*/
int poblar(int *red, float p, int dim, int semilla);
int imprimir(int *red, int *historial, int dim);
int clasificar(int *red,int *historial, int dim);
int etiqueta_falsa(int *red, int dim, int *historial, int s1, int s2, int i, int j);
int repaso(int *red, int *historial, int dim);
int conteo(int *red, int *historial,int *taman,int *canti, int dim);
int percola(int *red, int dim);
/*Acá está la función principal del programa*/
int main()
	{time_t prin;
	time_t fin;
	time_t semilla; //Preguntar si este tipo es un int u otra cosa
	//printf("Pasame dos números. El primer y segundo número que me pases son la dimension y la probabilidad\n");
	//printf("Te recomiendo que el primero sea entero y el segundo float\n");	
	int dim;
	int percolacion;
	int potencia;
	int n;
	int m;
	float p;
	float correcion;
	char filename[255];
	for(dim=6;dim<129;dim = dim+2){
		time(&prin);
		m=0;
		sprintf(filename,"Datos_Promediar_p_dimension_%d.txt",dim);
		FILE *fp=fopen(filename,"w");		
		while(m<27000){
			p=0.5;
			correcion = 0.5;
			m=m+1;
			semilla = time(NULL)+m;
			for(n=0;n<15;n++){ //¿Cuantas mediciones necesito tomar para asegurarme que estoy teniendo la precisiòn que quiero?
				int *red;
				red=(int*) malloc(dim*dim*sizeof(int));
				//printf("Arme Red\n");
				int *historial;
				historial=(int*) malloc((((dim*dim)/2)+1)*sizeof(int));
				int i;
				for(i=0; i<((dim*dim)/2)+1; i++)
					{*(historial+i)=i;
				}
				poblar(red,p,dim,semilla);
				clasificar(red,historial,dim);
				repaso(red,historial,dim);	
				percolacion = percola(red,dim);
				correcion = correcion/2;
				if(percolacion){
					p = p-correcion;
				}
				else{
					p = p+correcion;
				}
				//printf("Fijate que la salida de percola es %d\n", percolacion);
				free(red); //La idea es que en este punto ya guarde la probabilidad y ya termine una iteracion. Sino poner free red, historial, taman y canti.
				free(historial);	
			}
		fprintf(fp,"%f\t",p);
		}
		// Una vez terminada la medicion tengo que guardarme este p, para despues promediarlo
		fclose(fp);
		time(&fin);
		printf("Tarde  %ld segundos en completar la red de tamano %d x %d \n", (fin-prin),dim,dim);
	}
	system("PAUSE");
    return 0;
}

/* Definamos poblar*/
int poblar(int *red, float p, int dim, int semilla)
	{int i;
	float alea;
	srand(semilla);
	//printf("Estoy poblando la red\n");
	for(i=0; i<dim*dim; i++)
		{alea = (float)rand()/(float) RAND_MAX;
		if(alea<p)
			{*(red+i)=1; /*Esto me puebla los lugares con probabilidad p*/
			}
		else
			{*(red+i)=0; //Esto me coloca un cero si no pudo poner un 1
			}
		}
	return 0;
	}

/* Definamos una función para visualizar la red*/
int imprimir(int *red, int *historial, int dim)
	{int i,j;
	//printf("Estoy imprimiendo\n");
	for(i=0; i<dim; i=i+1)
		{for(j=0; j<dim; j=j+1)
			{printf("%d ",*(red+dim*i+j));
			}
		printf("\n");
		} //Con esto imprimo la matríz
	//printf("Ya te imprimí la matríz, ahora imprimamos el historial\n");
	for(i=0;i<((dim*dim)/2)+1;i++){
		printf("%d ", *(historial+i));
		}// Con esto imprimo el historial
	printf("\n");
	printf("\n");
	//printf("Ya te imprimi el historial\n");
	return 0;
	}

/*Definamos una funcion para clasificar la red*/
int clasificar(int *red,int *historial, int dim){
	int frag;
	int s1;
	int s2;
	int i,j;
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
			if (s2>0){
				*(red+i) = s2;
				}
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
					if(s1>0) {
						*(red+i*dim)=s1;
						} 
					else {
						*(red+i*dim)=frag;
						frag++;
						}
					}//Acá termina el if para estar sobre la primer columna
				else {	
					// Izquierda no-cero, arriba cero				
					s1=*(red+(i-1)*dim+j);// El de arriba
					s2=*(red+i*dim+j-1); // El de la izquierda
					if(s1==0 && s2>0) {*(red+i*dim+j)=s2;
						}
					// Izquierda cero, Ariba no-cero
					if(s2==0 && s1>0) {*(red+i*dim+j)=s1;
						}
					// Izquierda cero, arriba cero
					if(s1==0 && s2==0) {
						*(red+i*dim+j)=frag;
						frag++;
						}
					// Caso de conflicto
					if(s1>0 && s2>0) {
						etiqueta_falsa(red, dim, historial, s1, s2, i, j);
						}
					} // Esto es el trabajo fuera de la columna
					} //Este es el if para ver si hay un 1 en la red
				} //Acá terminan las j
		} //Acá terminan las i
	return 0;
	} //Acá termina clasificar
	
// Armemos el programa que resuelve los casos de conflicto
int etiqueta_falsa(int *red, int dim, int *historial, int s1, int s2, int i, int j){
	int minimo;
	int maximo;
	while (*(historial + s1)<0) {
		s1 = - (*(historial+s1));
		} // Esto revisa el historial y lleva el s1 al valor que realmente le corresponde a ese cluster
	while (*(historial + s2)<0) {
		s2 = - (*(historial+s2));
		}// Esto revisa el historial y lleva el s2 al valor que realmente le corresponde a ese cluster
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
	for(i=2;i<((dim*dim)/2)+1;i++){
		if (*(historial+i)<0){
			*(historial+i) = *(historial-(*(historial+i)));
			} // Si el nùmero es negativo, armo a cambio, que es el tipo que voy a meter para cambiar la etiqueta
		}
	// Ya revisé el historial y le reajusté todos los numeros. Ahora revisemos mi matriz
	for(i=0;i<dim*dim;i++){
		*(red+i) = *(historial+*(red+i)); //Esto a cada tipo le coloca el que le corresponde segun el historial
		}
	return 0;
	} //Acá termina la funciòn Repaso

// Armemos una función que cuente la cantidad de clusters de cada tamaño que tengo
int conteo(int *red, int *historial,int *taman,int *canti, int dim){
	int i;
	for(i=0;i<dim*dim;i++){
		if(*(red+i)){
			*(taman+*(red+i))= *(taman+*(red+i))+1;
			}
		} //Con esto Me armé el vector taman con el tamaño de cada cluster
	for(i=0;i<((dim*dim)/2)+1;i++){
		*(canti+*(taman+i))= *(canti+*(taman+i))+1;
		} //Con esto Me armé el vector canti que me dice la cantidad de clusters de cada tamaño
	/*for(i=1;i<((dim*dim)/2)+1;i++){
		printf("La cantidad de clusters de tamaño %d es %d \n",i,*(canti+i));
		}*/
	return 0;
	}
	
// Armemos la función para ver si percola
// La verdad no creo que esto sea muy eficiente
int percola(int *red,int dim){
	int i;
	int j;
	int nop;
	int salida;
	int largo;
	nop = 1;
	salida = 0;
	for(i=dim-1;i>-1;i=i-1){
		if(*(red+i)>0){
			largo = *(red+i)+1;
			/*printf("%d\n", largo);
			int *compa;
			compa = (int*) malloc(largo*sizeof(int));
			for(j=0;j<largo,j++){
				*(compa+j) = j;
			}*/ //Me di cuenta que compa no tenía sentido porque no necesitaba esos enteros
			break;
		}
		else if(i==0){
			nop = 0; // La idea es que si no hay cluster en la primer fila, directamente no percola
		}
	}/* La idea es leer la primera fila desde el final hasta el principio
	Y tomar el primer número que encuentre como el tamaño de compa.
	De esa manera, en Compa estarán todos los valores de clusters posibles
	que hay en la primer fila. Esos voy a tener que buscar al final*/
	//
	if(nop){
		for(i=dim*(dim-1);i<dim*dim;i++){
			if(*(red+i)>0){
				for(j=2;j<largo;j++){
					if(*(red+i)==j){
						salida = 1;
						break;
					}
				}
			}
			if(salida){
				break;
			}
		}
	}
	/*if(salida){
		printf("La red Percola \n"); 
	}
	else {
		printf("La red NO Percola \n");
	}*/
	return salida;
}

