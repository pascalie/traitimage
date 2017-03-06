/**
 * @file color-transfer
 * @brief transfert color from source image to target image.
 *        Method from Reinhard et al. : 
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley, 
 *        'Color Transfer between Images', IEEE CGA special issue on 
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 * @authors HENRY Pascalie & GRUCHET Sébastien
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcl.h>
#include <time.h>


#define D 3
#define GRIDSIZE 15
#define GRIDPOINTS GRIDSIZE*GRIDSIZE

static float RGB2LMS[D][D] = {
	{0.3811, 0.5783, 0.0402}, 
	{0.1967, 0.7244, 0.0782},	
	{0.0241, 0.1288, 0.8444}
	};

static float LMS2RGB[D][D] = {
	{4.4679, -3.5873, 0.1193}, 
	{-1.2186, 2.3809, -0.1624},	
	{0.0497, -.2439, 1.2045}
	};

static float LMS2LAB[D][D] = {
	{0.57735026919, 0.57735026919, 0.57735026919},
	{0.40824829046, 0.40824829046, -0.81649658092},
	{0.70710678118, -0.70710678118,0}
};

static float LAB2LMS[D][D] = {
	{0.57735026919, 0.40824829046, 0.70710678118},
	{0.57735026919, 0.40824829046, -0.70710678118},
	{0.57735026919, -0.81649658092, 0}
};


// Conversion Unsigned short to float
float** convert_to_float(pnm RGB){
	int rows = pnm_get_height(RGB);
	int cols = pnm_get_width(RGB);

	unsigned short *oldStart = pnm_get_image(RGB);
	unsigned short *oldCurrent = pnm_get_image(RGB);

	float** ret = malloc(3*sizeof(float*));
	for(int k = 0; k<3; k++)
		ret[k] = malloc(sizeof(float)*rows*cols);
	
	unsigned short* R = malloc(sizeof(unsigned short)*cols*rows);
	for(int i=0;i<rows; i++){
		for(int j=0;j<cols;j++){
			oldCurrent = oldStart + pnm_offset(RGB, i,j);
			R[(i*cols)+j] = *oldCurrent;
		}
	}

	unsigned short* G = malloc(sizeof(unsigned short)*cols*rows);
	for(int i=0;i<rows; i++){
		for(int j=0;j<cols;j++){
			oldCurrent = oldStart + pnm_offset(RGB, i,j);
			oldCurrent++;
			G[(i*cols)+j] = *oldCurrent;
		}
	}

	unsigned short* B = malloc(sizeof(unsigned short)*cols*rows);
	for(int i=0;i<rows; i++){
		for(int j=0;j<cols;j++){
			oldCurrent = oldStart + pnm_offset(RGB, i,j);
			oldCurrent+=2;
			B[(i*cols)+j] = *oldCurrent;
		}
	}

	for(int i=0;i<rows; i++){
		for(int j=0;j<cols;j++){
			ret[0][(i*cols)+j] = (float)R[(i*cols)+j];
			ret[1][(i*cols)+j] = (float)G[(i*cols)+j];
			ret[2][(i*cols)+j] = (float)B[(i*cols)+j];
		}
	}
    	return ret;
}

// Conversion float to unsigned short
pnm convert_to_unsigned_short(float** matrix,int rows, int cols){
	pnm img = pnm_new(cols, rows, PnmRawPpm);

	unsigned short *current = pnm_get_image(img);
	unsigned short *start = pnm_get_image(img);

	for(int i=0;i<rows; i++){
		for(int j=0;j<cols;j++){
			current = start + pnm_offset(img, i,j);
			for(int k=0; k<3; k++){
				*current = (unsigned short)matrix[k][(i*cols)+j];
				current++;
			}
		}
	}
	return img;
}


// Produit de deux matrices de taille 3*3
float** produitMat(float** image, float matrice[][3], int size){
	float **ret = malloc(sizeof(float*)*3);
	for(int k=0; k<3;k++){
		ret[k]=malloc(sizeof(float)*size);
	}

	for(int k=0; k<3; k++){	
		for(int i=0; i<size;i++){
			ret[k][i] = 0;
			for(int j=0;j<3;j++){
				ret[k][i] += matrice[k][j]*image[j][i];
			}
		}
	}

	return ret;
}


// Passage au logarithme en base 10
void logMatrice(float** image, int size){
	for(int k=0; k<3; k++){
		for (int i = 0; i < size; i++){
			if(image[k][i] > 0)
				image[k][i] = log10f(image[k][i]);	
			else {
				image[k][i] = 0.0001;
				image[k][i] = log10f(image[k][i]);
			}
		}
	}
}

// Fonction inverse 
void puissance10(float** image, int size){
	for(int k=0; k<3; k++){
		for (int i = 0; i < size; i++){
			image[k][i] = powf(10,image[k][i]);				
		}
	}
}


// Calcul de la moyenne
float moyenne(float* tab, int size){
	float somme = 0;
	for (int i = 0; i < size; i++){
		somme += tab[i];				
	}
	return somme/size;
}


// Calcul de l'ecart-type
float ecartType(float* tab, int size){
	float somme = 0;
	for (int i = 0; i < size; i++)
		somme += (tab[i]*tab[i]);				
	float mean = moyenne(tab, size);
	float mean2 = mean*mean;
	float ecartType = sqrtf( (somme/size) - mean2 );
	return ecartType;
} 

int randAB(int a, int b){
    return rand()%(b-a) +a;
}

void luminanceRemapping(float* couleur, float* black, int sizeCouleur, int sizeBlack){

	float Ecouleur = ecartType(couleur,sizeCouleur);
	float Eblack = ecartType(black, sizeBlack);

	float Mcouleur = moyenne(couleur, sizeCouleur);
	float Mblack = moyenne(black, sizeBlack);

	for (int i = 0; i < sizeCouleur; i++){
		couleur[i] = ((Eblack*(couleur[i] - Mcouleur))/Ecouleur) + Mblack; 
	}
}

float* pixelNeighborhood(float* image, int rows, int cols, int abs, int ord, int *retSize){
	int distHG = 2;
	int distHD = 2;
	int distVH = 2;
	int distVB = 2;
	if(abs < 2){
		distHG = abs;
	}

	if(abs > cols-3){
		distHD = cols - 1 - abs;
	}

	if(ord < 2){
		distVH = ord;
	}

	if(ord > rows-3){
		distVB = rows - 1 - ord;
	}

	int tmpWidth = distHG + distHD +1;
	int tmpHeight = distVH + distVB +1;

	*retSize = tmpWidth*tmpHeight;

	float** tmp = malloc(sizeof(float*) * tmpWidth);
	for(int l=0; l<tmpWidth;l++){
		tmp[l] = malloc(sizeof(float)* tmpHeight);
	}

	float* tmpLinear = malloc(sizeof(float) * tmpWidth * tmpHeight);

	int J0 = ord - distVH;
	int I0 = abs - distHG;
	for(int i=0; i<tmpWidth;i++){
		for(int j=0; j<tmpHeight; j++){
			tmp[i][j] = image[(J0+j)*cols+I0+i];
		}
	}

	for(int i=0; i< tmpWidth; i++){
		for (int j = 0; j < tmpHeight; j++){
			tmpLinear[j+tmpWidth+i] = tmp[i][j];
		}
	}

	return tmpLinear;
}

float** statisticsOfCouleur(float* couleur, float** grid, int rowsCouleur, int colsCouleur){
	float** ret = malloc(2*sizeof(float*));
	//luminance moyenne
	ret[0] = malloc((GRIDPOINTS)*sizeof(float));
	
	//luminance ecart type
	ret[1] = malloc((GRIDPOINTS)*sizeof(float));


	for(int k=0; k <GRIDPOINTS; k++){
		int* tmpLinearSize = malloc(sizeof(int));
		float *tmpLinear = pixelNeighborhood(couleur, rowsCouleur, colsCouleur, grid[0][k], grid[1][k], tmpLinearSize);
		ret[0][k] = moyenne(tmpLinear, *tmpLinearSize);
		ret[1][k] = ecartType(tmpLinear, *tmpLinearSize);		
	/*
		int distHG = 2;
		int distHD = 2;
		int distVH = 2;
		int distVB = 2;
		if(grid[0][k] < 2){
			distHG = grid[0][k];
		}

		if(grid[0][k] > colsCouleur-3){
			distHD = colsCouleur - 1 - grid[0][k];
		}

		if(grid[1][k] < 2){
			distVH = grid[1][k];
		}

		if(grid[1][k] > rowsCouleur-3){
			distVB = rowsCouleur - 1 - grid[1][k];
		}

		int tmpWidth = distHG + distHD +1;
		int tmpHeight = distVH + distVB +1;

		float** tmp = malloc(sizeof(float*) * tmpWidth);
		for(int l=0; l<tmpWidth;l++){
			tmp[l] = malloc(sizeof(float)* tmpHeight);
		}

		float* tmpLinear = malloc(sizeof(float) * tmpWidth * tmpHeight);

		int J0 = grid[1][k] - distVH;
		int I0 = grid[0][k] - distHG;
		for(int i=0; i<tmpWidth;i++){
			for(int j=0; j<tmpHeight; j++){
				tmp[i][j] = couleur[(J0+j)*colsCouleur+I0+i];
			}
		}

		for(int i=0; i< tmpWidth; i++){
			for (int j = 0; j < tmpHeight; j++){
				tmpLinear[j+tmpWidth+i] = tmp[i][j];
			}
		}

		free(tmp);

		ret[0][k] = moyenne(tmpLinear, tmpWidth*tmpHeight);
		ret[1][k] = ecartType(tmpLinear, tmpWidth*tmpHeight);

		free(tmpLinear);
	*/
	}
	

	return ret;
}


/*
	ret[0] => abscisse
	ret[1] => ordonnees
*/
float** randomGrid(int rows, int cols){
	if(rows < (GRIDPOINTS) || cols < (GRIDPOINTS)){
		exit(EXIT_FAILURE);
	}

	float** ret = malloc(2*sizeof(float*));
	ret[0] = malloc((GRIDPOINTS)*sizeof(float));
	ret[1] = malloc((GRIDPOINTS)*sizeof(float));

	int width = rows / GRIDSIZE;
	int height = cols / GRIDSIZE;

	for(int i=0; i<GRIDSIZE;i++){
		for(int j=0; j<GRIDSIZE;j++){
			ret[0][j*GRIDSIZE+i] = randAB(i*width, (i+1)*width);
			ret[1][j*GRIDSIZE+i] = randAB(j*height, (j+1)*height);
		}
	}

	return ret;
}

float distance(float a, float b, float x, float y){
	return sqrt((a-x)*(a-x) + (b-y)*(b-y));
}

float minimum(float a, float b){
	if(a < b){
		return a;
	} else {
		return b;
	}
}



void testRandGrid(int rows, int cols, char* name){
	pnm imd = pnm_new(cols, rows, PnmRawPpm);

	float** grid = randomGrid(rows, cols);	

	for(int i=0; i<GRIDPOINTS; i++){
		for(int k=0; k<3;k++){
			pnm_set_component(imd, grid[0][i],grid[1][i], k,255);
		}
	}

	pnm_save(imd, PnmRawPpm, name);

}

int matchingPixel(float *neighborhood, float** statistics, int* sizeNeighborhood){
	//TODO
}



static void process(char *ims, char *imt, char* imdname){
	pnm source = pnm_load(imt);
	pnm target = pnm_load(ims);

	int cols = pnm_get_width(source);
	int rows = pnm_get_height(source);
	int size = rows*cols;

	int colsTarget = pnm_get_width(source);
	int rowsTarget = pnm_get_height(source);
	int sizeTarget = rowsTarget*colsTarget;

	pnm imd = pnm_new(cols, rows, PnmRawPpm);


	//Etape 1 : RGB -> LMS -> lalphabeta

	// unsigned short -> float
	float ** sourcef = convert_to_float(source);
	float ** targetf = convert_to_float(target);

	// RGB -> LMS
	float** lmssource = produitMat(sourcef, RGB2LMS, size);
	float** lmstarget = produitMat(targetf, RGB2LMS, size);

	// Conversion en log (attention en 0 -> noir)
	logMatrice(lmssource,size);
	logMatrice(lmstarget,size);

	// LMS -> lalphabeta
	float** labsource = produitMat(lmssource, LMS2LAB,size);
	float** labtarget = produitMat(lmstarget, LMS2LAB,size);



	//Etape 2 : Luminance remapping
	luminanceRemapping(labsource[0], labtarget[0], size, sizeTarget);


	//Etape 3 : Génération d'une liste aléatoire de points
	float** jitteredGrid = randomGrid(rows, cols);

	//Etape 4 : Transformation finale
	float** statistics = statisticsOfCouleur(labsource[0], jitteredGrid, rows, cols);





 	 //Etape 1 bis (transformation inverse) : lalphabeta -> LMS -> RGB
	
    // lalphabeta -> LMS
	float** lmsimd = produitMat(labsource, LAB2LMS, size);

  	// Convertion exponentielle
	puissance10(lmsimd,size);

	// LMS -> RGB
  	// Troncature, gérer les "pixels cramés", ramener pixels entre 0 et 255
	float** rgbimd = produitMat(lmsimd,LMS2RGB,size);
	for (int k = 0; k < 3; k++)
	{
		for (int i = 0; i < size; i++)
		{
			if(rgbimd[k][i] > 255){
				rgbimd[k][i] = 255;
			} else if(rgbimd[k][i] < 0){
				rgbimd[k][i] = 0;
			}
		}
	}

	// float -> unsigned short
	imd = convert_to_unsigned_short(rgbimd,rows,cols);


	pnm_save(imd, PnmRawPpm, imdname);
}


void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  testRandGrid(720, 1024, "a.ppm");
  testRandGrid(720, 1024, "b.ppm");
  testRandGrid(720, 1024, "c.ppm");
  testRandGrid(720, 1024, "d.ppm");
  testRandGrid(720, 1024, "e.ppm");

  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[]){
	srand(time(NULL));
	
	if (argc != param+1) 
    	usage(argv[0]);
	process(argv[1], argv[2], argv[3]);
	return EXIT_SUCCESS;
}
