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

#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

// Compute the inverse of an homography.
// The 3x3 matrix is inverted using closed-form formulas.
void invert_homography(double iH[9], const double H[9]);

// Compute the image y of the vector x by the homography H.
void apply_homography(double y[2], const double x[2], const double H[9]);

#endif
