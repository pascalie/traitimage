#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include <math.h>
#define THRESHOLD 150


#define PI 3.14159265

float getAngle(pnm imd){
	int cols = pnm_get_width(imd);
	int rows = pnm_get_height(imd);

	int x1=0;
	int y1=0;
	int x2=0;
	int y2=0;

	//determination du point (x1,y1), point le plus a gauche, non nul
	for(int j=0; j<cols; j++){
		for(int i=0; i<rows; i++){
			if((pnm_get_component(imd, i, j, 0) != 0) && (x1 == 0 && y1 ==0) ){
				x1 = j;
				y1 = i;
			}
		}
	}	
	
	//determination du point (x2,y2), point le plus a droite, non nul
	for(int j=cols-1; j>=0;j--){
		for(int i=rows-1; i>=0; i--){
			if((pnm_get_component(imd, i, j, 0) != 0) && (x2 == 0 && y2 ==0) ){
				x2 = j;
				y2 = i;	
			}
		}
	}

	printf("x1: %d\nx2:%d\n",x1,x2);
	printf("y1: %d\ny2:%d\n",y1,y2);
	double dx = x2-x1;
	double dy = y2-y1;

	printf("dx: %lf, dy: %lf, dy/dx : %lf", dx, dy, dy/dx);
	double val = 180.0 / PI;


	//determiner angle
	double angle;
	if(dy/dx < PI/4)
		angle = (float)-atan(dy/dx)*val;
	else 
		angle = (float)90-(atan(dy/dx)*val);

	printf("angle : %f\n",angle);
	
	return angle;
}

float neighborhoodMean(float* image, int rows, int cols, int abs, int ord, int neighborhoodSize){
	if(neighborhoodSize%2 == 0){
		neighborhoodSize--;
	}

	int neighborhoodhalf = neighborhoodSize/2 +1;

	int distHG = neighborhoodhalf;
	int distHD = neighborhoodhalf;
	int distVH = neighborhoodhalf;
	int distVB = neighborhoodhalf;
	

	if(abs < neighborhoodhalf){
		distHG = abs;

	}

	if(abs > cols-neighborhoodhalf-1){
		distHD = cols - 1 - abs;
	}

	if(ord < neighborhoodhalf){
		distVH = ord;
	}

	if(ord > rows-neighborhoodhalf-1){
		distVB = rows - 1 - ord;
	}

	int tmpWidth = distHG + distHD +1;
	int tmpHeight = distVH + distVB +1;

	float tmp[tmpWidth][tmpHeight];

	int J0 = ord - distVH;
	int I0 = abs - distHG;

	for(int i=0; i<tmpWidth;i++){
		for(int j=0; j<tmpHeight; j++){
			tmp[i][j] = image[(J0+j)*cols+I0+i];
		}
	}

	float moyenne = 0;
	for(int i=0; i< tmpWidth; i++){
		for (int j = 0; j < tmpHeight; j++){
			moyenne += tmp[i][j];
		}
	}
	moyenne = moyenne/(tmpHeight*tmpWidth);

	return moyenne;
}

int isWhite(float value,int print){
	if(print)
		return(value>100);
		//printf("%f\n",value);
	return(value>100);
}
int isCircle(float** moyennes, int rows, int cols,int abs, int ord, int neighborhoodSize){
	//printf("Size :%d, Half: %d\n", neighborhoodSize,neighborhoodHalf);
	//printf("abs:%d, ord:%d\n",abs,ord);
	if((abs<neighborhoodSize) || (ord<neighborhoodSize) || 
	   (abs>=cols-neighborhoodSize) || (ord>=rows-neighborhoodSize) ){
		return 0;
	} else{
		if(abs==30 && ord==532){
			if(isWhite(moyennes[abs][ord],1) &&
			 !(isWhite(moyennes[abs-neighborhoodSize][ord],1)) &&
			 !(isWhite(moyennes[abs+neighborhoodSize][ord],1)) &&
			 !(isWhite(moyennes[abs][ord-neighborhoodSize],1)) &&
			 !(isWhite(moyennes[abs][ord+neighborhoodSize],1)) ){
				return 1;
			} else return 0;
		} else if(isWhite(moyennes[abs][ord],0) &&
		 !(isWhite(moyennes[abs-neighborhoodSize][ord],0)) &&
		 !(isWhite(moyennes[abs+neighborhoodSize][ord],0)) &&
		 !(isWhite(moyennes[abs][ord-neighborhoodSize],0)) &&
		 !(isWhite(moyennes[abs][ord+neighborhoodSize],0)) ){
			return 1;
		} else return 0;
	}
}


void process(int threshold, char* ims_name,char* imd_name){	
	pnm ims = pnm_load(ims_name);
	
	int cols = pnm_get_width(ims);
	int rows = pnm_get_height(ims);

	int neighborhoodSize = (rows/60);

	pnm imd = pnm_new(cols, rows, PnmRawPpm);

	unsigned short *ps = pnm_get_image(ims);
	unsigned short *pd = pnm_get_image(imd);
	float *gray = malloc(rows*cols*sizeof(float));

	float** moyennes = malloc(rows*sizeof(float*));
	for(int i=0;i<rows;i++){
		moyennes[i] = malloc(cols*sizeof(float));
	}

	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			for(int c=0; c<3; c++){
				float tmp = (float)*ps;
				tmp = tmp/46.4;
				tmp=expf(tmp);
				*pd = (unsigned short)tmp;//image transfo exponentielle

				ps++;
				pd++;
			}
		}
	}
	ps-=3*rows*cols;
	pd-=3*rows*cols;

	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			unsigned short R;
			unsigned short G;
			unsigned short B;

			R = *pd;
			pd++;
			G = *pd;
			pd++;
			B = *pd;
			pd++;
			
			float tmp = (float)(R+G+B)/3.0;
			*gray = tmp;

			gray++;
		}
	}
	gray-=rows*cols;
	pd-=3*rows*cols;

	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			moyennes[i][j] = neighborhoodMean(gray, rows, cols, j, i, neighborhoodSize);
		}
	}

	printf("isCirle : %d", isCircle(moyennes,cols,rows,532,30,neighborhoodSize));
	pnm newImage = pnm_new(cols, rows, PnmRawPpm);
	printf("sgioeig\n");
	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			if(isCircle(moyennes, cols, rows, i, j, neighborhoodSize)){
				pnm_set_component(newImage,i,j,0,255);
				pnm_set_component(newImage,i,j,1,0);
				pnm_set_component(newImage,i,j,2,0);
				pd+=3;
			} else {
				for(int c=0; c<3; c++){
					pnm_set_component(newImage,i,j,c,*pd);
					pd++;
				}
			}
		}
	}
	pd-=3*cols*rows;
	//getAngle(imd);

	(void)threshold;
	pnm_save(newImage, PnmRawPpm, imd_name);
	pnm_free(ims);
	pnm_free(imd);
}

void 
usage (char *s){
	fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
	exit(EXIT_FAILURE);
}

#define PARAM 2
int main(int argc, char *argv[]){
	if (argc != PARAM+1) usage(argv[0]);	
	char *ims_name = argv[1];
	char *imd_name = argv[2];
	
	process(THRESHOLD, ims_name,imd_name);
	return EXIT_SUCCESS;
}
