#include <float.h>
#include <stdlib.h>
#include <math.h>

#include "fft.h"

fftw_complex *forward(int rows, int cols, unsigned short* image)
{
  //Transformaton en complexe
  fftw_complex* img_in = (fftw_complex*) fftw_malloc(rows*cols*sizeof(fftw_complex));
  fftw_complex* img_out = (fftw_complex*) fftw_malloc(rows*cols*sizeof(fftw_complex));

  for (int i = 0; i < rows; ++i)
  {
    //centrage
    for (int j = 0; j < cols; ++j)
    {
      if ((i+j) % 2 == 0)
        *img_in = *image + I*0;
      else
        *img_in = - (double) (*image) + I*0;
      image+=3;
      img_in++;
    }
  }
  img_in -= cols*rows;
  image -= cols * rows * 3;

  //Transformation de fourrier
  fftw_plan p = fftw_plan_dft_2d(rows, cols, img_in, img_out, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(p);


  fftw_destroy_plan(p);
  fftw_free(img_in);

  return img_out;
}


unsigned short 
*backward(int rows, int cols, fftw_complex* freq_repr)
{
  //Transformation de fourrier inverse
  unsigned short * img_out = malloc(rows*cols*sizeof(unsigned short)*3);

  fftw_complex* fftw_back = (fftw_complex*) fftw_malloc(rows*cols*sizeof(fftw_complex));
  fftw_plan p = fftw_plan_dft_2d(rows, cols, freq_repr, fftw_back, FFTW_BACKWARD, FFTW_ESTIMATE);
  

  fftw_execute(p);
  fftw_destroy_plan(p);

  float tmp;

  //recentrange
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      (*fftw_back)*=(1./(rows*cols));
      for (int k = 0; k < 3; ++k)
      {
        if ((i+j) % 2 == 0){
          tmp = creal(*fftw_back);
        }
        else{
          tmp = -creal(*fftw_back);
        }
        if(tmp>255)
          *img_out = 255;
        else if(tmp<0)
          *img_out = 0;
        else
          *img_out = (unsigned short) tmp;
        img_out++;
      }
      fftw_back++;
    }
  }
  img_out -= cols*rows*3;
  fftw_back -= cols*rows;


  fftw_free(fftw_back);
  return img_out;
}

//Génération des spectres de fréquence / phase
void freq2spectra(int rows, int cols, fftw_complex* freq_repr, float* as, float* ps) 
{
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {

      for (int k = 0; k < 3; ++k)
      {
        *as = sqrtf( creal(*freq_repr)*creal(*freq_repr) + cimag(*freq_repr)*cimag(*freq_repr));
        *ps = atan2f( cimag(*freq_repr), creal(*freq_repr) );
        as++;
        ps++;
      }
      freq_repr++;
    }
  }
  as -= 3*cols*rows;
  ps -= 3*rows*cols;
  freq_repr -= rows*cols;
}


//Génération d'image à partir des spectres
void spectra2freq(int rows, int cols, float* as, float* ps, fftw_complex* freq_repr)
{
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      *freq_repr = *as * cos(*ps) + I * *as * sin(*ps);
      freq_repr++;
      as+=3;
      ps+=3;
    }
  }

  as -= 3*cols*rows;
  ps -= 3*rows*cols;
  freq_repr-= cols*rows;
}

