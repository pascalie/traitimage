/**
 * @file copy.c
 * @brief create a zoom of factor on 
 * the image taken in paramater by 
 * copy of pixels
 *
 */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bcl.h>

void run(int factor,pnm ims,char * name){
	int rows = pnm_get_height(ims);
	int cols = pnm_get_width(ims); 
	int new_rows = rows*factor;
	int new_cols = cols*factor;
	pnm imd = pnm_new(new_cols,new_rows,PnmRawPpm);
	
	//Remplissage de la nouvelle image
	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				unsigned short value = pnm_get_component(ims,i,j,k);

				for (int l = 0; l < factor; ++l)
				{
					for (int m = 0; m < factor; ++m)
					{
						pnm_set_component(imd,(i*factor)+l,(j*factor)+m,k,value);
					}
					
				}
			}
		}
	}
	pnm_save(imd,PnmRawPpm,name);
}

void usage (char *s)
{
  fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  if (argc != 4)
    usage(argv[0]);
  int factor = atoi(argv[1]);
  pnm ims = pnm_load(argv[2]);
  char * name = argv[3];
  run(factor,ims, name);

  return EXIT_SUCCESS;
}