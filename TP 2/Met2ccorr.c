// Vamos a calcular la correlacion para el punto 2c aca

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int Nuevospin(int *grilla,int l,int *li);
int Metropolis(int s,int l, int *grilla, float *protran, int *li, float Je);
int poblar(int *grilla, int l);
int Correlacion(float *muestreo, int j,int nmed, int medcorr, float d, int l);
int Concon(int *grilla, int l);

int main(){
	time_t prin, fin, semilla;
	time(&prin);
	float Je;
	int i,j,n,f,c,si,sf,su,sd,sr,sl,l,nj=6,medcorr=200000,termal=1000000,tardanza; //El nj es 6, ya hice la cuenta y no va a cambiar. Por ahora
	int nmed=3*medcorr;
	int *li;
	li = (int*) malloc(1*sizeof(int));
	*li=0;
	float *interj;
	interj = (float*) malloc(nj*sizeof(float));
	for(j=0;j<nj;j++) *(interj+j)=0.1+j*0.1;
	float *protran;
	protran = (float*) malloc(5*sizeof(float));
	//float *magnet;
	//magnet = (float*) malloc((nmed+1)*sizeof(float));
	float *energia;
	energia = (float*) malloc((nmed+1)*sizeof(float));
	//char filename[255];
	for(l=4;l<=64;l=l*2){
		int *grilla;
		grilla = (int*) malloc(((l+2)*(l+2))*sizeof(int));
		for(n=0;n<nj;n++){
			Je = *(interj+n);
			for(i=0;i<5;i++) *(protran+i)=expf((i*4-8)*Je);
			semilla = time(NULL);
			srand(semilla);
			poblar(grilla,l+2);
			Concon(grilla,l+2);
			for(i=0;i<termal;i++){
				si = Nuevospin(grilla,l+2,li);
				sf = Metropolis(si,l+2,grilla,protran,li,Je);
				*(grilla+*li) = sf;
				Concon(grilla,l+2);
			}
			for(j=0;j<5;j++){
				//for(i=0;i<nmed+1;i++) *(magnet+i)=0;
				for(i=0;i<nmed+1;i++) *(energia+i)=0;
				for(f=1;f<=l;f++){
					for(c=1;c<=l;c++){
						//*magnet = *magnet+(float)*(grilla+f*(l+2)+c)/(float)(l*l);
						sr = *(grilla+f*(l+2)+c+1);
						sd = *(grilla+(f+1)*(l+2)+c);
						*energia = *energia - *(grilla+f*(l+2)+c)*(Je*(sr+sd))/(float)(l*l);
					}
				}
				printf("Je=%f,Medicion %d,L=%d",Je,j,l);
				printf("\n");
				for(i=0;i<nmed;i++){
					si = Nuevospin(grilla,l+2,li);
					sf = Metropolis(si,l+2,grilla,protran,li,Je);
					*(grilla+*li) = sf;
					Concon(grilla,l+2);
					su = *(grilla+*li-(l+2));
					sd = *(grilla+*li+(l+2));
					sl = *(grilla+*li-1);
					sr = *(grilla+*li+1);
					if(si==sf){
						//*(magnet+i+1) = *(magnet+i);
						*(energia+i+1) = *(energia+i);
					}
					else{
						//*(magnet+i+1) = *(magnet+i)-(2.0*si)/(float)(l*l);
						*(energia+i+1) = *(energia+i)+(2.0*Je*si*((float)su+(float)sd+(float)sl+(float)sr))/(float) (l*l);
					}
				}
				//Correlacion(magnet,j,nmed,medcorr,Je);
				Correlacion(energia,j,nmed,medcorr,Je,l);
			}
			time(&fin);
			tardanza = (int) (fin-prin);
			printf("Tarde %d segundos en armar los datos de muestreo con Je=%f,L=%d \n",tardanza,Je,l);	
		}
		free(grilla);
	}
	free(interj);
	//free(magnet);
	free(energia);
	free(protran);
	free(li);
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
	*li = f*l+c;
	s = *(grilla+*li);
	return s;
}

//------------------------------------------------
// Con esta funciÃ³n elijo el nuevo paso

int Metropolis(int s,int l,int *grilla,float *protran,int *li,float Je){
	int sf,su=*(grilla+*li-l),sd=*(grilla+*li+l),sl=*(grilla+*li-1),sr=*(grilla+*li+1),st; //Son spin up,down,left,right
	float alea;
	st = su+sd+sl+sr;
	alea = ((float)rand()/(float) RAND_MAX);
	if(s==1){
		if(alea<=*(protran-(st/2-2))) sf=-s;
		else sf=s;
	}
	else{
		if(alea<=*(protran+(st/2+2))) sf = -s;
		else sf = s;
	}
	return sf;
}


//-----------------------------------------------------------------------
// Esta funcion arma el archivo con datos de correlacion

int Correlacion(float *muestreo, int j, int nmed, int medcorr, float d,int l){
	float vmc,vcm,vik,suma1,suma2,suma3;
	int i,k;
	/*fprintf(fp,"med\t");
	for(k=0;k<(4*medcorr);k++) fprintf(fp,"k%d\t",k);
	fprintf(fp,"\n");*/
	float *corr;
	corr = (float*) malloc(medcorr*sizeof(float));
	for(i=0;i<medcorr;i++) *(corr+i)=0;
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
		*(corr+k) = (vik-vmc)/(vcm-vmc);
	}
	char filename[255];
	//sprintf(filename,"Datos_Correlacion_Ising_Magnetismo_Medicion=%d_Je=%f.txt",j,d);
	sprintf(filename,"Datos_Correlacion_Ising_Energia_Medicion=%d_L=%d_Je=%f.txt",j,l,d);
	FILE *fp=fopen(filename,"w");
	for(k=0;k<medcorr;k++) fprintf(fp,"%f\t",*(corr+k));
	fclose(fp);
	free(corr);
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


