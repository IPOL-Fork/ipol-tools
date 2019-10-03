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
#include "resampling.h"
#include "fft_core.h"

#define PAR_DEFAULT_INPUT 1

// display help usage
void print_help(char *name)
{
    printf("\n<Usage>: %s input output valuex valuey [OPTIONS]\n\n", name);

    printf("The optional parameter is:\n");
    printf("-t, \t Specify the input type of valuex and valuey (by default %i)\n", PAR_DEFAULT_INPUT);
    printf("\t \t \t 0 --> Output image sizes\n");
    printf("\t \t \t 1 --> Down-sampling factors (>=1)\n");
}

// read command line parameters
int read_parameters(int argc, char *argv[], char **infile, char **outfile,
                    double *factorx, double *factory, int *input_type)
{
    // display usage
    if (argc < 5) {
        print_help(argv[0]);
        return 0;
    }
    else {
        int i = 1;
        *infile  = argv[i++];
        *outfile = argv[i++];
        *factorx  = atof(argv[i++]);
        *factory  = atof(argv[i++]);

        // "default" value initialization
        *input_type    = PAR_DEFAULT_INPUT;

        //read each parameter from the command line
        while(i < argc) {
            if(strcmp(argv[i],"-t")==0)
                if(i < argc-1)
                    *input_type = atoi(argv[++i]);
            i++;
        }

        // check consistency
        if ( !(*factorx >= 1) || !(*factory >= 1) ) {
                printf("Invalid input factor\n");
                return 0;
        }

        return 1;
    }
}

// Main function for the down-sampling of images (Algorithm 4)
int main(int c, char *v[])
{
    // initialize FFTW
    init_fftw();
    
    // declaration of parameters
    char *infile, *outfile;
    double factorx, factory;
    int input_type;

    // read parameters
    int result = read_parameters(c, v, &infile, &outfile,
                                 &factorx, &factory, &input_type);

    if (result ) {

        // read image
        int w, h, pd;
        double *in = iio_read_image_double_split(infile, &w, &h, &pd);

        // initialize time
        unsigned long t1 = xmtime(), t2;

        // output sizes
        int wout, hout;
        if ( input_type ) {
            wout = w/factorx;
            hout = h/factory;
            if ( factorx != w*1.0/wout ) {
                factorx = w*1.0/wout;
                printf("Changing horizontal down-sampling factor to %lf\n", factorx);
            }
            if ( factory != h*1.0/hout ) {
                factory = h*1.0/hout;
                printf("Changing vertical down-sampling factor to %lf\n", factory);
            }
        }
        else {
            wout = factorx;
            hout = factory;
            if ( w < wout ) {
                wout = w;
                printf("WARNING: the output width cannot be greater than the input one\n");
            }
            if ( h < hout ) {
                hout = h;
                printf("WARNING: the output height cannot be greater than the input one\n");
            }
        }

        if ( wout == w && hout == h ) { // odd case (nothing to do)
            // final time and print time
            t2 = xmtime();

            // write output image
            iio_write_image_double_split(outfile, in, w, h, pd);

            // free memory
            free(in);
        }
        else {
            // memory allocation
            double *out = malloc(wout*hout*pd*sizeof*out);

            // downsample the image
            downsampling(out, in, w, h, wout, hout, pd);

            // final time and print time
            t2 = xmtime();

            // write output image
            iio_write_image_double_split(outfile, out, wout, hout, pd);

            // free memory
            free(in);
            free(out);
        }

        // print time
        printf("Down-sampling made in %.3f seconds \n",(float) (t2-t1)/1000);
    }

    // clean
    clean_fftw();
    
    return EXIT_SUCCESS;
}
