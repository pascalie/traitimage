#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>


void process(int num, char* ims, char* newName){
	pnm oldImage = pnm_load(ims);
	int cols = pnm_get_width(oldImage);
	int rows = pnm_get_height(oldImage);

	pnm newImage = pnm_new(cols, rows, PnmRawPpm);

	//start of images
	unsigned short *oldStart = pnm_get_image(oldImage);
	unsigned short *newStart = pnm_get_image(newImage);

	//current position in the images
	unsigned short *oldCurrent = pnm_get_image(oldImage);
	unsigned short *newCurrent = pnm_get_image(newImage);

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			//get newimage pixel
			newCurrent = newStart + pnm_offset(newImage, i,j);
			
			//get oldimage pixel
			oldCurrent = oldStart + pnm_offset(oldImage, i,j);
			//get channel of said pixel
			oldCurrent = oldCurrent + num;

			for(int k=0;k<3;k++){
				//set newimage channels
				*newCurrent = *oldCurrent;
				newCurrent++;
			}
		}
	}

	pnm_save(newImage, PnmRawPpm, newName);

	pnm_free(oldImage);
	pnm_free(newImage);

	/*
	(void)num;
	(void)newName;

	printf("test2");
	*/
}

int main(int argc, char* argv[]){

	printf("test");
	if(argc != 4){
		exit(EXIT_FAILURE);
	}

	int num = atoi(argv[1]);
	char* oldImage = argv[2];
	char* newImage = argv[3];
	process(num, oldImage, newImage);

	return(EXIT_SUCCESS);
}