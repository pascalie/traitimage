#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include <math.h>
#define THRESHOLD 95


#define PI 3.14159265

typedef struct node {
    int i;
    int j;

    struct node * headGroup;

    struct node * previousInGroup;
    struct node * nextInGroup;
    
    struct node * nextGroup;
} node_t;


float distanceAB(int xa, int ya, int xb, int yb){
	return(sqrtf( (float)((xa-xb)*(xa-xb) + (ya-yb)*(ya-yb)) ));
}

float getAngle(int x1, int y1, int x2, int y2){
	//printf("x1: %d\nx2:%d\n",x1,x2);
	//printf("y1: %d\ny2:%d\n",y1,y2);
	double dx = x2-x1;
	double dy = y2-y1;

	//printf("dx: %lf, dy: %lf, dy/dx : %lf", dx, dy, dy/dx);
	double val = 180.0 / PI;


	//determiner angle
	double angle;
	if(dy/dx < PI/4)
		angle = (float)-atan(dy/dx)*val;
	else 
		angle = (float)90-(atan(dy/dx)*val);

	//printf("angle : %f\n",angle);
	
	return angle;
}

float getAngleCercles(char* ims_name){	
	pnm ims = pnm_load(ims_name);
	
	int cols = pnm_get_width(ims);
	int rows = pnm_get_height(ims);

	unsigned short *ps = pnm_get_image(ims);

	int absMin = cols;
	int absMax = 0;
	int ordMin = rows;
	int ordMax = 0;
	
	int nbFound =0;
	int lastLineFound=-1;

	int circles[2][2]; //circles[a][0]=abs //circles[a][1]=ord


	//determiner les deux cercles les plus hauts
	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			if(*ps==255 && *(ps+1)==0 && *(ps+2)==0){
				if(lastLineFound==i || lastLineFound==i-1){
					lastLineFound=i;
					if(i<ordMin){
						ordMin = i;
						//printf("ordMin: %d\n",ordMin);
					}
					if(i>ordMax){
						ordMax = i;
						//printf("   ordMax: %d\n",ordMax);
					}

					if(j<absMin){
						absMin = j;
						//printf("      absMin: %d\n",absMin);
					}
					if(j>absMax){
						absMax = j;
						//printf("         absMax: %d\n",absMax);
					}
				} else if(lastLineFound != -1){
					lastLineFound=i;
					//printf("found new group ! \n    lastLineFound:%d -- current line: %d\n",lastLineFound,i);

					if(nbFound<2){
						int abs=(absMin+absMax)/2;
						int ord=(ordMin+ordMax)/2;
						circles[nbFound][0]=abs;
						circles[nbFound][1]=ord;
						
						nbFound++;
						absMin=j;
						absMax=j;
						ordMin=i;
						ordMax=i; 
					}
				} else {
					absMin=j;
					absMax=j;
					ordMin=i;
					ordMax=i;
					lastLineFound=i;
				}				
			}
			ps+=3;
		}
	}
	ps-=3*rows*cols;

	//printf("COORDONNEES\n");
	for(int i=0; i<nbFound;i++){
		//printf("    (%d,%d)\n",circles[i][0],circles[i][1]);
	}

	float angle;
	if(nbFound != 2){
		printf("ERREUR : detection des cercles impossible, rotation de 0degres par defaut.");
		return 0.0;
	} else {
		if(circles[0][0] < circles[1][0])
			angle = getAngle(circles[0][0],circles[0][1],circles[1][0],circles[1][1]);
		else
			angle = getAngle(circles[1][0],circles[1][1],circles[0][0],circles[0][1]);
	}


	printf("Angle: %f\n",angle);
	return angle;
}

void 
usage (char *s){
	fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
	exit(EXIT_FAILURE);
}

#define PARAM 1
int main(int argc, char *argv[]){
	if (argc != PARAM+1) usage(argv[0]);	
	char *ims_name = argv[1];
	
	getAngleCercles(ims_name);
	return EXIT_SUCCESS;
}
