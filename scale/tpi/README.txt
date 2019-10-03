# Trigonometric Polynomial Interpolation of Images #

## Summary ##
This repository contains implementations of the following operations using trigonometric polynomial interpolation:
DFT translation, homographic transformation, up-sampling and down-sampling  
It is part of an [IPOL publication](https://doi.org/10.5201/ipol.2019.273)

## Authors ##

* Thibaud Briand <briand.thibaud@gmail.com>

Laboratoire d'Informatique Gaspard Monge (LIGM)/ Ecole des Ponts ParisTech
Centre de mathématiques et de leurs applications (CMLA)/ ENS Paris-Saclay

## Version ##

Version 1.0, released on 09/09/2019

## License ##

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2018-2019, Thibaud Briand <briand.thibaud@gmail.com>

All rights reserved.

## Build ##

Required environment: Any unix-like system with a standard compilation
environment (make and C compiler) and [Cmake](https://cmake.org/).

Required libraries:
[libpng](http://libpng.org/pub/png/libpng.html),
[lipjpeg](http://ijg.org/),
[libtiff](http://simplesystems.org/libtiff/)
[libfftw3](http://www.fftw.org/)

Optional libraries:
[libopenmp](https://www.openmp.org/)

Build instructions:

     mkdir build
     cd build
     cmake -DCMAKE_BUILD_TYPE=Release ..
     make

It produces programs "homographic_transform", "shift_transform", "upsampling" and "downsampling".

## Usage of shift_transform ##

The program reads an input image, the shift parameters, optionnally takes the type of interpolator and
produces a shifted version of the image using trigonometric polynomial interpolation.
This corresponds to Algorithm 1.

    <Usage>: ./shift_transform input output dx dy [interp]

The parameter "interp" controls the trigonometric polynomial interpolator type (by default 1)
	 0 --> Real part of complex convention
	 1 --> Real convention

Execution examples:

  1.  Horizontal shift of 0.5 pixel with real convention:

       ./shift_transform input.png output.tiff 0.5 0

  2.  Vertical shift of -0.5 pixel with real part of complex convention:

       ./shift_transform input.png output.tiff 0 -0.5 0

## Usage of homographic_transform ##

The program reads an input image, an homography, optionnally takes the type of interpolator and
produces an homographic transformation of the image using trigonometric polynomial interpolation.
This corresponds to Algorithm 2.

   <Usage>: ./homographic_transform input output "h11 h12 h13 h21 h22 h23 h31 h32 h33" [interp]

The parameter "interp" controls the trigonometric polynomial interpolator type (by default 1)
	 0 --> Real part of complex convention
	 1 --> Real convention

Execution examples:

  1.  Horizontal shift of 0.5 pixel with real convention:

       ./homographic_transform input.png output.tiff "1 0 0.5 0 1 0 0 0 1"

  2.  Vertical shift of -0.5 pixel with real part of complex convention:

       ./homographic_transform input.png output.tiff "1 0 0 0 1 -0.5 0 0 1"

## Usage of upsampling ##

The program reads an input image, the up-sampling factors (or the output sizes), optionnally takes the type of interpolator and produces an up-sampled version of the image using trigonometric polynomial interpolation.
This corresponds to Algorithm 3.

   <Usage>: ./upsampling input output valuex valuey [OPTIONS]

The optional parameters are:
-i, 	 Specify the trigonometric polynomial interpolator type (by default 1)
	 	 	 0 --> Real part of complex convention
	 	 	 1 --> Real convention
-t, 	 Specify the input type of valuex and valuey (by default 1)
	 	 	 0 --> Output image sizes
			 1 --> Up-sampling factors (>=1)

Execution examples:

  1.  Up-sampling of factor (2.5,3) with real convention:

       ./upsampling input.png output.tiff 2.5 3

  2.  Up-sampling to have an image of size 1000x2000 with real part of complex convention:

       ./upsampling input.png output.tiff 1000 2000 -i 0 -t 0

## Usage of downsampling ##

The program reads an input image, the down-sampling factors (or the output sizes) and produces a down-sampled version of the image using trigonometric polynomial interpolation.
This corresponds to Algorithm 4.

   <Usage>: ./downsampling input output valuex valuey [OPTIONS]

The optional parameters are:
-t, 	 Specify the input type of valuex and valuey (by default 1)
	 	 	 0 --> Output image sizes
			 1 --> Down-sampling factors (>=1)

Execution examples:

  1.  Down-sampling of factor (2.5,3) with real convention:

       ./downsampling input.png output.tiff 2.5 3

  2.  Down-sampling to have an image of size 100x200:

       ./downsampling input.png output.tiff 100 200 -t 0 

## List of files ##

* CMakeList.txt          : CMake file for the compilation
* LICENSE		 : License file
* README.txt             : This file

In the data/ directory:

* rubberwhale.png        : Test image (color)
* rubberwhale_gray.png   : Test image (grayscale)

In the src/ directory:

* fft_core.[hc]               : Functions related to the FFT
* homographic_transform.[hc]  : Functions to perform Algorithm 2
* homography_core.[hc]	      : Functions related to homographies
* main_downsampling.c         : Main program for input/output (Algorithm 4)
* main_shift.c                : Main program for input/output (Algorithm 1)
* main_transform.c            : Main program for input/output (Algorithm 2)
* main_upsampling.c           : Main program for input/output (Algorithm 3)
* resampling.[hc]	      : Functions to perform Algorithm 3 et Algorithm 4
* shift_transform.[hc]        : Functions to perform Algorithm 1

Additional files are provided in the external/ directory:

* iio.[hc]               : Functions for opening images in any format
* xmtime.h               : Clock with millisecond precision
* nfft-3.5.0             : NFFT3 library

