#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include <math.h>
#define THRESHOLD 95


#define PI 3.14159265

float getAngle(int x1, int y1, int x2, int y2){
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

float getAngleFourrier(pnm imd){
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
	
	return getAngle(x1,y1,x2,y2);
}

void process(int threshold, char* ims_name,char* imd_name){	
	pnm ims = pnm_load(ims_name);
	
	int cols = pnm_get_width(ims);
	int rows = pnm_get_height(ims);
	pnm imd = pnm_new(cols, rows, PnmRawPpm);

	unsigned short *ps = pnm_get_image(ims);
	unsigned short *pd = pnm_get_image(imd);

	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			for(int c=0; c<3; c++){
				if(*ps < threshold){
					//printf("trop petit : %u\n",*ps);
					*pd = 0;
				} else {
					//printf("on est ok  : %u (%d,%d,%d)\n",*ps,i,j,c);
					*pd = *ps;
				}
				ps++;
				pd++;
			}
		}
	}

	getAngleFourrier(imd);

	pnm_save(imd, PnmRawPpm, imd_name);
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
