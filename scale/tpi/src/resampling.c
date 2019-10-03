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

#include <complex.h>
#include <fftw3.h>

#include "fft_core.h"

// Compute the DFT coefficients of the up-sampled image (Line 3 of Algorithm 3 using Proposition 11)
static void upsampling_fourier(fftw_complex *out, fftw_complex *in,
                               int nxin, int nyin, int nxout, int nyout, int nz, int interp)
{
    int i, j, l, i2, j2;
    
    // normalization constant
    double norm = nxout*nyout*1.0/(nxin*nyin);
    
    // central indices
    int cx = (nxin+1)/2; 
    int cy = (nyin+1)/2;

    // fill the output dft with zeros
    for (i = 0; i < nxout*nyout*nz; i++)
        out[i] = 0.0;

    // fill the corners with the values
    for(j = 0; j < nyin; j++) {
        j2 = (j < cy) ? j : j + nyout-nyin;
        for(i = 0; i < nxin; i++) {
            i2 = (i < cx) ? i : i + nxout-nxin;
            for (l = 0; l < nz; l++)
                out[i2 + j2*nxout + l*nxout*nyout] = norm*in[i + j*nxin + l*nxin*nyin];
        }
    }

    // real part
    // useless in practice if the real part of the image is taken afterwards
    if ( !(nxin%2) && nxout>nxin) {
        i = cx; // positive in output and negative in input
        i2 = cx + nxout-nxin; // negative in output (already initialized)
        for(j = 0; j < nyin; j++) {
            j2 = (j < cy) ? j : j + nyout-nyin;
            for (l = 0; l < nz; l++) {
                out[i2 + j2*nxout + l*nxout*nyout] *= 0.5;
                out[i + j2*nxout + l*nxout*nyout] = out[i2 + j2*nxout + l*nxout*nyout];
            }
        }
    }

    if ( !(nyin%2) && nyout>nyin) {
        j = cy; // positive in output and negative in input
        j2 = cy + nyout-nyin; // negative in output (already initialized)
        for(i = 0; i < nxin; i++) {
            i2 = (i < cx) ? i : i + nxout-nxin;
            for (l = 0; l < nz; l++) {
                out[i2 + j2*nxout + l*nxout*nyout] *= 0.5;
                out[i2 + j*nxout + l*nxout*nyout] = out[i2 + j2*nxout + l*nxout*nyout];
            }
        }
    }
    
    if( !(nxin%2) && !(nyin%2) && nxout>nxin && nyout>nyin) {
        i = cx; // positive in output and negative in input
        i2 = cx + nxout-nxin; // negative in output
        j = cy; // positive in output and negative in input
        j2 = cy + nyout-nyin; // negative in output
        double factor = interp ? 0.25*norm : 0.5*norm;
        for (l = 0; l < nz; l++) {
            double complex hf = factor*in[i + j*nxin + l*nxin*nyin];
            out[i + j*nxout + l*nxout*nyout] = out[i2 + j*nxout + l*nxout*nyout] = hf;
            if( interp ) // real convention case
                out[i + j2*nxout + l*nxout*nyout] = out[i2 + j2*nxout + l*nxout*nyout] = hf;
        }
    }
}

// Up-sampling of an image using TPI (Algorithm 3).
void upsampling(double *out, double *in, int nxin, int nyin, int nxout, int nyout, int nz, int interp) 
{
    // start threaded fftw if FFTW_NTHREADS is defined
    #ifdef FFTW_NTHREADS
    fftw_init_threads();
    #ifdef _OPENMP
    fftw_plan_with_nthreads(omp_get_max_threads());
    #endif
    #endif

    // allocate memory for fourier transform
    fftw_complex *inhat = fftw_malloc(nxin*nyin*nz*sizeof*inhat);
    fftw_complex *outhat = fftw_malloc(nxout*nyout*nz*sizeof*outhat);

    // compute DFT of the input
    do_fft_real(inhat, in, nxin, nyin, nz);

    // phase shift (complex convention)
    upsampling_fourier(outhat, inhat, nxin, nyin, nxout, nyout, nz, interp);

    // compute iDFT of the input
    do_ifft_real(out, outhat, nxout, nyout, nz);

    // free memory and cleanup
    fftw_cleanup();
    #ifdef FFTW_NTHREADS
    fftw_cleanup_threads(); 
    #endif
    fftw_free(inhat);
    fftw_free(outhat);
}

// Compute the DFT coefficients of the down-sampled image (Line 2 of Algorithm 4 using Proposition 12).
static void downsampling_fourier(fftw_complex *out, fftw_complex *in,
                                 int nxin, int nyin, int nxout, int nyout, int nz)
{
    int i, j, l, i2, j2;
    
    // normalization factor
    double norm = nxout*nyout*1.0/(nxin*nyin);
    
    // central indices
    int cx = (nxout+1)/2; 
    int cy = (nyout+1)/2;

    // first pass of filling
    for(j = 0; j < nyout; j++) {
        j2 = (j < cy) ? j : j + nyin-nyout;
        for(i = 0; i < nxout; i++){
            i2 = (i < cx) ? i : i + nxin-nxout;
            for (l = 0; l < nz; l++)
                out[i + j*nxout + l*nxout*nyout] = in[i2 + j2*nxin + l*nxin*nyin]*norm;
        }
    }

    // handling of boundary coefficients
    if ( nxout%2 == 0 && nxout<nxin) {
        i2 = cx; // border index in output AND opposite border index in input
        for (j = 0; j < nyout; j++) { // index in output
            j2 = (j < cy) ? j : j + nyin-nyout; // index in input
            for (l = 0; l < nz; l++)
                out[i2 + j*nxout + l*nxout*nyout] += in[i2 + j2*nxin + l*nxin*nyin]*norm;
        }
    }

    if ( nyout%2 == 0 && nyout<nyin) {
        j2 = cy; // border index in output AND opposite border index in input
        for (i = 0; i < nxout; i++) { // index in output
            i2 = (i < cx) ? i : i + nxin-nxout; // index in input
            for (l = 0; l < nz; l++)
                out[i + j2*nxout + l*nxout*nyout] += in[i2 + j2*nxin + l*nxin*nyin]*norm;
        }
    }

    if ( nxout%2 == 0 && nyout%2 == 0 && nxout<nxin && nyout<nyin) {
        i = cx; // border index in output AND opposite border index in input
        i2 = cx + nxin-nxout; // border index in input
        j = cy; // border index in output AND opposite border index in input
        j2 = cy + nyin-nyout; // border index in input
        for (l = 0; l < nz; l++)
            out[i + j*nxout + l*nxout*nyout] = norm*(
                    in[i + j*nxin + l*nxin*nyin] 
                + in[i2 + j*nxin + l*nxin*nyin]
                + in[i + j2*nxin + l*nxin*nyin] 
                + in[i2 + j2*nxin + l*nxin*nyin]);
    }
}

// Down-sampling of an image using TPI (Algorithm 4).
void downsampling(double *out, double *in, int nxin, int nyin, int nxout, int nyout, int nz) 
{
    // allocate memory for fourier transform
    fftw_complex *inhat = fftw_malloc(nxin*nyin*nz*sizeof*inhat);
    fftw_complex *outhat = fftw_malloc(nxout*nyout*nz*sizeof*outhat);

    // compute DFT of the input
    do_fft_real(inhat, in, nxin, nyin, nz);
    
    // phase shift (complex convention)
    downsampling_fourier(outhat, inhat, nxin, nyin, nxout, nyout, nz);
    
    // compute iDFT of the input
    do_ifft_real(out, outhat, nxout, nyout, nz);
    
    // free memory and cleanup
    fftw_free(inhat);
    fftw_free(outhat);
}