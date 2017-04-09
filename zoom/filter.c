#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include <math.h>
#include <string.h>

float box(float x) {
	if (x>=-0.5 && x<0.5)
		return 1.0;
	else
		return 0.0;
}

float tent(float x) {
	if (x>=-1.0 && x<=1.0)
		return 1.0-fabsf(x);
	else
		return 0.0;
}

float bell(float x) {
	if (fabsf(x)<=0.5) 
		return (-x*x)+0.75;

	else if (fabsf(x)>0.5 && fabsf(x)<=1.5)
		return 0.5*(fabsf(x)-1.5) * (fabsf(x)-1.5);

	else
		return 0.0;
}

float mitch(float x){
	if (x>=-1.0 && x<=1.0)
		return 7.0/6.0*pow(fabsf(x),3) - 2.0*x*x + 8.0/9.0;

	else if ((x>=-2.0 && x<=-1.0) || (x>=1 && x<= 2))
		return -7.0/18.0*pow(fabsf(x),3) + 2.0*x*x - 10.0/3.0*fabsf(x) + 16.0/9.0;

	else
		return 0.0;
}

void run(int factor, char* filter_name, pnm ims, char* imd){
	int cols = pnm_get_width(ims);
	int rows = pnm_get_height(ims);

	int newCols = factor*cols;
	int newRows = factor*rows;

	//pour premiere interpolation
	pnm tmpImage = pnm_new(newCols, rows, PnmRawPpm);

	//interpolation en colonne
	for(int i=0; i<rows; i++){	
		for(int jprime=0; jprime<newCols; jprime++){
			float j=(float)jprime/factor;
			
			float WF = 0;
			if (strcmp(filter_name, "box")==0) 
				WF = 0.5;
			else if (strcmp(filter_name, "tent")==0) 
				WF = 1.0;
			else if (strcmp(filter_name, "bell")==0) 
				WF = 1.5;
			else if (strcmp(filter_name, "mitch")==0) 
				WF = 2.0;

			float left = j-WF;
			float right = j+WF;

			int leftEntier = floor(left);
			int rightEntier = floor(right);

			float S = 0.0;
			for (int k=leftEntier; k<=rightEntier; k++) {
				float h;
				if (strcmp(filter_name, "box")==0) 
					h = box((float)k-j);
				else if (strcmp(filter_name, "tent")==0) 
					h = tent((float)k-j);
				else if (strcmp(filter_name, "bell")==0) 
					h = bell((float)k-j);
				else if (strcmp(filter_name, "mitch")==0) 
					h = mitch((float)k-j);

				if (k<0)
					S += pnm_get_component(ims, i, 0, 0)*h;
				else if (k>=cols)
					S += pnm_get_component(ims, i, cols-1, 0)*h;
				else 
					S += pnm_get_component(ims, i, k, 0)*h;
			}

			for (int c=0; c<3; c++){
				pnm_set_component(tmpImage, i, jprime, c, (unsigned short)S);
			}
		}
	}

	//pour la deuxieme interpolation
	pnm newImage = pnm_new(newCols, newRows, PnmRawPpm);
	
	//interpolation en ligne
	for(int iprime=0; iprime<newRows; iprime++){	
		for(int j=0; j<newCols; j++){
			float i = (float)iprime/factor;

			float WF = 0;
			if (strcmp(filter_name, "box")==0) 
				WF = 0.5;
			else if (strcmp(filter_name, "tent")==0) 
				WF = 1.0;
			else if (strcmp(filter_name, "bell")==0) 
				WF = 1.5;
			else if (strcmp(filter_name, "mitch")==0) 
				WF = 2.0;

			float left = i-WF;
			float right = i+WF;

			int leftEntier = floor(left);
			int rightEntier = floor(right);

			float S = 0.0;
			for (int k=leftEntier; k<=rightEntier; k++) {
				float h;
				if (strcmp(filter_name, "box")==0) 
					h = box((float)k-i);
				else if (strcmp(filter_name, "tent")==0) 
					h = tent((float)k-i);
				else if (strcmp(filter_name, "bell")==0) 
					h = bell((float)k-i);
				else if (strcmp(filter_name, "mitch")==0) 
					h = mitch((float)k-i);

				if (k<0)
					S += pnm_get_component(tmpImage, 0, j, 0)*h;
				else if (k>=rows)
					S += pnm_get_component(tmpImage, rows-1, j, 0)*h;
				else
					S += pnm_get_component(tmpImage, k, j, 0)*h;
			}

			for (int c=0; c<3; c++){
				pnm_set_component(newImage, iprime, j, c, (unsigned short)S);
			}
		}
	}

	pnm_save(newImage, PnmRawPpm, imd);
	pnm_free(ims);
	pnm_free(tmpImage);
	pnm_free(newImage);
}	


void usage (char *s){
	fprintf(stderr, "Usage: %s <factor> <filter-name> <ims> <imd> \n", s);
	exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char *argv[]){
	if (argc != PARAM+1) usage(argv[0]);	
	int factor = atoi(argv[1]);
	char *filter_name = argv[2];
	pnm ims = pnm_load(argv[3]);
	char *name = argv[4];

	run(factor,filter_name,ims,name);
	return EXIT_SUCCESS;
}
