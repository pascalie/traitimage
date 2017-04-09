#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "fft.h"

fftw_complex *forward(int rows,int cols,unsigned short *image){
  int size=rows*cols;
  fftw_complex *img_in=malloc(size*sizeof(fftw_complex));
  fftw_complex *img_out=malloc(size*sizeof(fftw_complex));

  //centrer
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++){
      if((i+j)%2 == 0)
        *img_in = *image +I*0;
      else
        *img_in = -1.0* (*image) + I*0;
      image++;
      img_in++;
    }
  }
  image -= cols*rows;
  img_in -= cols*rows;

  //transformation de fourrier
  fftw_plan plan=fftw_plan_dft_2d(rows,cols,img_in,img_out,FFTW_FORWARD,FFTW_ESTIMATE);

  fftw_execute(plan);fftw_destroy_plan(plan);free(img_in);return img_out;
}

unsigned short *backward(int rows, int cols, fftw_complex *freq_repr,int factor){
  int size=rows*cols;

  fftw_complex *fftw_back=malloc(size*sizeof(fftw_complex));
  unsigned short *img_out=malloc(size*sizeof(unsigned short));

  fftw_plan plan=fftw_plan_dft_2d(rows,cols,freq_repr,fftw_back,FFTW_BACKWARD,FFTW_ESTIMATE);
  fftw_execute(plan);

  //recentrer
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      float tmp = creal(*fftw_back)*factor*factor/(rows*cols);

      if((i+j)%2 == 1){
        tmp = -tmp;
      }

      if(tmp>255){
        tmp = 255;
      } else if (tmp<0){
        tmp = 0;
      }

      *img_out = (unsigned short)tmp;

      fftw_back++;
      img_out++;
    }
  }

  img_out -= rows*cols;
  fftw_back -= rows*cols;

  fftw_destroy_plan(plan);
  free(fftw_back);
  return img_out;
}

