// Vamos a resolver el punto 3 acá

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
int conteo(int *red, int *historial,int *taman, int dim);
int percola(int *red, int dim);
int percolaP(int *red, int *taman, int dim);
/*Acá está la función principal del programa*/
int main()
	{time_t prin;
	time_t fin;
	time_t semilla; //Preguntar si este tipo es un int u otra cosa
	int intensidadP,dim,m,s,j;
	int larprom=27000;
	float p;
	float pc;
	float correcion;
	time(&prin);
	char filename[255];
	for(dim=5;dim<10;dim=dim+2){
		float *regpc;
		regpc=(float*) malloc(larprom*sizeof(float));
		sprintf(filename,"Datos_M_dim=%d.txt",dim);
		FILE *fp=fopen(filename,"w");
		pc=0;
		m=0;
		correcion = 0.5;
		while(m<larprom){ //Esto es para armar el vector regpc con el cual hallar el pc para cada dimension
			s=0;
			p=0.5;
			semilla = time(NULL)+m;
			while(s<15){ //Esto es para hallar un pc con un 2**(-15) de correcion
				s++;
				int *red;
				red=(int*) malloc(dim*dim*sizeof(int));
				int *historial;
				historial=(int*) malloc((((dim*dim)/2)+1)*sizeof(int));
				int *taman;
				taman=(int*) malloc((((dim*dim)/2)+1)*sizeof(int));
				int i;
				for(i=0; i<((dim*dim)/2)+1; i++)
					{*(historial+i)=i;
				}
				poblar(red,p,dim,semilla);
				clasificar(red,historial,dim);
				repaso(red,historial,dim);	
				conteo(red,historial,taman,dim);
				intensidadP = percolaP(red,taman,dim);
				correcion = correcion/2;
				if(intensidadP){
					p = p-correcion;
				}
				else{
					p = p+correcion;
				}
				free(red);
				free(historial);
				free(taman);
			}
			*(regpc+m) = p;
			m=m+1;
		}
		// La idea es que regpc tiene un registro de todos los pc, para 27000 casos diferentes
		// Después los promedio esos valores para hallar un pc para cada L
		for(j=0;j<larprom;j++){
			pc = pc+*(regpc+j)/larprom;
		}
		free(regpc);
		s=0;
		m=0;
		while(m<larprom){
			semilla = time(NULL)+s;
			s++;
			int *red;
			red=(int*) malloc(dim*dim*sizeof(int));
			int *historial;
			historial=(int*) malloc((((dim*dim)/2)+1)*sizeof(int));
			int *taman;
			taman=(int*) malloc((((dim*dim)/2)+1)*sizeof(int)); // Quise acotarlo, pero creo que lo que me ahorro en memoria lo pierdo en tiempo de iteracion
			int i;
			for(i=0; i<((dim*dim)/2)+1; i++)
				{*(historial+i)=i;
			}
			poblar(red,pc,dim,semilla);
			clasificar(red,historial,dim);
			repaso(red,historial,dim);	
			conteo(red,historial,taman,dim);
			intensidadP = percolaP(red,taman,dim);
			if(intensidadP){
				m++;
				fprintf(fp,"%d\t",intensidadP); 
			}
			free(red); //La idea es que en este punto ya guarde la probabilidad y ya termine una iteracion. Sino poner free red, historial, taman y canti.
			free(historial);
			free(taman);
		}
		time(&fin);
		fclose(fp);
		printf("Tarde %ld segundos en completar la red de tamano %d x %d \n",(fin-prin),dim,dim);
    }
    system("PAUSE");
	return 0;
}

/* Definamos poblar*/
int poblar(int *red, float p, int dim, int semilla)
	{int i;
	float alea;
	srand(semilla);
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
	for(i=0; i<dim; i=i+1)
		{for(j=0; j<dim; j=j+1)
			{printf("%d ",*(red+dim*i+j));
			}
		printf("\n");
		}
	for(i=0;i<((dim*dim)/2)+1;i++){
		printf("%d ", *(historial+i));
		}
	printf("\n");
	printf("\n");
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
int conteo(int *red, int *historial,int *taman, int dim){
	int i;
	for(i=0;i<(dim*dim/2+1);i++) {
    *(taman+i)=0;
    }
    for(i=0;i<dim*dim;i++) (*(taman+(*(red+i))))++;
    //for(i=0;i<(dim*dim/2+1);i++) (*(canti+(*(taman+i))))++;
	/*for(i=0;i<dim*dim;i++){
		if(*(red+i)){
			*(taman+*(red+i))= *(taman+*(red+i))+1;
			}
		} //Con esto Me armé el vector taman con el tamaño de cada cluster
	for(i=0;i<((dim*dim)/2)+1;i++){
		*(canti+*(taman+i))= *(canti+*(taman+i))+1;
		} //Con esto Me armé el vector canti que me dice la cantidad de clusters de cada tamaño*/
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
	return salida;
}

// Esta función que revisa si percola devuelve directamente el valor de P, la intesidad del cluster percolante
int percolaP(int *red,int *taman, int dim){
	int i,j,k,nop,largo,pinf;
	pinf = 0;
	nop = 1;
	for(i=dim-1;i>-1;i--){
		if(*(red+i)>0){
			largo = *(red+i)+1;
			break;
		}
		else if(i==0){
			nop = 0; // La idea es que si no hay cluster en la primer fila, directamente no percola
		}
	}
	k = 0;
	if(nop){
		for(i=dim*(dim-1);i<dim*dim;i++){
			if(*(red+i)>0 && k==0){
				for(j=2;j<largo;j++){
					if(*(red+i)==j){
						k = j;
						break;
					}
				}
			}
		}
		pinf = *(taman+k);
	}
	return pinf;
}
