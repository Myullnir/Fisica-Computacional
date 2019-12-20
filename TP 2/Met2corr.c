// Vamos a calcular la correlacion para el punto 2 aca

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int Nuevospin(int *grilla,int l,int *li);
int Metropolis(int s, float protran, float Be);
int poblar(int *grilla, int l);
int Correlacion(float *muestreo, int j,int nmed, int medcorr, float d);

int main(){
	time_t prin, fin, semilla=time(NULL);
	time(&prin);
	float Be,protran;
	int i,j,m,n,si,sf,l=32,nb=11,medcorr=200000,tardanza; //Atento que quizás cambiar el nb te da problemas con campob
    int nmed = 4*medcorr;
    int *li;
    li = (int*) malloc(1*sizeof(int));
	*li=0;
	float *campob;
	campob = (float*) malloc(nb*sizeof(float));
	*campob = 0.1;
	for(i=1;i<nb;i++) *(campob+i) = 0.1+0.4*i;
	int *grilla;
	grilla = (int*) malloc((l*l)*sizeof(int));
	float *magnet;
	magnet = (float*) malloc((nmed+1)*sizeof(float));
	char filename[255];
	for(n=0;n<nb;n++){
		Be = *(campob+n);
		if(Be>0) protran = expf(-Be*2);
		else protran = expf(Be*2); //Resulta innecesario si Be es siempre positivo, pero por ahora dejalo
		srand(semilla);
		poblar(grilla,l);
		for(j=0;j<5;j++){ //El 50 es la cantidad de mediciones que despu�s voy a promediar
			for(i=0;i<nmed+1;i++) *(magnet+i)=0;
			sprintf(filename,"Datos_Ising_J=0_Magcorrj=%d_Be=%f.txt",j,Be);
			FILE *fp=fopen(filename,"w");
            for(i=0;i<l*l;i++) *magnet = *magnet+(float)*(grilla+i)/(float)(l*l);
			printf("Be=%f,Medicion %d \n",Be,j);
			//printf("La magnetizacion inicial es %f \n",*magnet);
    		for(i=0;i<nmed;i++){
    			si = Nuevospin(grilla,l,li);
    			sf = Metropolis(si,protran,Be);
    			*(grilla+*li) = sf;
				//printf("%d",si-sf);
    			if(si==sf) *(magnet+i+1)=*(magnet+i);
    			else *(magnet+i+1)=*(magnet+i)-(2.0*si)/(float)(l*l);
    		}
			for(m=0;m<nmed+1;m++) fprintf(fp,"%f\t",*(magnet+m));
			fclose(fp);
    		Correlacion(magnet,j,nmed,medcorr,Be);
        }
		time(&fin);
		tardanza= (int) fin-prin;
		printf("Tarde %d segundos en armar los datos de muestreo con Be=%f \n",tardanza,Be);	
	}
	free(grilla);
	free(campob);
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
	*li = floor(alea*l*l);
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

//-----------------------------------------------------------------------
// Esta funcion arma el archivo con datos de correlacion

int Correlacion(float *muestreo, int j, int nmed, int medcorr, float d){
	float vmc,vcm,vik,suma1,suma2,suma3;
	int i,k;
	char filename[255];
	sprintf(filename,"Datos_Correlacion_Ising_Magnetismo_Medicion=%d_Be=%f.txt",j,d);
	FILE *fp=fopen(filename,"w");
	/*fprintf(fp,"med\t");
	for(k=0;k<(4*medcorr);k++) fprintf(fp,"k%d\t",k);
	fprintf(fp,"\n");*/
	//float *corr;
	//corr = (float*) malloc(medcorr*sizeof(float));
	//for(i=0;i<medcorr;i++) *(corr+i)=0;
	vmc = 0;
	vcm = 0;
	suma1 = 0;
	suma2 = 0;
	for(i=0;i<nmed;i++) suma1 = suma1+*(muestreo+i);
	for(i=0;i<nmed;i++) suma2 = suma2+(*(muestreo+i))*(*(muestreo+i));
	vmc = (1.0/nmed)*(1.0/nmed)*suma1*suma1;
	vcm = (1.0/nmed)*suma2;
	for(k=0;k<medcorr;k++){
        vik = 0;
		suma3 = 0;
		for(i=0;i<nmed-k;i++) suma3 = suma3+(*(muestreo+i+k))*(*(muestreo+i));
		vik = (1.0/(nmed-k))*suma3;
		//*(corr+k) = (vik-vmc)/(vcm-vmc);
		fprintf(fp,"%f\t",(vik-vmc)/(vcm-vmc));
	}
	fclose(fp);
	//free(corr);
	return 0;
}


