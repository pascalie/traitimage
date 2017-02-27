/**
 * @file flip.c
 * @author HENRY Pascalie & GRUCHET Sébastien
 */

#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include <string.h>

 void  
 process(char* directions ,char* ims_name,char* imd_name){

  pnm ims = pnm_load(ims_name);

  int cols = pnm_get_width(ims);
  int rows = pnm_get_height(ims);
  
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  int dir = strlen(directions);
  for (int d=0; d<dir; d++){
    cols = pnm_get_width(ims);
    rows = pnm_get_height(ims);

    unsigned short *ps = pnm_get_image(ims);
    unsigned short *pd = pnm_get_image(imd);
    int i,j;
    unsigned short *image = pnm_get_image(ims);

    switch(directions[d]) {
      case 'h':
      for(i=0;i<rows;i++){
        for(j=0;j<cols;j++){
          ps = image + pnm_offset(ims,i,cols - j-1);
          for(int c=0; c<3; c++){
            *pd = *ps;
            pd++;
            ps++;
          }
        }
      }
      break;

      case 'v':
      for(i=0;i<rows;i++){
        for(j=0;j<cols;j++){
          ps = image + pnm_offset(ims,rows - i-1, j);
          for(int c=0; c<3; c++){
            *pd = *ps;
            pd++;
            ps++;
          }
        }
      }
      break;

      case 't':
      pnm_free(imd);
      imd = pnm_new(rows, cols, PnmRawPpm);
      unsigned short *pd = pnm_get_image(imd);
      for(i=0;i<cols;i++){
        for(j=0;j<rows;j++){
          ps = image + pnm_offset(ims,j,i);
          for(int c=0; c<3; c++){
            *pd = *ps;
            pd++;
            ps++;
          }
        }
      }
      break;

    }
    ims = pnm_dup(imd);
  }
  
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <dir> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int 
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  char *dir = argv[1]; 
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  process(dir,ims_name,imd_name);
  return EXIT_SUCCESS;
}

