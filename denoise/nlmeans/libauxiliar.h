/*
 * Copyright (c) 2009-2011, A. Buades <toni.buades@uib.es>,
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _LIBAUXILIAR_H_
#define _LIBAUXILIAR_H_


///// LUT tables
#define LUTMAX 30.0
#define LUTMAXM1 29.0
#define LUTPRECISION 1000.0

const char *pick_option(int *c, char **v, const char *o, const char *d);

void wxFillExpLut(float *lut, int size);        // Fill exp(-x) lut

float wxSLUT(float dif, float *lut);                     // look at LUT

void fpClear(float *fpI, float fValue, int iLength);

float fiL2FloatDist(float *u0,
                    float *u1,
                    int i0,
                    int j0,
                    int i1,
                    int j1,
                    int radius,
                    int width0,
                    int width1);

float fiL2FloatDist(float **u0,
                    float **u1,
                    int i0,
                    int j0,
                    int i1,
                    int j1,
                    int radius,
                    int channels,
                    int width0,
                    int width1);

#endif
