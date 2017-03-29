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

/**
 * @brief test the forward and backward functions
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */



int mystrlen(char* a){
  int ret = 0;

  for(int i=0; a[i] != '\0'; i++){
    ret++;
  }
  ret++;

  return ret;

}

char * mystrcat(char* first, char* second){
  int sFirst = strlen(first);
  int sSecond = strlen(second);

  char* ret = malloc(sFirst+sSecond-1);

  for(int i=0; i< sFirst; i++){
    ret[i] = first[i];
  }

  for(int i=0; i<sSecond;i++){
    ret[i+sFirst] = second[i];
  }

  return ret;
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

static void
test_for_backward(pnm ims, char* name)
{
  fprintf(stderr, "test_for_backward: ");

  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  unsigned short *start = pnm_get_image(ims);
  unsigned short *current = pnm_get_image(ims);
  unsigned short* canal = malloc(sizeof(unsigned short)*cols*rows); // TO FREE
  for(int i=0;i<rows; i++){
    for(int j=0;j<cols;j++){
      current = start + pnm_offset(ims, i,j);
      canal[(i*cols)+j] = *current;
    }
  }

  fftw_complex *complex_img = forward(rows,cols,canal);
  unsigned short *values = backward(rows,cols,complex_img);


  pnm ret = pnm_new(cols, rows, PnmRawPpm);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      for (int k = 0; k < 3; ++k)
        pnm_set_component(ret,i,j,k,values[i*cols+j]);  


  char tmpName[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmpName[i-find_last_slash(name)] = name[i];
  }

  char prefix[] = "FB-";

  printf("tmpName : %s\n",tmpName);

  printf("prefix : %s\n", prefix);

  char* newName = mystrcat(prefix,tmpName);

  

  printf("newName : %s\n", newName);

  pnm_save(ret,PnmRawPpm,newName);
  
  free(newName);
  free(canal);
  free(complex_img);
  free(values);
  fprintf(stderr, "OK\n");
}




/**
 * @brief test image reconstruction from of amplitude and phase spectrum
 * @param pnm ims: the source image
 * @param char *name: the image file name
 */
static void
test_reconstruction(pnm ims, char* name)
{ 
  (void)ims;
  (void)name;
  /*
  fprintf(stderr, "test_reconstruction: ");
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  unsigned short *start = pnm_get_image(ims);
  unsigned short *current = pnm_get_image(ims);
  unsigned short* canal = malloc(sizeof(unsigned short)*cols*rows); // TO FREE
  for(int i=0;i<rows; i++){
    for(int j=0;j<cols;j++){
      current = start + pnm_offset(ims, i,j);
      canal[(i*cols)+j] = *current;
    }
  }

  //toComplex
  fftw_complex *complex_img = forward(rows,cols,canal);

      //toSpectre
  float * as = malloc(rows*cols*sizeof(float));
  float * ps = malloc(rows*cols*sizeof(float));
  freq2spectra(rows,cols,complex_img,as,ps);


  fftw_complex * freq_rep = malloc(rows*cols*sizeof(fftw_complex)); 
  
      //end toSpectre
  spectra2freq(rows,cols,as,ps,freq_rep);
  
  //end toComplex
  unsigned short *values = backward(rows,cols,freq_rep);


  pnm ret = pnm_new(cols, rows, PnmRawPpm);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      for (int k = 0; k < 3; ++k){
        pnm_set_component(ret,i,j,k,values[i*cols+j]);
        //printf("ret : %u\n",pnm_get_component(ret,i,j,k));  
      }


  char tmpName[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmpName[i-find_last_slash(name)] = name[i];
  }

  char prefix[] = "FB-ASPS-";

  char* newName = mystrcat(prefix,tmpName);


  pnm_save(ret,PnmRawPpm,newName);
  
  free(newName);
  free(canal);
  free(complex_img);
  free(values);
  free(as);
  free(ps);
  free(freq_rep);
  fprintf(stderr, "OK\n");
  */
}


/**
 * @brief test construction of amplitude and phase images in ppm files
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_display(pnm ims, char* name)
{
  fprintf(stderr, "test_display: ");

  (void)ims;
  (void)name;
 /*
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  unsigned short *start = pnm_get_image(ims);
  unsigned short *current = pnm_get_image(ims);
  unsigned short* canal = malloc(sizeof(unsigned short)*cols*rows); // TO FREE
  for(int i=0;i<rows; i++){
    for(int j=0;j<cols;j++){
      current = start + pnm_offset(ims, i,j);
      canal[(i*cols)+j] = *current;
    }
  }

  //toComplex
  fftw_complex *complex_img = forward(rows,cols,canal);

      //toSpectre
  float * as = malloc(rows*cols*sizeof(float));
  float * ps = malloc(rows*cols*sizeof(float));
  freq2spectra(rows,cols,complex_img,as,ps);

  fftw_complex * freq_rep = malloc(rows*cols*sizeof(fftw_complex)); 
  unsigned short * asu = malloc(rows*cols*sizeof(unsigned short));
  unsigned short * psu = malloc(rows*cols*sizeof(unsigned short));
  for (int i = 0; i < rows*cols; ++i)   
  {
    as[i] = logf(1 + as[i]);
    asu[i] = convert_to_unsigned_short(as[i]);
    psu[i] = convert_to_unsigned_short(ps[i]);
  }

  

  pnm ret1 = pnm_new(cols, rows, PnmRawPpm);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      for (int k = 0; k < 3; ++k)
        pnm_set_component(ret1,i,j,k,as[i*cols+j]);  

  pnm ret2 = pnm_new(cols, rows, PnmRawPpm);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      for (int k = 0; k < 3; ++k)
        pnm_set_component(ret2,i,j,k,ps[i*cols+j]); 



  char tmpName[strlen(name) - find_last_slash(name)];
  for(int i = find_last_slash(name); name[i] != '\0';i++){
    tmpName[i-find_last_slash(name)] = name[i];
  }

  char prefix1[] = "AS-";
  char prefix2[] = "PS-";

  char* newName1 = mystrcat(prefix1,tmpName);
  char* newName2 = mystrcat(prefix2,tmpName);

  pnm_save(ret1,PnmRawPpm,newName1);
  pnm_save(ret2,PnmRawPpm,newName2);
  
  free(newName1);
  free(newName2);
  free(canal);
  free(complex_img);
  free(as);
  free(ps);
  free(freq_rep);

  */
  fprintf(stderr, "OK\n");
}

/**
 * @brief test the modification of amplitude spectrum and 
 *        construct output images
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_add_frequencies(pnm ims, char* name)
{
  fprintf(stderr, "test_add_frequencies: ");
  (void)ims;
  (void)name;
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