#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include <math.h>
#define THRESHOLD 150


#define PI 3.14159265
#define E_TRAIT 60



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

int isWhite(float value){
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
			if(isWhite(moyennes[abs][ord]) &&
			 !(isWhite(moyennes[abs-neighborhoodSize][ord])) &&
			 !(isWhite(moyennes[abs+neighborhoodSize][ord])) &&
			 !(isWhite(moyennes[abs][ord-neighborhoodSize])) &&
			 !(isWhite(moyennes[abs][ord+neighborhoodSize])) ){
				return 1;
			} else return 0;
		} else if(isWhite(moyennes[abs][ord]) &&
		 !(isWhite(moyennes[abs-neighborhoodSize][ord])) &&
		 !(isWhite(moyennes[abs+neighborhoodSize][ord])) &&
		 !(isWhite(moyennes[abs][ord-neighborhoodSize])) &&
		 !(isWhite(moyennes[abs][ord+neighborhoodSize])) ){
			return 1;
		} else return 0;
	}
}


/*************************************************************
 *			PREPROCESSING
 *
 */
pnm transfoExponentielle(pnm ims, int rows, int cols){
	pnm imd = pnm_new(cols, rows, PnmRawPpm);

	unsigned short *ps = pnm_get_image(ims);
	unsigned short *pd = pnm_get_image(imd);

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

	return imd;
}

void toGray(pnm ims, float* ret, int rows, int cols){
	unsigned short *ps = pnm_get_image(ims);
	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			unsigned short R;
			unsigned short G;
			unsigned short B;

			R = *ps;
			ps++;
			G = *ps;
			ps++;
			B = *ps;
			ps++;
			
			float tmp = (float)(R+G+B)/3.0;
			*ret = tmp;

			ret++;
		}
	}
	ps-=3*rows*cols;
	ret-=rows*cols;
}

void toMoyenne(float* gray, float** moyennes, int rows, int cols, int neighborhoodSize){
	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			moyennes[i][j] = neighborhoodMean(gray, rows, cols, j, i, neighborhoodSize);
		}
	}
}

pnm colorCirclesRed(pnm ims, float** moyennes, int rows, int cols, int neighborhoodSize){
	pnm circlesImg = pnm_new(cols, rows, PnmRawPpm);
	unsigned short *ps = pnm_get_image(ims);
	for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
			if(isCircle(moyennes, cols, rows, i, j, neighborhoodSize)){
				pnm_set_component(circlesImg,i,j,0,255);
				pnm_set_component(circlesImg,i,j,1,0);
				pnm_set_component(circlesImg,i,j,2,0);
				ps+=3;
			} else {
				for(int c=0; c<3; c++){
					pnm_set_component(circlesImg,i,j,c,*ps);
					ps++;
				}
			}
		}
	}
	ps-=3*cols*rows;

	return circlesImg;
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
	
	return angle;
}

float getAngleCercles(pnm ims){	
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

	return angle;
}


/*************************************************************
 *			ROTATE IMAGE
 *
 */
unsigned short bilinear_interpolation(float x, float y, pnm ims, int c) {

    unsigned short res;
    int i = (int)y;
    int j = (int)x;
    float dx = x - j;
    float dy = y - i;

    unsigned short Iij = pnm_get_component(ims, i, j, c);
    unsigned short Iij1 = pnm_get_component(ims, i, j+1, c);
    unsigned short Ii1j = pnm_get_component(ims, i+1, j, c);
    unsigned short Ii1j1 = pnm_get_component(ims, i+1, j+1, c);

    res = (1 - dx)*(1 - dy)*Iij + dx*(1 - dy)*Iij1 + (1 - dx)*dy*Ii1j + dx*dy*Ii1j1;

    return res;
}

void rotatePoint(float *res, int x, int y, float angle, int x0, int y0) {
    res[0] = cos(angle)*(x-x0) - sin(angle)*(y-y0)+x0;
    res[1] = sin(angle)*(x-x0) + cos(angle)*(y-y0)+y0;
}

void rotateImg(float angle, pnm ims, char* imd_name) {
    angle = -angle;
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);

    pnm imd = pnm_new(width, height, PnmRawPpm);

    int x0 = width/2;
    int y0 = height/2;

    float coord[2];
    float radAngle = angle * PI / 180;
    unsigned short tmp;

    for (int x=0; x<width-1; x++) {

        for (int y=0; y<height-1; y++) {

            rotatePoint(coord, x, y, radAngle, x0, y0);

            for (int c=0; c<3; c++) {

                if (coord[0] >= 0 && coord[1] >=0 && coord[0] < width-1 && coord[1] < height-1) {
                    tmp = bilinear_interpolation(coord[0], coord[1], ims, c);
                    pnm_set_component(imd, y, x, c, tmp);
                }
            }
        }
    }


    pnm_save(imd, PnmRawPpm, imd_name);
    pnm_free(imd);
    pnm_free(ims);

}




/*************************************************************
 *			IMAGES INTERMEDIAIRES
 *
 */
void getGrayImg(float* gray, int rows, int cols){
 	pnm grayImg = pnm_new(cols, rows, PnmRawPpm);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				pnm_set_component(grayImg,i,j,c,*gray);
			}
			gray++;
		}
	}
	gray-=cols*rows;

	pnm_save(grayImg, PnmRawPpm, "intermediary-gray.ppm");
}

void getMoyImg(float** moyennes, int rows, int cols){
	pnm moyImg = pnm_new(cols, rows, PnmRawPpm);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				pnm_set_component(moyImg,i,j,c,moyennes[i][j]);
			}
		}
	}
	pnm_save(moyImg, PnmRawPpm, "intermediary-moyenne.ppm");
}


float processCorners(pnm ims, int rows, int cols, int neighborhoodSize){
	//preprocessing
	//transfo exponentielle
	pnm expoImg = transfoExponentielle(ims, rows, cols);
	//to gray
	float *gray = malloc(rows*cols*sizeof(float));
	toGray(expoImg, gray, rows, cols);
	//to moyenne
	float** moyennes = malloc(rows*sizeof(float*));
	for(int i=0;i<rows;i++){
		moyennes[i] = malloc(cols*sizeof(float));
	}
	toMoyenne(gray, moyennes, rows, cols, neighborhoodSize);

	//color corners
	pnm circlesImg = colorCirclesRed(ims, moyennes, rows, cols, neighborhoodSize);

	//get angle from corners
	float angle = getAngleCercles(circlesImg);

	//affichage images intermediaires
	//decommenter la section pour enregistrer les images intermediaires
	/*
	pnm_save(expoImg, PnmRawPpm, "intermediary-exponentielle.ppm");
	getGrayImg(gray, rows, cols);
	getMoyImg(moyennes, rows, cols);
	pnm_save(circlesImg, PnmRawPpm, "intermediary-circleLocation.ppm");
	*/
	free(expoImg);
	free(gray);
	for(int i=0;i<rows;i++){
		free(moyennes[i]);
	}
	free(moyennes);
	free(circlesImg);
	
	return angle;
}

void process(char* ims_name, char* imd_name){
	pnm ims = pnm_load(ims_name);
	
	int cols = pnm_get_width(ims);
	int rows = pnm_get_height(ims);

	int neighborhoodSize = rows/E_TRAIT;

	float angleCorners = processCorners(ims, rows, cols, neighborhoodSize);

	printf("Rotation de %f degrès à effectuer.\n", angleCorners);


	rotateImg(angleCorners, ims, imd_name);
}

void usage (char *s){
	fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
	exit(EXIT_FAILURE);
}

#define PARAM 2
int main(int argc, char *argv[]){
	if (argc != PARAM+1) usage(argv[0]);	
	char *ims_name = argv[1];
	char *imd_name = argv[2];
	
	process(ims_name,imd_name);
	return EXIT_SUCCESS;
}
