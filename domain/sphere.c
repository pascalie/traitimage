/**
 * @file sphere.c
 * @author HENRY Pascalie & GRUCHET Sébastien
 */

#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include "domain.h"
#include <math.h>

#define PI acos(-1.0)

void process(char* ims_name, char* imd_name) {

    pnm ims = pnm_load(ims_name);

    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);

    pnm imd = pnm_new(width, height, PnmRawPpm);

    //int k = 0;
    unsigned short tmp;
    for (int x = 0; x < width; ++x) {
        float nx = 2.0*x/width-1.0;
        float nx2 = nx*nx;

        for (int y=0; y<height; y++) {
            float ny = 2.0*y/height-1.0;
            float ny2 = ny*ny;

            float r = sqrt(nx2+ny2);

            if (r >= 0.0 && r <= 1.0) {
                float nr = (r + (1.0-sqrt(1.0-r*r))) / 2.0;

                if (nr <= 1.0) {
                    float theta = atan2(ny, nx);
                    float nnx = nr*cos(theta);
                    float nny = nr*sin(theta);

                    float x1 = (nnx+1.0)*width/2.0;
                    float y1 = (nny+1.0)*height/2.0;

                    for (int c=0;c<3;c++) {
                        tmp = bilinear_interpolation(x1,y1,ims,c);
                        pnm_set_component(imd, y, x, c, tmp);
                    }
                }
            }
        }
    }


    pnm_save(imd, PnmRawPpm, imd_name);
    pnm_free(imd);
    pnm_free(ims);

}

void usage (char *s) {
    fprintf(stderr, "Usage: %s <ims> <imd>\n", s);
    exit(EXIT_FAILURE);
}

#define PARAM 2
int main(int argc, char *argv[]) {
    if (argc != PARAM+1) usage(argv[0]);
    char *ims_name = argv[1];
    char *imd_name = argv[2];
    process(ims_name,imd_name);
    return EXIT_SUCCESS;
}

