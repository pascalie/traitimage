/**
 * @file rotate.c
 * @author HENRY Pascalie & GRUCHET Sébastien
 */

#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include "domain.h"
#include <math.h>

#define PI acos(-1.0)


void rotate(float res[], int x, int y, float angle, int x0, int y0) {
    res[0] = cos(angle)*(x-x0) - sin(angle)*(y-y0)+x0;
    res[1] = sin(angle)*(x-x0) + cos(angle)*(y-y0)+y0;
}

void
process(unsigned int x0, unsigned int y0, float angle, char* ims_name, char* imd_name) {

    pnm ims = pnm_load(ims_name);

    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);

    pnm imd = pnm_new(width, height, PnmRawPpm);

    float coord[2];
    float radAngle = angle * PI / 180;
    unsigned short tmp;

    for (int x=0; x<width-1; x++) {

        for (int y=0; y<height-1; y++) {

            rotate(coord, x, y, radAngle, x0, y0);

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

void
usage (char *s) {
    fprintf(stderr, "Usage: %s <x> <y> <angle> <ims> <imd>\n", s);
    exit(EXIT_FAILURE);
}

#define PARAM 5
int
main(int argc, char *argv[]) {
    if (argc != PARAM+1) usage(argv[0]);
    unsigned int x0 = atoi(argv[1]);
    unsigned int y0 = atoi(argv[2]);
    int angle = atof(argv[3]);
    char *ims_name = argv[4];
    char *imd_name = argv[5];
    process(x0,y0,angle,ims_name,imd_name);
    return EXIT_SUCCESS;
}

