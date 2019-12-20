//Este es el archivo para testeos

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int main(){
	time_t prin,fin,semilla;
	time(&prin);
	semilla = time(NULL);
	srand(semilla);
	int i,tardanza;
	for(i=0;i<21;i++) printf("%f se redondea a %f\n",-0.510+i*0.001,round(-0.510+i*0.001));
	time(&fin);
	tardanza = (int) (fin-prin);
	printf("Tarde %d en terminar de armar la lista de datos\n",tardanza);
	return 0;
}