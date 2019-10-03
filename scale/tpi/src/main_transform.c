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
#include <iio.h>

#include "xmtime.h"
#include "homographic_transform.h"
#include "fft_core.h"

#define PAR_DEFAULT_INTERP 1

// Function to transform char of the form "v0 v1 ..." into an array
// of doubles t where t[i] = vi.
// Taken from parsenumbers.c in the imscript repository:
// https://github.com/mnhrdt/imscript
static int parse_doubles(double *t, int nmax, const char *s)
{
    int i = 0, w;
    while (i < nmax && 1 == sscanf(s, "%lg %n", t + i, &w)) {
            i += 1;
            s += w;
    }
    return i;
}

// display help usage
void print_help(char *name)
{
    printf("\n<Usage>: %s input output \"h11 h12 h13 h21 h22 h23 h31 h32 h33\" [interp]\n\n", name);
    
    printf("The parameter \"interp\" controls the trigonometric polynomial interpolator type (by default %i)\n", PAR_DEFAULT_INTERP);
    printf("\t 0 --> Real part of complex convention\n");
    printf("\t 1 --> Real convention\n");
}

// Main function for the geometric transformation of an image
// using the NFFT algorithm (Algorithm 2).
int main(int c, char *v[])
{
    // initialize FFTW
    init_fftw();
    
    if (c < 3) {
        print_help(v[0]);
        return EXIT_SUCCESS;
    }
    
    char *filename_in  = v[1];
    char *filename_out = v[2];
    char *input_params = v[3];
    int interp = c > 4 ? atoi(v[4]) : PAR_DEFAULT_INTERP;
    
    /* Read transformation */
    double H[9];
    int maxparam = 9;
    parse_doubles(H, maxparam, input_params);
    
    // read image
    int w, h, pd;
    double *in = iio_read_image_double_split(filename_in, &w, &h, &pd);
    
    // initialize time
    unsigned long t1 = xmtime();
    
    // memory allocation
    double *out = malloc(w*h*pd*sizeof*out);
    
    // homographic transformation of the image using NFFT 
    // the boundary condition is implicitly periodic
    interpolate_image_homography(out, in, w, h, pd, H, interp);
    
    // final time and print time
    unsigned long t2 = xmtime();
    printf("Interpolation made in %.3f seconds \n", (float) (t2-t1)/1000);

    // write output image
    iio_write_image_double_split(filename_out, out, w, h, pd);
    
    // free memory
    free(in);
    free(out);
    clean_fftw();
    
    return EXIT_SUCCESS;
}
