/**
 * @file color-transfer
 * @brief transfert color from source image to target image.
 *        Method from Reinhard et al. : 
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley, 
 *        'Color Transfer between Images', IEEE CGA special issue on 
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcl.h>

#define D 3

/*static float RGB2LMS[D][D] = {
  {0.3811, 0.5783, 0.0402}, 
  {0.1967, 0.7244, 0.0782},  
  {0.0241, 0.1288, 0.8444}
  };*/

// Conversion Unsigned short to float
float** convert_to_float(pnm RGB){
	printf("CONVERTING TO FLOAT \n");
	int rows = pnm_get_height(RGB);
	int cols = pnm_get_width(RGB);

	unsigned short *oldStart = pnm_get_image(RGB);
	unsigned short *oldCurrent = pnm_get_image(RGB);

	float** ret = malloc(3*sizeof(float*));
	ret[0] = malloc(sizeof(float)*rows*cols);
	ret[1] = malloc(sizeof(float)*rows*cols);
	ret[2] = malloc(sizeof(float)*rows*cols);

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


pnm convert_to_unsigned_short(float** matrix,int rows, int cols){
		printf("CONVERTING TO UNSIGNED \n");
  pnm img = pnm_new(cols, rows, PnmRawPpm);

  unsigned short *current = pnm_get_image(img);
  unsigned short *start = pnm_get_image(img);

	for(int i=0;i<rows; i++){
		for(int j=0;j<cols;j++){
			current = start + pnm_offset(img, i,j);
			
			*current = (unsigned short)matrix[0][(i*cols)+j];
			current++;
			*current = (unsigned short)matrix[1][(i*cols)+j];
			current++;
			*current = (unsigned short)matrix[2][(i*cols)+j];
			current++;
		}
	}
  return img;
}


static void process(char *ims, char *imt, char* imd){
  (void) ims;
  (void) imt;
  (void) imd;

  

  //Etape 1 : RGB -> LMS -> lalphabeta
 

    

  // RGB -> LMS
  // Conversion en log (attention en 0 -> noir)
  // LMS -> lalphabeta

  //Etape 2 : Transfert des stats

  //Etape 3 : lalphabeta -> LMS -> RGB
  // lalphabeta -> LMS
  // Convertion exponentielle
  // LMS -> RGB
  // Troncature, gérer les "pixels cramés", ramener pixels entre 0 et 255


}

void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[]){

  pnm ims = pnm_load(argv[1]);
  float ** test = convert_to_float(ims);
    int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  pnm imd = convert_to_unsigned_short(test,rows,cols);
    pnm_save(imd, PnmRawPpm, "a.ppm");

  


  if (argc != param+1) 
    usage(argv[0]);
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;
}
