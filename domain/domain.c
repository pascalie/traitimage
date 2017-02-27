/**
 * @file domain.c
 * @author HENRY Pascalie & GRUCHET Sébastien
 */
#include <math.h>
#include "domain.h"

unsigned short
bilinear_interpolation(float x, float y, pnm ims, int c) {

    unsigned short res;
    int i = (int)y;
    int j = (int)x;
    float dx = x - j;
    float dy = y - i;

    unsigned short Iij = pnm_get_component(ims, i, j, c);
    unsigned short Iij1 = pnm_get_component(ims, i, j+1, c);
    unsigned short Ii1j = pnm_get_component(ims, i+1, j, c);
    unsigned short Ii1j1 = pnm_get_component(ims, i+1, j+1, c);

    res = (1 - dx)*(1 - dy)*Iij + dx*(1 - dy)*Iij1 + (1 - dx)*dy*Ii1j + dx*dy*Ii1j1;

    return res;
}

