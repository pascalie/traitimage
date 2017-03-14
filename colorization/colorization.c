/**
 * @file colorization
 * @brief Transferring Color to Greyscale Images
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
#define NEIGHBORHOODHALF 2
#define NEIGHBORHOODSIZE NEIGHBORHOODHALF*2+1


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

// Calcul d'un entier aléatoire entre A et B
int randAB(int a, int b){
    return rand()%(b-a) +a;
}

// Méthode d'adaptation de la luminance
void luminanceRemapping(float* couleur, float* black, int sizeCouleur, int sizeBlack){
	float Ecouleur = ecartType(couleur,sizeCouleur);
	float Eblack = ecartType(black, sizeBlack);

	float Mcouleur = moyenne(couleur, sizeCouleur);
	float Mblack = moyenne(black, sizeBlack);

	for (int i = 0; i < sizeCouleur; i++){
		couleur[i] = ((Eblack*(couleur[i] - Mcouleur))/Ecouleur) + Mblack; 
	}
}

// Calcul du voisinage d'un pixel
float* pixelNeighborhood(float* image, int rows, int cols, int abs, int ord, int *retSize){
	int distHG = NEIGHBORHOODHALF;
	int distHD = NEIGHBORHOODHALF;
	int distVH = NEIGHBORHOODHALF;
	int distVB = NEIGHBORHOODHALF;
	if(abs < NEIGHBORHOODHALF){
		distHG = abs;
	}

	if(abs > cols-NEIGHBORHOODHALF-1){
		distHD = cols - 1 - abs;
	}

	if(ord < NEIGHBORHOODHALF){
		distVH = ord;
	}

	if(ord > rows-NEIGHBORHOODHALF-1){
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

	for (int i = 0; i < tmpWidth; i++){
		free(tmp[i]);
	}
	free(tmp);
	return tmpLinear;
}


// Création d'une matrice contenant les moyennes et les écart-types du voisinage d'un pixel
float** statisticsOfCouleur(float* couleur, int** grid, int rowsCouleur, int colsCouleur){
	float** ret = malloc(2*sizeof(float*));
	//luminance moyenne
	ret[0] = malloc((GRIDPOINTS)*sizeof(float));
	
	//luminance ecart type
	ret[1] = malloc((GRIDPOINTS)*sizeof(float));
	for(int k=0; k <GRIDPOINTS; k++){
		int* tmpLinearSize = malloc(sizeof(int));
		float *tmpLinear = pixelNeighborhood(couleur, rowsCouleur, colsCouleur, grid[1][k], grid[0][k], tmpLinearSize);
		ret[0][k] = moyenne(tmpLinear, *tmpLinearSize);
		ret[1][k] = ecartType(tmpLinear, *tmpLinearSize);
	}
	
	return ret;
}

/* Création d'une grille répartisant aléatoirement des points en divisant le rectangle initial 
selon les constantes GRIDPOINTS et GRIDSIZE, puis un point est choisie aléatoirement dans chaque
rectangle.
La fonction retourne un tableau contenant la liste des abscisses et la liste des ordonnées de ces points. */
int** randomGrid(int rows, int cols){
	if(rows < (GRIDSIZE) || cols < (GRIDSIZE)){
		exit(EXIT_FAILURE);
	}

	int** ret = malloc(2*sizeof(int*));
	ret[0] = malloc((GRIDPOINTS)*sizeof(int));
	ret[1] = malloc((GRIDPOINTS)*sizeof(int));

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


// Calcul de la distance entre la luminance moyenne et l'écart-type des deux matrices 
float distance(float a, float b, float x, float y){
	return sqrt((a-x)*(a-x) + (b-y)*(b-y));
}


// Fonction testant la génération aléatoire des points, elle n'est pas utilisée par la suite
void testRandGrid(int rows, int cols, char* name){
	pnm imd = pnm_new(cols, rows, PnmRawPpm);

	int** grid = randomGrid(rows, cols);	

	for(int i=0; i<GRIDPOINTS; i++){
		for(int k=0; k<3;k++){
			pnm_set_component(imd, grid[0][i],grid[1][i], k,255);
		}
	}

	pnm_save(imd, PnmRawPpm, name);

}


// Fonction qui choisit le pixel le plus semblable à chaque pixel de l'image parmis la liste des points aléatoire 
int matchingPixel(float *neighborhood, float** statistics, int* sizeNeighborhood){
	float moy = moyenne(neighborhood, *sizeNeighborhood);
	float ecType = ecartType(neighborhood, *sizeNeighborhood);

	int ret;

	float min = distance(moy, ecType, statistics[0][0], statistics[1][0]);
	
	float tmp = min;
	for(int i=1; i<GRIDPOINTS;i++){
		tmp = distance(moy, ecType, statistics[0][i], statistics[1][i]);
		if(tmp <= min){
			ret = i;
			min = tmp;
		}
	}
	return ret;
}


// Fonction qui copie les champs alpha et béta des points sélectionnés sur l'image colorée dans l'image en noir et blanc
void colorize(float** black, float** couleur, float** statistics, int** jitteredGrid, int rowsBlack, int colsBlack, int colsCouleur){
	for(int i=0; i<colsBlack;i++){
		for (int j = 0; j < rowsBlack; j++){
			int* neighborhoodSize = malloc(sizeof(int));
			float* neighborhood = pixelNeighborhood(black[0], rowsBlack, colsBlack, i, j, neighborhoodSize);

			int matchPixel = matchingPixel(neighborhood, statistics, neighborhoodSize);
			int absMatching = jitteredGrid[0][matchPixel];
			int ordMatching = jitteredGrid[1][matchPixel];

			black[1][j*colsBlack+i] = couleur[1][ordMatching*colsCouleur+absMatching];
			black[2][j*colsBlack+i] = couleur[2][ordMatching*colsCouleur+absMatching];

			free(neighborhoodSize);
		}
	}
}


static void process(char *ims, char *imt, char* imdname){
	pnm source = pnm_load(imt);//couleur
	pnm target = pnm_load(ims);//gris

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
	luminanceRemapping(labtarget[0], labsource[0], size, sizeTarget);
	//calcul moyenne / ecart type pour remapping

	//Etape 3 : Génération d'une liste aléatoire de points
	int** jitteredGrid = randomGrid(rowsTarget, colsTarget);

	//Etape 4 : Transformation finale
	float** statistics = statisticsOfCouleur(labtarget[0], jitteredGrid, rowsTarget, colsTarget);

	colorize(labsource, labtarget, statistics, jitteredGrid, rows, cols,colsTarget);

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

	//Libération de la mémoire allouée
	//flottants
	for(int i=0; i<3;i++){
		free(sourcef[i]);
	}
	free(sourcef);
	for(int i=0; i<3;i++){
		free(targetf[i]);
	}
	free(targetf);

	//lms
	for(int i=0; i<3;i++){
		free(lmssource[i]);
	}
	free(lmssource);
	for(int i=0; i<3;i++){
		free(lmstarget[i]);
	}
	free(lmstarget);

	//lab
	for(int i=0; i<3;i++){
		free(labsource[i]);
	}
	free(labsource);
	for(int i=0; i<3;i++){
		free(labtarget[i]);
	}
	free(labtarget);

	//jitteredGrid
	for(int i=0; i<2;i++){
		free(jitteredGrid[i]);
	}
	free(jitteredGrid);

	//statistics
	for(int i=0; i<2;i++){
		free(statistics[i]);
	}
	free(statistics);

	//imd
	for(int i=0; i<3;i++){
		free(lmsimd[i]);
	}
	free(lmsimd);
	for(int i=0; i<3;i++){
		free(rgbimd[i]);
	}
	free(rgbimd);
}


void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[]){

	srand(time(NULL));
	
	if (argc != param+1) 
    	usage(argv[0]);
	process(argv[1], argv[2], argv[3]);
	printf("Image colorisée.\n");
	return EXIT_SUCCESS;
}
