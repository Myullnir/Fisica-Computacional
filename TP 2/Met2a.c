// Vamos a resolver el punto 2a acá

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int Nuevospin(int *grilla,int l,int *li);
int Metropolis(int s, float protran, float Be);
int poblar(int *grilla, int l);

int main(){
	time_t prin, fin, semilla=time(NULL);
	time(&prin);
	float Be,protran,enertemp,magtemp;
	int i,j,m,n,si,sf,l=32,nb=1000,nmed=500,termal=100000,desco=10000,tardanza; //Atento que quizás cambiar el nb te da problemas con campob
	int *li;
	li = (int*) malloc(1*sizeof(int));
	float *campob;
	campob = (float*) malloc(nb*sizeof(float));
	*campob = 0.001;
	for(i=1;i<nb;i++) *(campob+i) = 0.001+0.001*i;
	char filename[255];
	int *grilla;
	grilla = (int*) malloc(l*l*sizeof(int));
	float *magnet;
	magnet = (float*) malloc((nmed+1)*sizeof(float));
	float *energia;
	energia = (float*) malloc((nmed+1)*sizeof(float));
	float *resultados;
	resultados = (float*) malloc(2*nb*sizeof(float));
	for(i=0;i<2*nb;i++) *(resultados+i) = 0;
	for(i=0;i<nmed+1;i++) *(magnet+i)=0;
	for(i=0;i<nmed+1;i++) *(energia+i)=0;
	srand(semilla);
	poblar(grilla,l);
	for(n=0;n<nb;n++){
		for(i=0;i<nmed+1;i++) *(magnet+i)=0;
		for(i=0;i<nmed+1;i++) *(energia+i)=0;
		Be = *(campob+n);
		if(Be>0) protran = expf(-Be*2);
		else protran = expf(Be*2); //Resulta innecesario si Be es siempre positivo, pero por ahora dejalo
		for(i=0;i<termal;i++){
			si = Nuevospin(grilla,l,li);
			sf = Metropolis(si,protran,Be);
			*(grilla+*li) = sf;
			//printf("%d",si-sf);
    	}
		for(i=0;i<l*l;i++) *magnet = *magnet+(float)*(grilla+i)/(float)(l*l);
		*energia = -Be*(*magnet);
		for(i=0;i<nmed;i++){
			enertemp = *(energia+i);
			magtemp = *(magnet+i);
			for(j=0;j<desco;j++){
				si = Nuevospin(grilla,l,li);
				sf = Metropolis(si,protran,Be);
				*(grilla+*li) = sf;
				if(si!=sf){
					magtemp = magtemp-(2.0*si)/(float)(l*l);
					enertemp = enertemp+(2.0*Be*si)/(float)(l*l);
				}
			}
			*(energia+i+1) = enertemp;
			*(magnet+i+1) = magtemp;
		}
		enertemp = 0;
		magtemp = 0;
		for(i=0;i<nmed+1;i++){
			enertemp = enertemp+*(energia+i)/(nmed+1);
			magtemp = magtemp+*(magnet+i)/(nmed+1);
		}
		*(resultados+n) = enertemp;
		*(resultados+n+nb) = magtemp;
		time(&fin);
		tardanza = (int) (fin-prin);
		printf("Tarde %d segundos en armar los datos de muestreo con Be=%f \n",tardanza,Be);
	}
	sprintf(filename,"Datos_Ising_J=0.txt");
	FILE *fp=fopen(filename,"w");
	fprintf(fp,"Be\t");
	for(m=0;m<nb;m++) fprintf(fp,"%f\t",*(campob+m));
	fprintf(fp,"\n");
	fprintf(fp,"Magnetismo\t");
	for(m=0;m<nb;m++) fprintf(fp,"%f\t",*(resultados+m+nb));
	fprintf(fp,"\n");
	fprintf(fp,"Energia\t");
	for(m=0;m<nb;m++) fprintf(fp,"%f\t",*(resultados+m));
	fclose(fp);
	free(grilla);
	free(resultados);
	free(campob);
	free(energia);
	free(magnet);
	free(li);
	return 0;
}

//----------------------------------------------------------
// Con esta función se puebla la red

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

//-----------------------------------------------
// Con esta función elijo un nuevo spin

int Nuevospin(int *grilla,int l,int *li){
	int s;
	float alea;
	alea = ((float)rand()/(float) RAND_MAX);
	*li = round(alea*l*l);
	s = *(grilla+*li);
	return s;
}

//------------------------------------------------
// Con esta función elijo el nuevo paso

int Metropolis(int s,float protran,float Be){
	int sf;
	float alea;
	if(Be*s<0) sf = -s;
	else{
		alea = ((float)rand()/(float) RAND_MAX);
		if (alea<=protran) sf = -s;
		else sf = s;
	}
	return sf;
}



