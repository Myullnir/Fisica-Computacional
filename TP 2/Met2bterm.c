// Vamos a calcular la termalización para el punto 2b aca

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int Nuevospin(int *grilla,int l,int *li);
int Metropolis(int s,int l, int *grilla, float *protran, int *li, float Je);
int poblar(int *grilla, int l);
int Concon(int *grilla, int l);

int main(){
	time_t prin, fin, semilla;
	time(&prin);
	float Je;
	int i,j,n,m,f,c,si,sf,su,sd,sl,sr,l=32,nj=6,nmed=2500000,tardanza; //El nj es 6, ya hice la cuenta y no va a cambiar. Por ahora
	int *li;
	li = (int*) malloc(1*sizeof(int));
	*li=0;
	float *interj;
	interj = (float*) malloc(nj*sizeof(float));
	for(j=0;j<nj;j++) *(interj+j)=0.1+j*0.1;
	float *protran;
	protran = (float*) malloc(5*sizeof(float));
	int *grilla;
	grilla = (int*) malloc(((l+2)*(l+2))*sizeof(int));
	//float *magnet;
	//magnet = (float*) malloc((nmed+1)*sizeof(float));
	float *energia;
	energia = (float*) malloc((nmed+1)*sizeof(float));
	char filename[255];
	for(n=0;n<nj;n++){
		Je = *(interj+n);
		for(i=0;i<5;i++) *(protran+i)=expf((i*4-8)*Je);
		for(j=0;j<10;j++){
			semilla = time(NULL);
			srand(semilla);
			poblar(grilla,l+2); //Si bien l es el tamaño que yo quiero simular, el tamaño total va a ser l+2
			Concon(grilla,l+2);
			//for(i=0;i<nmed+1;i++) *(magnet+i)=0;
			for(i=0;i<nmed+1;i++) *(energia+i)=0;
			for(f=1;f<=l;f++){
				for(c=1;c<=l;c++){
					//*magnet = *magnet + (float)*(grilla+f*(l+2)+c)/(float)(l*l);
					sr = *(grilla+f*(l+2)+c+1);
					sd = *(grilla+(f+1)*(l+2)+c);
					*energia = *energia - *(grilla+f*(l+2)+c)*(Je*(sr+sd))/(float)(l*l);
				}
			}
			for(i=0;i<nmed;i++){
				si = Nuevospin(grilla,l+2,li);
				sf = Metropolis(si,l,grilla,protran,li,Je);
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
					*(energia+i+1) = *(energia+i)+(2*Je*si*((float)su+(float)sd+(float)sl+(float)sr))/(float)(l*l);
				}
			}
			sprintf(filename,"Datos_Ising_B=0_Termj=%d_Je=%f.txt",j,Je);
			FILE *fp=fopen(filename,"w");
			/*fprintf(fp,"Magnetismo\t");
			for(m=0;m<nmed+1;m++) fprintf(fp,"%f\t",*(magnet+m));
			fprintf(fp,"\n");*/
			fprintf(fp,"Energia\t");
			for(m=0;m<nmed+1;m++) fprintf(fp,"%f\t",*(energia+m));
			fclose(fp);
		}
		time(&fin);
		tardanza = (int) (fin-prin);
		printf("Tarde %d segundos en armar los datos de muestreo con Je=%f \n",tardanza,Je);
	}
	free(grilla);
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
	int sf,su=*(grilla+*li-(l+2)),sd=*(grilla+*li+(l+2)),sl=*(grilla+*li-1),sr=*(grilla+*li+1),st; //Son spin up,down,left,right
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


