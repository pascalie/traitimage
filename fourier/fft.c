#include <float.h>
#include <stdlib.h>
#include <math.h>

#include "fft.h"

fftw_complex *forward(int rows, int cols, unsigned short* g_img){
   //Conversion en complexe
   fftw_complex* cg_img = malloc(rows*cols*sizeof(fftw_complex));
   for (int i = 0; i < rows* cols; ++i){
      cg_img[i] = (float)g_img[i] + I*0;
   }
   fftw_complex* cg_result = malloc(rows*cols*sizeof(fftw_complex)); //TO FREE

   //Transformation de Fourier

   fftw_plan plan = fftw_plan_dft_2d(rows,cols,cg_img,cg_result,FFTW_FORWARD,FFTW_ESTIMATE);
   fftw_execute(plan);
   fftw_destroy_plan(plan);
 
   free(cg_img);
   return cg_result;
}


unsigned short convert_to_unsigned_short(float a){
   if( a < 0 )
      return (unsigned short)0;
   else if (a > 255)
      return (unsigned short)255;
   else
      return (unsigned short)a;
}

unsigned short *backward(int rows, int cols, fftw_complex* freq_repr){
   fftw_complex* freq_result = malloc(rows*cols*sizeof(fftw_complex)); //TO FREE
   fftw_plan plan = fftw_plan_dft_2d(rows,cols,freq_repr,freq_result,FFTW_BACKWARD,FFTW_ESTIMATE);
   fftw_execute(plan);
   fftw_destroy_plan(plan);
   
   unsigned short* ret = malloc(cols*rows*sizeof(unsigned short));

   for (int i = 0; i < rows* cols; ++i){
      float tmp = creal(freq_result[i])/(rows*cols); 
      ret[i] = convert_to_unsigned_short(tmp);
   }
   free(freq_result);
   return ret;

}


void freq2spectra(int rows, int cols, fftw_complex* freq_repr, float* as, float* ps) 
{
  (void)rows;
  (void)cols;
  (void)freq_repr;
  (void)as;
  (void)ps;
}


void spectra2freq(int rows, int cols, float* as, float* ps, fftw_complex* freq_repr)
{
  (void)rows;
  (void)cols;
  (void)freq_repr;
  (void)as;
  (void)ps;
}
