#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

void usage(char* s){
  fprintf(stderr,"%s <dir> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

pnm flipH(pnm img, int rows, int cols){
	pnm tmpImage = pnm_new(cols, rows, PnmRawPpm);
	unsigned short* tmpStart = pnm_get_image(tmpImage);
	unsigned short* oldStart = pnm_get_image(img);

	unsigned short *tmpCurrent = pnm_get_image(tmpImage);
	unsigned short *oldCurrent = pnm_get_image(img);
	

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			tmpCurrent = tmpStart + pnm_offset(tmpImage,i,j);
			oldCurrent = oldStart + pnm_offset(img,i,cols-1-j);
			for(int k=0;k<3;k++){
				*tmpCurrent = *oldCurrent;
				tmpCurrent++;
				oldCurrent++;
			}
		}
	}

	return tmpImage;
}

pnm flipV(pnm img, int rows, int cols){
	pnm tmpImage = pnm_new(cols, rows, PnmRawPpm);
	unsigned short* tmpStart = pnm_get_image(tmpImage);
	unsigned short* oldStart = pnm_get_image(img);

	unsigned short *tmpCurrent = pnm_get_image(tmpImage);
	unsigned short *oldCurrent = pnm_get_image(img);
	

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			tmpCurrent = tmpStart + pnm_offset(tmpImage,i,j);
			oldCurrent = oldStart + pnm_offset(img,rows-1-i,j);
			for(int k=0;k<3;k++){
				*tmpCurrent = *oldCurrent;
				tmpCurrent++;
				oldCurrent++;
			}
		}
	}

	return tmpImage;
}

pnm flipT(pnm img, int rows, int cols){
	pnm tmpImage = pnm_new(rows, cols, PnmRawPpm);
	unsigned short* tmpStart = pnm_get_image(tmpImage);
	unsigned short* oldStart = pnm_get_image(img);

	unsigned short *tmpCurrent = pnm_get_image(tmpImage);
	unsigned short *oldCurrent = pnm_get_image(img);
	

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			tmpCurrent = tmpStart + pnm_offset(tmpImage,j,i);
			oldCurrent = oldStart + pnm_offset(img,i,j);
			for(int k=0;k<3;k++){
				*tmpCurrent = *oldCurrent;
				tmpCurrent++;
				oldCurrent++;
			}
		}
	}

	return tmpImage;
}

void process(char* dir,char* oldName,char* newName){
	pnm newImage = pnm_load(oldName);
	int cols = pnm_get_width(newImage);
	int rows = pnm_get_height(newImage);

	int i =0;
	while(dir[i] != '\0'){
		if(dir[i] == 'h'){
			newImage = flipH(newImage, rows, cols);
		} else if(dir[i] == 'v'){
			newImage = flipV(newImage, rows, cols);
		} else if(dir[i] == 't'){
			newImage = flipT(newImage, rows, cols);
		} 

		i++;
	}

	pnm_save(newImage, PnmRawPpm, newName);

	pnm_free(newImage);
}

#define PARAM 3
int main(int argc, char* argv[]){
	if(argc != PARAM+1)
		usage(argv[0]);

	char* dir = argv[1];
	char* oldImage = argv[2];
	char* newImage = argv[3];

	process(dir,oldImage, newImage);

	return EXIT_SUCCESS;
}
