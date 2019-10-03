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
#include "shift_transform.h"
#include "fft_core.h"

#define PAR_DEFAULT_INTERP 1

// display help usage
void print_help(char *name)
{
    printf("\n<Usage>: %s input output dx dy [interp]\n\n", name);
    
    printf("The parameter \"interp\" controls the trigonometric polynomial interpolator type (by default %i)\n", PAR_DEFAULT_INTERP);
    printf("\t 0 --> Real part of complex convention\n");
    printf("\t 1 --> Real convention\n");
}

// Main function for the DFT translation of an image (Algorithm 1).
int main(int c, char *v[])
{
    // initialize FFTW
    init_fftw();
    
    if (c < 5) {
        print_help(v[0]);
        return EXIT_SUCCESS;
    }
    
    char *filename_in  = v[1];
    char *filename_out = v[2];
    double dx = atof(v[3]);
    double dy = atof(v[4]);
    int interp = c > 5 ? atoi(v[5]) : PAR_DEFAULT_INTERP;

    // read image
    int w, h, pd;
    double *in = iio_read_image_double_split(filename_in, &w, &h, &pd);
    
    // initialize time
    unsigned long t1 = xmtime();
    
    // memory allocation
    double *out = malloc(w*h*pd*sizeof*out);
    
    // apply shift using DFT computations
    interpolate_image_shift(out, in, w, h, pd, dx, dy, interp);

    // final time and print time
    unsigned long t2 = xmtime();
    printf("Interpolation made in %.3f seconds \n",(float) (t2-t1)/1000);
    
    // write output image
    iio_write_image_double_split(filename_out, out, w, h, pd);
    
    // free memory
    free(in);
    free(out);
    clean_fftw();
    
    return EXIT_SUCCESS;
}
