/**
 * @file test-fft.c
 * @brief test the behaviors of functions in fft module
 *
 */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fft.h"

//Renvoie la valeur maximale d'un tableau de float
float maxValue(float *as, int rows, int cols)
{
  float max = 0;

  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      for (int k = 0; k < 3; ++k)
      {
        if(*as > max)
          max = *as;

        as++;
      }
    }
  }
  as -= cols*rows*3;
  return max;
}


int find_last_slash(char* a){
  int ret=0;
  for(int i=0; a[i] != '\0'; i++){
    if(a[i] == '/')
      ret = i;
  }
  ret++;
  return ret;
}

/**
 * @brief test the forward and backward functions
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
 static void test_for_backward(pnm ims, char* name){

   fprintf(stderr, "test_for_backward: ");
   int cols = pnm_get_width(ims);
   int rows = pnm_get_height(ims);

   pnm imd = pnm_new(cols, rows, PnmRawPpm);

   unsigned short *ps = pnm_get_image(ims);
   unsigned short *pd = pnm_get_image(imd);

   fftw_complex *fftw_for;
   
   //Transformation de fourrier
   fftw_for = forward(rows, cols, ps);
   //Transformation inverse
   unsigned short *out = backward(rows, cols, fftw_for);

   for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      for(int c=0; c<3; c++){
        *pd = *out;
        pd++;
        out++;
      }
    }
  }
  pd -= 3*cols*rows;
  out -= 3*cols*rows;
 
  //Gestion des noms
  char tmp_name[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmp_name[i-find_last_slash(name)] = name[i];
  }

  char dest_name [30];
  strcpy(dest_name,"FB-");
  strcat(dest_name,tmp_name);
  pnm_save(imd, PnmRawPpm, dest_name);
  fftw_free(fftw_for);
  pnm_free(imd);
  free(out);
  fprintf(stderr, "OK\n");
}


/**
 * @brief test image reconstruction from of amplitude and phase spectrum
 * @param pnm ims: the source image
 * @param char *name: the image file name
 */
 static void test_reconstruction(pnm ims, char* name)
 {
  fprintf(stderr, "test_reconstruction: ");
  int cols = pnm_get_width(ims);
  int rows = pnm_get_height(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short *ps = pnm_get_image(ims);
  unsigned short *pd = pnm_get_image(imd);
  fftw_complex* fftw_for;
  //Transformation de Fourrier
  fftw_for = forward(rows, cols, ps);


  float* freq_as = malloc(rows*cols*3*sizeof(float));
  float* freq_ps = malloc(rows*cols*3*sizeof(float));

  //Génération des spectres
  freq2spectra(rows,cols,fftw_for,freq_as,freq_ps);

  fftw_complex* freq_repr = (fftw_complex*) fftw_malloc(rows*cols*sizeof(fftw_complex));
  
  //Génération de l'image à partir des spectres
  spectra2freq(rows,cols,freq_as,freq_ps,freq_repr);

  //transformation de Fourrier inverse
  unsigned short *out = backward(rows, cols, freq_repr);
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      for(int c=0; c<3; c++){
        *pd = *out;
        pd++;
        out++;
      }
    }
  }
    
  //Gestion des noms
  char tmp_name[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmp_name[i-find_last_slash(name)] = name[i];
  }

  char dest_name [30];
  strcpy(dest_name,"FB-ASPS-");
  strcat(dest_name,tmp_name);
  pnm_save(imd, PnmRawPpm, dest_name);
  fftw_free(freq_repr);
  fftw_free(fftw_for);
  free(freq_as);
  free(freq_ps);
  fprintf(stderr, "OK\n");
}

/**
 * @brief test construction of amplitude and phase images in ppm files
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
 static void test_display(pnm ims, char* name)
 {
  fprintf(stderr, "test_display: ");
  int cols = pnm_get_width(ims);
  int rows = pnm_get_height(ims);
  unsigned short *ps = pnm_get_image(ims);
  fftw_complex* fftw_for;

  //Transformation de Fourrier
  fftw_for = forward(rows, cols, ps);

  float* freq_as = malloc(rows*cols*3*sizeof(float));
  float* freq_ps = malloc(rows*cols*3*sizeof(float));

  //Génération des Spectres
  freq2spectra(rows,cols,fftw_for,freq_as,freq_ps);

  pnm im_as = pnm_new(cols, rows, PnmRawPpm);
  pnm im_ps = pnm_new(cols, rows, PnmRawPpm);

  float asmax = maxValue(freq_as, rows, cols);

  //Transformation non linéaire
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      for (int k = 0; k < 3; ++k)
      {
        pnm_set_component(im_as, i, j, k,(short) (255*pow(*freq_as/asmax, 0.15)));
        pnm_set_component(im_ps, i, j, k, (short) *freq_ps);
        freq_as++;
        freq_ps++;
      }
    }
  }

  //Gestion des noms
  char tmp_name[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmp_name[i-find_last_slash(name)] = name[i];
  }

  char dest_name [30];
  strcpy(dest_name,"AS-");
  strcat(dest_name,tmp_name);
  pnm_save(im_as, PnmRawPpm, dest_name);
  
  char tmp_name2[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmp_name2[i-find_last_slash(name)] = name[i];
  }
  char dest_name2 [30];
  strcpy(dest_name2,"PS-");
  strcat(dest_name2,tmp_name2);
  pnm_save(im_ps, PnmRawPpm, dest_name2);
  pnm_free(im_ps);
  pnm_free(im_as);
  fprintf(stderr, "OK\n");
}


/**
 * @brief test the modification of amplitude spectrum and 
 *        construct output images
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
 static void test_add_frequencies(pnm ims, char* name)
 {
  fprintf(stderr, "test_add_frequencies: ");
  int cols = pnm_get_width(ims);
  int rows = pnm_get_height(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  unsigned short *ps = pnm_get_image(ims);
  unsigned short *pd = pnm_get_image(imd);
  fftw_complex* fftw_for;
  
  //Transformation de Fourrier
  fftw_for = forward(rows, cols, ps);

  float* freq_as = malloc(rows*cols*3*sizeof(float));
  float* freq_ps = malloc(rows*cols*3*sizeof(float));

  //Génération des Spectres
  freq2spectra(rows,cols,fftw_for,freq_as,freq_ps);

  float asmax = maxValue(freq_as, rows, cols);

  pnm im_as = pnm_new(cols, rows, PnmRawPpm);

  int mi = rows/2;
  int mj = cols/2;

  //Modification du spectre
  float new_val = 0.25*asmax;
  for(int k = 0 ; k<3 ; k++){
    //fonction horizontale
    freq_as[cols*mi*3+mj*3 - 8*3 +k] = new_val;
    freq_as[cols*mi*3+mj*3 + 8*3 +k] = new_val;
    //fonction verticale
    freq_as[cols*(mi-8)*3 + mj*3 +k] = new_val;
    freq_as[cols*(mi+8)*3 + mj*3 +k] = new_val;

  }

  //Transformation non linéaire
  asmax = maxValue(freq_as, rows, cols);

  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      for (int k = 0; k < 3; ++k)
      {
        pnm_set_component(im_as, i, j, k,(short) (255*pow(*freq_as/asmax, 0.15)));
        freq_as++;
      }
    }
  }
  freq_as -= 3*rows*cols;
  //Génération de l'image à partir des spectres
  fftw_complex* freq_repr = (fftw_complex*) fftw_malloc(rows*cols*sizeof(fftw_complex));
  spectra2freq(rows,cols,freq_as,freq_ps,freq_repr);

  unsigned short *out = backward(rows, cols, freq_repr);
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      for(int c=0; c<3; c++){
        *pd = *out;
        pd++;
        out++;
      }
    }
  }

  //Gestion des noms
  char tmp_name[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmp_name[i-find_last_slash(name)] = name[i];
  }

  char dest_name [30];
  strcpy(dest_name,"FAS-");
  strcat(dest_name,tmp_name);
  pnm_save(im_as, PnmRawPpm, dest_name);
  
  char tmp_name2[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmp_name2[i-find_last_slash(name)] = name[i];
  }
  char dest_name2 [30];
  strcpy(dest_name2,"FREQ-");
  strcat(dest_name2,tmp_name2);
  pnm_save(imd, PnmRawPpm, dest_name2);
  pnm_free(im_as);
  pnm_free(imd);

  fprintf(stderr, "OK\n");
}

static void 
run(pnm ims, char* name)
{
  test_for_backward(ims, name);
  test_reconstruction(ims, name);
  test_display(ims, name);
  test_add_frequencies(ims, name);
}


void 
usage (char *s)
{
  fprintf(stderr, "Usage: %s <ims> \n", s);
  exit(EXIT_FAILURE);
}

int 
main(int argc, char *argv[])
{
  if (argc != 2)
    usage(argv[0]);
  pnm ims = pnm_load(argv[1]);
  run(ims, argv[1]);

  return EXIT_SUCCESS;
}
