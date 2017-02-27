#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

int floatToInt(float b){
	int tmp = (int)b;
	float diff = b-tmp;

	if(diff > 0.5){
		return tmp+1;
	} else {
		return tmp;
	}
}

int normChannel(float r, float g, float b){
	float norm = (r + g + b)/(3.0);

	return floatToInt(norm);
}

void process(char* ims, char* newName){
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

	//
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			//get newimage pixel
			newCurrent = newStart + pnm_offset(newImage, i,j);
			
			//get oldimage pixel
			oldCurrent = oldStart + pnm_offset(oldImage, i,j);

			int r = *oldCurrent;
			oldCurrent++;
			int g = *oldCurrent;
			oldCurrent++;
			int b = *oldCurrent;
			oldCurrent++;

			int norm = normChannel(r,g,b);

			for(int k=0;k<3;k++){
				//set newimage channels
				*newCurrent = norm;
				newCurrent++;
			}
		}
	}

	pnm_save(newImage, PnmRawPpm, newName);

	pnm_free(oldImage);
	pnm_free(newImage);
}


int main(int argc, char* argv[]){
	if(argc != 3){
		exit(EXIT_FAILURE);
	}

	char* oldImage = argv[1];
	char* newImage = argv[2];
	process(oldImage, newImage);

	return(EXIT_SUCCESS);
}