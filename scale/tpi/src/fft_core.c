// Utility functions regarding the FFT

/* This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Pulic License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2014-2019, Thibaud Briand <thibaud.briand@enpc.fr>
 *
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <complex.h>
#include <fftw3.h>

#define FFTW_NTHREADS // comment to disable multithreaded FFT

// start threaded FFTW if FFTW_NTHREADS is defined
void init_fftw(void) {
    #ifdef FFTW_NTHREADS
    fftw_init_threads();
    #ifdef _OPENMP
    fftw_plan_with_nthreads(omp_get_max_threads());
    #endif
    #endif
}

// clean FFTW
void clean_fftw(void) {
    fftw_cleanup();
    #ifdef FFTW_NTHREADS
    fftw_cleanup_threads(); 
    #endif
}

// Compute the DFT of a real-valued image.
void do_fft_real(fftw_complex *out, const double *in, int nx, int ny, int nz)
{
    // memory allocation
    fftw_complex *in_plan = (fftw_complex *) malloc(nx*ny*sizeof(fftw_complex));
    fftw_complex *out_plan = (fftw_complex *) malloc(nx*ny*sizeof(fftw_complex));
    fftw_plan plan = fftw_plan_dft_2d(ny, nx, in_plan, out_plan, FFTW_FORWARD, FFTW_ESTIMATE);

    // loop over the channels
    for (int l = 0; l < nz; l++) {
        // Real --> complex
        for(int i = 0; i < nx*ny; i++)
            in_plan[i] = (double complex) in[i + l*nx*ny];

        // compute fft
        fftw_execute(plan);

        // copy to output
        memcpy(out + l*nx*ny, out_plan, nx*ny*sizeof(fftw_complex));
    }

    // free
    fftw_destroy_plan(plan);
    fftw_free(in_plan);
    fftw_free(out_plan);
}

// Compute the real part of the iDFT of a complex-valued image.
void do_ifft_real(double *out, const fftw_complex *in, int nx, int ny, int nz)
{
    // memory allocation
    fftw_complex *in_plan = (fftw_complex *) malloc(nx*ny*sizeof(fftw_complex));
    fftw_complex *out_plan = (fftw_complex *) malloc(nx*ny*sizeof(fftw_complex));
    fftw_plan plan = fftw_plan_dft_2d (ny, nx, in_plan, out_plan, FFTW_BACKWARD, FFTW_ESTIMATE);

    // normalization constant
    double norm = 1.0/(nx*ny);

    // loop over the channels
    for (int l = 0; l < nz; l++) {
        // copy to input
        memcpy(in_plan, in + l*nx*ny, nx*ny*sizeof(fftw_complex));

        // compute ifft
        fftw_execute(plan);

        // complex to real + normalization
        for(int i = 0; i < nx*ny; i++)
            out[i + l*nx*ny] = creal(out_plan[i])*norm;
    }

    // free
    fftw_destroy_plan(plan);
    fftw_free(in_plan);
    fftw_free(out_plan);
}

// Compute the fftshift of a complex-valued image.
void fftshift(fftw_complex *fshift, fftw_complex *fhat, int nx, int ny, int nz) {
    int nx2 = nx/2;
    int ny2 = ny/2;
    int cx = (nx+1)/2;
    int cy = (ny+1)/2;
    int i, j, l, i2, j2;

    for(j = 0; j < ny; j++) {
        j2 = (j < cy) ? j + ny2 : j - cy;
        for(i = 0; i < nx; i++) {
            i2 = (i < cx) ? i + nx2 : i - cx;
            for(l = 0; l < nz; l++)
                fshift[i2 + j2*nx + l*nx*ny] = fhat[i + j*nx + l*nx*ny];
        }
    }
}

