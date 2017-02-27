/**
 * @file wave.c
 * @author HENRY Pascalie & GRUCHET Sébastien
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#define PI acos(-1.0)

void wave(float coord[], int x0, int y0, float tx,float ax,float ty,float ay) {
  coord[0] = x0 + ax*sin(2*PI*y0/tx);
  coord[1] = y0 + ay*sin(2*PI*x0/ty);
}

void  
process(float tx,float ax,float ty,float ay, char *ims_name, char *imd_name) {

  pnm ims = pnm_load(ims_name);
  
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  
  pnm imd = pnm_new(width, height, PnmRawPpm);

  float coord[2];
  unsigned short tmp;

  for (int x=0; x<width; x++) {
    for (int y=0; y<height; y++) {
      wave(coord, x, y, tx, ax, ty, ay);
      for (int c=0; c<3; c++) {
        if (coord[0] >= 0 && coord[1] >=0 && coord[0] < width-1 && coord[1] < height-1) {
          tmp = bilinear_interpolation(coord[0], coord[1], ims, c);
          pnm_set_component(imd, y, x, c, tmp);
        }
      }
    }
  }



  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);

}

static void usage(char* s){
  fprintf(stderr,"Usage: %s <tx> <ax> <ty> <ay> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 6
int main(int argc, char* argv[]){
  if(argc != PARAM+1) usage(argv[0]);
  
  float tx = atof(argv[1]);
  float ax = atof(argv[2]);
  float ty = atof(argv[3]);
  float ay = atof(argv[4]);
  char *ims_name = argv[5];
  char *imd_name = argv[6];
  process(tx,ax,ty,ay,ims_name,imd_name);

  return EXIT_SUCCESS;
}
