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
	int cols = pnm_get_width(ims);
	int rows = pnm_get_height(ims);

	int newCols = factor*cols;
	int newRows = factor*rows;

	//get canal
	unsigned short *canal = malloc(cols*rows*sizeof(unsigned short));
	for(int i=0; i<rows;i++){
		for(int j=0;j<cols;j++){
			*canal = pnm_get_component(ims,i,j,0);
			canal++;
		}
	}
	canal -= cols*rows;

	//Transformation de fourrier
	fftw_complex *forwardCanal;
	forwardCanal = forward(rows, cols, canal);

	//centrer
	fftw_complex *centerImage = malloc(newRows*newCols*sizeof(fftw_complex));

	int midNewRows = newRows/2;
	int midNewCols = newCols/2;

	int midRows = rows/2;
	int midCols = cols/2;

	for (int i = 0; i < newRows; ++i)
	{
		for (int j = 0; j < newCols; ++j){
		   if((i>=(midNewRows-midRows))&&
		    	 (i<(midNewRows+midRows))&&
		    	 (j>=(midNewCols-midCols))&&
		    	 (j<(midNewCols+midCols)) ){
		   	*centerImage = *forwardCanal;
				forwardCanal++;
		   } else{
	   		*centerImage = 0;
	   	}
	   	centerImage++;     	 
		}
	}

	centerImage -= newRows*newCols;
	forwardCanal -= rows*cols;

	//transformation de fourrier inverse
	unsigned short* backwardCanal;
	backwardCanal = backward(newRows,newCols,centerImage,factor);

	//recreer image a partir du canal
	pnm newImage = pnm_new(newCols, newRows, PnmRawPpm);
	for(int i=0;i<newRows;i++){
		for(int j=0; j<newCols;j++){
			for(int k=0;k<3;k++){
				pnm_set_component(newImage,i,j,k,backwardCanal[(i*newCols)+j]);
			}
		}
	}
	pnm_save(newImage,PnmRawPpm,name);	
	free(canal);
	pnm_free(newImage);
}

void usage (char *s){
	fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
	if (argc != 4)
		usage(argv[0]);
	int factor = atoi(argv[1]);
	pnm ims = pnm_load(argv[2]);
	char * name = argv[3];
	run(factor,ims, name);

	return EXIT_SUCCESS;
}