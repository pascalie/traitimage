#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>


void process(int dx, int dy, char* oldName, char* newName){
	pnm oldImage = pnm_load(oldName);
	int cols = pnm_get_width(oldImage);
	int rows = pnm_get_height(oldImage);

	pnm newImage = pnm_new(cols, rows, PnmRawPpm);

	//start of images
	unsigned short *oldStart = pnm_get_image(oldImage);
	unsigned short *newStart = pnm_get_image(newImage);

	//current position in the images
	unsigned short *oldCurrent = pnm_get_image(oldImage);
	unsigned short *newCurrent = pnm_get_image(newImage);

	//end of images
	//unsigned short *oldEnd = pnm_get_image(oldImage);
	//unsigned short *newEnd = pnm_get_image(newImage);
	//oldEnd = oldStart + pnm_offset(oldImage, rows-1, cols-1);
	//newEnd = newStart + pnm_offset(newImage, rows-1, cols-1);

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			newCurrent = newStart + pnm_offset(newImage,i,j);
			oldCurrent = oldStart + pnm_offset(oldImage,i,j);

			int r, g, b;
		
			int vertOffset, horOffset;
			if(i-dy < 0){
				vertOffset = rows+i-dy;
			} else if(i-dy >= rows){
				vertOffset = i-dy-rows;
			} else {
				vertOffset = i-dy;
			}

			if(j-dx < 0){
				horOffset = cols+j-dx;
			} else if(j-dx >= cols){
				horOffset = j-dx-cols;
			} else {
				horOffset = j-dx;
			}

			oldCurrent = oldStart + pnm_offset(oldImage, vertOffset, horOffset);
			r = *oldCurrent;
			oldCurrent++;
			g = *oldCurrent;
			oldCurrent++;
			b = *oldCurrent;

			*newCurrent = r;
			newCurrent++;
			*newCurrent = g;
			newCurrent++;
			*newCurrent = b;
		}
	}


	pnm_save(newImage, PnmRawPpm, newName);

	pnm_free(oldImage);
	pnm_free(newImage);
}

void usage(char* s){
  fprintf(stderr,"%s <dx> <dy> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char* argv[]){
	if(argc != PARAM+1)
		usage(argv[0]);

	int dx = atoi(argv[1]);
	int dy = atoi(argv[2]);
	char* oldImage = argv[3];
	char* newImage = argv[4];

	process(dx, dy, oldImage, newImage);

	return EXIT_SUCCESS;
}
