/* SPDX-License-Identifier: GPL-2.0+
 * 
 * Thibaud Briand <briand.thibaud@gmail.com>
 * 
 * Copyright (c) 2018-2019, Thibaud Briand
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Pulic License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>

#include "fft_core.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

// Perform the phase shift (Line 3 of Algorithm 3 using Proposition 6)
static void phase_shift(fftw_complex *outhat, fftw_complex *inhat,
                        int nx, int ny, int nz,
                        double dx, double dy, int interp)
{
    // central indices
    int cx = (nx+1)/2;
    int cy = (ny+1)/2;
    
    double complex factorx, factory;
    int i, j, l, m, n;
    double thetax = -2*M_PI*dx/nx;
    double thetay = -2*M_PI*dy/ny;
    
    // complex convention
    for(j = 0; j < ny; j++) {
        n = (j < cy) ? j : j - ny;
        factory = cexp(I*n*thetay);
        for(i = 0; i < nx; i++) {
            m = (i < cx) ? i : i - nx;
            factorx = cexp(I*m*thetax);
            for(l = 0; l < nz; l++)
                outhat[i + j*nx + l*nx*ny] = inhat[i + j*nx + l*nx*ny]*factorx*factory;
        }
    }
    
    // real part
    // useless in practice if the real part is taken afterwards
    if( !(nx%2) ) { // case nx even
        i = nx/2;
        factorx = cos(M_PI*dx);
        for(j = 0; j < ny; j++) {
            n = (j < cy) ? j : j - ny;
            factory = cexp(I*n*thetay);
            for(l = 0; l < nz; l++)
                outhat[i + j*nx + l*nx*ny] = inhat[i + j*nx + l*nx*ny]*factorx*factory;
        }
    }
    
    if( !(ny%2) ) { // case ny even
        j = ny/2;
        factory = cos(M_PI*dy);
        for(i = 0; i < nx; i++) {
            m = (i < cx) ? i : i - nx;
            factorx = cexp(I*m*thetax);
            for(l = 0; l < nz; l++)
                outhat[i + j*nx + l*nx*ny] = inhat[i + j*nx + l*nx*ny]*factorx*factory;
        }
    }

    if( !(nx%2) && !(ny%2) ) { // case nx and ny even
        int ind = nx/2 + ny/2*nx;
        factorx = interp ? 0.5*(cos(M_PI*(dx + dy)) + cos(M_PI*(dx - dy))) : cos(M_PI*(dx + dy));
        for(l = 0; l < nz; l++) 
            outhat[ind + l*nx*ny] = inhat[ind + l*nx*ny]*factorx;
    }
}

// Translation of an image using trigonometric polynomial interpolation (Algorithm 1).
void interpolate_image_shift(double *out, double *in, int nx, int ny, int nz, 
                             double dx, double dy, int interp)
{
    // allocate memory for fourier transform
    fftw_complex *inhat = fftw_malloc(nx*ny*nz*sizeof*inhat);
    fftw_complex *outhat = fftw_malloc(nx*ny*nz*sizeof*outhat);

    // compute DFT of the input
    do_fft_real(inhat, in, nx, ny, nz);

    // phase shift
    phase_shift(outhat, inhat, nx, ny, nz, dx, dy, interp);
    
    // compute iDFT of the input
    do_ifft_real(out, outhat, nx, ny, nz);
    
    // free memory and cleanup
    fftw_free(inhat);
    fftw_free(outhat);
}
