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
	float Be,protran,magtemp;
	int i,j,m,n,si,sf,l=32,nb=5,nmed=1000000,desco=10000; //Atento que quizás cambiar el nb te da problemas con campob
	int *li;
	li = (int*) malloc(1*sizeof(int));
	float *campob;
	campob = (float*) malloc(nb*sizeof(float));
	*campob = 0.1;
	*(campob+1) = 1;
	*(campob+2) = 1.5;
	*(campob+3) = 2;
	*(campob+4) = 2.5;
	char filename[255];
	int *grilla;
	grilla = (int*) malloc(l*l*sizeof(int));
	float *magnet;
	magnet = (float*) malloc((nmed+1)*sizeof(float));
	for(n=0;n<nb;n++){
		Be = *(campob+n);
		if(Be>0) protran = expf(-Be*2);
		else protran = expf(Be*2); //Resulta innecesario si Be es siempre positivo, pero por ahora dejalo
		for(i=0;i<nmed+1;i++) *(magnet+i)=0;
		srand(semilla);
		sprintf(filename,"Datos_Ising_J=0_Be=%f.txt",Be);
		FILE *fp=fopen(filename,"w");
		poblar(grilla,l);
		for(i=0;i<l*l;i++) *magnet = *magnet+(float)*(grilla+i)/(float)(l*l);
		for(i=0;i<nmed;i++){
			magtemp = *(magnet+i);
			for(j=0;j<desco;j++){
				si = Nuevospin(grilla,l,li);
				sf = Metropolis(si,protran,Be);
				*(grilla+*li) = sf;
	    			if(si!=sf) magtemp=magtemp-(2.0*si)/(float)(l*l); //Está bien escrito el distinto?
			}
			*(magnet+i+1)=magtemp;
		}
		for(m=0;m<nmed+1;m++) fprintf(fp,"%f\t",*(magnet+i));
		fclose(fp);
		time(&fin);
		printf("Tarde %ld segundos en armar los datos de muestreo con Be=%f \n",fin-prin,Be);	
	}
	free(grilla);
	free(campob);
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



