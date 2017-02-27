#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>


void process(int absStart, int ordStart, int rows, int cols, char* ims, char* imd){
	
	pnm subImage = pnm_new(cols, rows, PnmRawPpm);
	pnm bigImage = pnm_load(ims);

	//get start of image
	unsigned short *bigStart = pnm_get_image(bigImage);
	unsigned short *subStart = pnm_get_image(subImage);

	unsigned short *bigCurrent = pnm_get_image(bigImage);
	unsigned short *subCurrent = pnm_get_image(subImage);

	//
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			subCurrent = subStart + pnm_offset(subImage, i,j);
			bigCurrent = bigStart + pnm_offset(bigImage, absStart, ordStart);
			bigCurrent = bigCurrent + pnm_offset(bigImage,i,j);

			for(int k=0;k<3;k++){
				*subCurrent = *bigCurrent;
				subCurrent++;
				bigCurrent++;
			}
		}
	}

	pnm_save(subImage, PnmRawPpm, imd);
	pnm_free(subImage);
	pnm_free(bigImage);
}

int main(int argc, char* argv[]){
	if(argc != 7){
		exit(EXIT_FAILURE);
	}

	int absStart = atoi(argv[1]);
	int ordStart = atoi(argv[2]);
	int rows = atoi(argv[3]);
	int cols = atoi(argv[4]);
	char* ims = argv[5];
	char* imd = argv[6];

	process(absStart, ordStart, rows, cols, ims, imd);
	return EXIT_SUCCESS;
}