/**
 * @file move.c
 * @author HENRY Pascalie & GRUCHET Sébastien
 */

#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>


void  
process(int dx, int dy, char* ims_name,char* imd_name){
  
  pnm ims = pnm_load(ims_name);

  int cols = pnm_get_width(ims);
  int rows = pnm_get_height(ims);
  
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short *p_ims = pnm_get_image(ims);
  unsigned short *p_imd = pnm_get_image(imd);
  
  unsigned short *images = pnm_get_image(ims);
  unsigned short *imaged = pnm_get_image(imd);
  
  int i=0,j=0,k=0;
  
  int idebut, ifin, jdebut, jfin;
  
  if(dx>=0){
	  jdebut=dx;
	  jfin=0;
  }
  else{
	  jdebut=0;
	  jfin=-dx;	  
  }
  
  if(dy>=0){
	  idebut=dy;
	  ifin=0;  
  }
  else{
	  idebut=0;
	  ifin=-dy;	  
  }
			  
			  
  for(i=idebut;i+ifin<rows;i++){
	  for(j=jdebut;j+jfin<cols;j++){
		  p_ims = images + pnm_offset(ims,i-dy,j-dx);
		  p_imd = imaged + pnm_offset(imd,i,j);
		  for(k=0; k<3; k++){
			*p_imd = *p_ims;
			p_imd++;
			p_ims++;
		  }
	  }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);

}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <dx> <dy> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int 
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);  
  unsigned int dx = atoi(argv[1]);
  unsigned int dy = atoi(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  process(dx,dy,ims_name,imd_name);
  return EXIT_SUCCESS;
}

