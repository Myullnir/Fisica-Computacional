// Vamos a calcular magnetización y energía para el punto 2c aca

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int Nuevospin(int *grilla,int l,int *li);
int Metropolis(int s,int l, int *grilla, float *protran, int *li, float Je);
int poblar(int *grilla, int l);
int Concon(int *grilla, int l);

int main(){
	printf("Todavía no la cague");
	time_t prin, fin, semilla;
	time(&prin);
	float Je,suma1,suma2,enertemp,magtemp,chi;
	int i,j,n,m,k,p=-1,f,c,si,sf,su,sd,sl,sr,l=8,nj=27,nmed=500,tardanza;
	int *li;
	li = (int*) malloc(1*sizeof(int));
	*li=0;
	float *interj;
	interj = (float*) malloc(nj*sizeof(float));
	for(j=0;j<5;j++) *(interj+j) = 0.1+j*0.05;
	for(j=5;j<25;j++) *(interj+j) = 0.3+(j-4)*0.01;
	for(j=25;j<nj;j++) *(interj+j) = 0.5+(j-24)*0.05;
	float *protran;
	protran = (float*) malloc(5*sizeof(float));
	float *magnet;
	magnet = (float*) malloc((nmed+1)*sizeof(float));
	float *energia;
	energia = (float*) malloc((nmed+1)*sizeof(float));
	// Atento a que si cambiás el nj, esto lo vas a tener que cambiar también
	// Los números salen de los gráficos de correlación y termalización
	int *termal;
	termal = (int*) malloc(4*nj*sizeof(int));
	for(i=0;i<nj;i++) *(termal+i) = 5000; //Esto es para L=8
	for(i=nj;i<2*nj;i++) *(termal+i) = 50000; //Esto es para L=16
	for(i=2*nj;i<3*nj;i++) *(termal+i) = 1000000; //Esto es para L=32
	for(i=3*nj;i<4*nj;i++) *(termal+i) = 20000000; //Esto es para L=64
	int *desco;
	desco = (int*) malloc(4*nj*sizeof(int));
	// Esto es para L=8
	for(i=0;i<5;i++) *(desco+i) = 1000;
	for(i=5;i<25;i++) *(desco+i) = 5000;
	for(i=25;i<nj;i++) *(desco+i) = 1000;
	// Esto es para L=16
	for(i=nj;i<5+nj;i++) *(desco+i) = 4000;
	for(i=5+nj;i<25+nj;i++) *(desco+i) = 40000;
	for(i=25+nj;i<2*nj;i++) *(desco+i) = 4000;
	// Esto es para L=32
	for(i=2*nj;i<5+3*nj;i++) *(desco+i) = 5000;
	for(i=5+2*nj;i<25+3*nj;i++) *(desco+i) = 100000;
	for(i=25+2*nj;i<3*nj;i++) *(desco+i) = 10000;
	// Esto es para L=64
	for(i=3*nj;i<5+4*nj;i++) *(desco+i) = 20000;
	for(i=5+3*nj;i<25+4*nj;i++) *(desco+i) = 2000000;
	for(i=25+3*nj;i<4*nj;i++) *(desco+i) = 40000;
	float *resultados;
	resultados = (float*) malloc(3*nj*sizeof(float));
	for(i=0;i<3*nj;i++) *(resultados+i) = 0;
	char filename[255];
	for(k=8;k<=8;k=k*2){
		p++;
		int *grilla;
		grilla = (int*) malloc(((l+2)*(l+2))*sizeof(int));
		semilla = time(NULL);
		srand(semilla);
		for(n=0;n<nj;n++){
			poblar(grilla,l+2);
			Concon(grilla,l+2);
			Je = *(interj+n);
			for(i=0;i<5;i++) *(protran+i) = expf((i*4-8)*Je);
			for(i=0;i<*(termal+p*nj+n);i++){
				si = Nuevospin(grilla,l+2,li);
				sf = Metropolis(si,l+2,grilla,protran,li,Je);
				*(grilla+*li) = sf;
				Concon(grilla,l+2);
			}
			for(i=0;i<nmed+1;i++) *(magnet+i)=0;
			for(i=0;i<nmed+1;i++) *(energia+i)=0;
			for(f=1;f<=l;f++){
				for(c=1;c<=l;c++){
					*magnet = *magnet+(float)*(grilla+f*(l+2)+c)/(float)(l*l);
					sr = *(grilla+f*(l+2)+c+1);
					sd = *(grilla+(f+1)*(l+2)+c);
					*energia = *energia-*(grilla+f*(l+2)+c)*(Je*(sr+sd))/(float)(l*l);
				}
			}
			for(i=0;i<nmed;i++){
				enertemp = *(energia+i);
				magtemp = *(magnet+i);
				for(j=0;j<*(desco+p*nj+n);j++){
						si = Nuevospin(grilla,l+2,li);
						sf = Metropolis(si,l+2,grilla,protran,li,Je);
						*(grilla+*li) = sf;
						Concon(grilla,l+2);
						if(si!=sf){
							su = *(grilla+*li-(l+2));
							sd = *(grilla+*li+(l+2));
							sl = *(grilla+*li-1);
							sr = *(grilla+*li+1);
							magtemp = magtemp-(2.0*si)/(float)(l*l);
							enertemp = enertemp+(2.0*Je*si*((float)su+(float)sd+(float)sl+(float)sr))/(float)(l*l);
						}
				}
				*(energia+i+1) = enertemp;
				*(magnet+i+1) = magtemp;
			}
			enertemp = 0;
			magtemp = 0;
			suma1 = 0;
			suma2 = 0;
			for(i=0;i<nmed+1;i++){
				enertemp = enertemp+*(energia+i)/(nmed+1);
				magtemp = magtemp+*(magnet+i)/(nmed+1);
				suma1 = suma1+*(magnet+i)/(nmed+1);
				suma2 = suma2+(*(magnet+i)*(*(magnet+i)))/(nmed+1);
			}
			chi = suma2-suma1*suma1;
			*(resultados+n) = magtemp;
			*(resultados+n+nj) = enertemp;
			*(resultados+n+2*nj) = chi;
			time(&fin);
			tardanza = (int) (fin-prin);
			printf("Tarde %d segundos en armar los datos de muestreo con Je=%f \n",tardanza,Je);	
		}
		sprintf(filename,"Datos_Ising_L=%d_B=0.txt",l);
		FILE *fp=fopen(filename,"w");
		fprintf(fp,"Je\t");
		for(m=0;m<nj;m++) fprintf(fp,"%f\t",*(interj+m));
		fprintf(fp,"\n");
		fprintf(fp,"Magnetismo\t");
		for(m=0;m<nj;m++) fprintf(fp,"%f\t",*(resultados+m));
		fprintf(fp,"\n");
		fprintf(fp,"Energia\t");
		for(m=0;m<nj;m++) fprintf(fp,"%f\t",*(resultados+m+nj));
		fprintf(fp,"\n");
		fprintf(fp,"Chi\t");
		for(m=0;m<nj;m++) fprintf(fp,"%f\t",*(resultados+m+2*nj));
		free(grilla);
	}
	free(interj);
	free(magnet);
	free(energia);
	free(resultados);
	free(desco);
	free(termal);
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


