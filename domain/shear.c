/**
 * @file shear.c
 * @author HENRY Pascalie & GRUCHET Sébastien
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#define M_PI acos(-1.0)

void  
process(char dir,float angle, char* ims_name,char* imd_name){
	
  pnm ims = pnm_load(ims_name);
  pnm imd = NULL;
  
  float rad = angle*M_PI/180;
  int colss = pnm_get_width(ims);
  int rowss = pnm_get_height(ims);
  
  int colsd;
  int rowsd;
  
  if(dir=='h'){
	  	  
	  int i,j,k;
	  float i_ims,j_ims;
	  
	  int f1, f2;
	  if(angle>=0){
		  f1 = 0;
		  f2 = 1;
	  }
	  else{
		  f1= 1;
		  f2= -1;
	  }

	  colsd = colss + rowss*tan(f2*rad);
	  rowsd = rowss;
	  imd = pnm_new(colsd, rowsd, PnmRawPpm);
	  
	  unsigned short *p_imd = pnm_get_image(imd);
	  unsigned short *imaged = pnm_get_image(imd);
	  
	  for(i=0;i<rowsd-1;i++){
		  for(j=0;j<colsd;j++){
					  
			  p_imd = imaged + pnm_offset(imd,i,j);	
			  
			  i_ims = i;
			  j_ims = j - (f1*rowss+f2*i)*tan(f2*rad);
			  
			  if(j_ims>=0 && j_ims<colss-1){
				  
				  for(k=0; k<3; k++){
					  *p_imd=bilinear_interpolation(j_ims,i_ims,ims,k);
					  p_imd++;
				  }
			  }
		  }
	  }	  
  }
  else if(dir=='v'){
	  
	  colsd = colss ;
	  rowsd = rowss + abs(colss*tan(rad));
	  
	  int i,j,k;
	  float i_ims,j_ims;
	  
	  int f1, f2;
	  if(angle>=0){
		  f1 = 0;
		  f2 = 1;
	  }
	  else{
		  f1= 1;
		  f2= -1;
	  }

	  colsd = colss;
	  rowsd = rowss + colss*tan(f2*rad);
	  imd = pnm_new(colsd, rowsd, PnmRawPpm);
	  
	  unsigned short *p_imd = pnm_get_image(imd);
	  unsigned short *imaged = pnm_get_image(imd);
	  
	  for(i=0;i<rowsd;i++){
		  for(j=0;j<colsd-1;j++){
					  
			  p_imd = imaged + pnm_offset(imd,i,j);	
			  
			  j_ims = j;
			  i_ims = i - (f1*colss+f2*j)*tan(f2*rad);
			  
			  if(i_ims>=0 && i_ims<rowss-1){
				  
				  for(k=0; k<3; k++){
					  *p_imd=bilinear_interpolation(j_ims,i_ims,ims,k);
					  p_imd++;
				  }
			  }
		  }
	  }
	  	  
  }
  else{
	printf("Erreur\n");
  }

 
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);

}

static void usage(char* s){
  fprintf(stderr,"Usage: %s <dir>{h ,v} <angle> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char* argv[]){
  if(argc != PARAM+1) usage(argv[0]);
  
  char dir = (argv[1])[0]; 
  float angle = atof(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  process(dir,angle,ims_name,imd_name);

  return EXIT_SUCCESS;
}
