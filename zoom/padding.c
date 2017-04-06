/**
 * @file padding.c
 * @brief 
 *
 */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bcl.h>
#include "fft.h"

void run(int factor,pnm ims,char * name){
	int rows = pnm_get_height(ims);
	int cols = pnm_get_width(ims); 
	int new_rows = rows*factor;
	int new_cols = cols*factor;
	pnm imd = pnm_new(new_cols,new_rows,PnmRawPpm);
	
   	unsigned short *ps = pnm_get_image(ims);
   	unsigned short *pd = pnm_get_image(imd);

   	
   	fftw_complex *fftw_for;
   
   	//Transformation de fourrier
   	fftw_for = forward(rows, cols, ps);

  

   	//Centrer l'image dans l'autre
    fftw_complex* fftw_big = (fftw_complex*) fftw_malloc(new_rows*new_cols*sizeof(fftw_complex));

    for (int i = 0; i < new_cols*new_rows; ++i)
    {
    	fftw_big[i] = 0*I + 0;
    }
    /*

    for (int i = 0; i < cols; ++i)
    {
    	for (int j = 0; j < rows; ++j)
    	{
    		for (int k = 0; k < 3; ++k)
    		{
    			unsigned short value = pnm_get_component(ims,i,j,k);
    			pnm_set_component(imd,(factor*cols)/2 - (int)(cols/2) + i ,(factor*rows)/2 - (int)(rows/2) + j,k,value);
    		}
    		//fftw_big[(i*cols)+j] = fftw_for[(i*cols)+j];
    	}
    }

    */

     for (int i = 0; i < cols; ++i)
    {
    	for (int j = 0; j < rows; ++j)
    	{
    	
  				fftw_big[(((new_cols)/2 - (int)(cols/2) + i )*cols)+((new_rows)/2 - (int)(rows/2) + j)] = fftw_for[(i*cols)+j];
    			//printf("%f\n",creal(fftw_big[i*new_cols+j]) );
    		//fftw_big[(i*cols)+j] = fftw_for[(i*cols)+j];
    	}
    }

   	//Transformation inverse
   	unsigned short *out = backward(new_rows, new_cols, fftw_big, factor);

    for(int i=0;i<new_rows;i++){
      for(int j=0;j<new_cols;j++){
       for(int c=0; c<3; c++){
         *pd = *out;
         pd++;
         out++;
       }
     }
   }
   pd -= 3*new_cols*new_rows;
   out -= 3*new_cols*new_rows;
 
   pnm_save(imd, PnmRawPpm, name);
   fftw_free(fftw_for);
   fftw_free(fftw_big);
   pnm_free(imd);
   free(out);
   fprintf(stderr, "OK\n");



	}

void usage (char *s)
{
  fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  if (argc != 4)
    usage(argv[0]);
  int factor = atoi(argv[1]);
  pnm ims = pnm_load(argv[2]);
  char * name = argv[3];
  run(factor,ims, name);

  return EXIT_SUCCESS;
}