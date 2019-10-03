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

// Compute the inverse of an homography.
// The 3x3 matrix is inverted using closed-form formulas.
void invert_homography(double iH[9], const double H[9])
{
  double det = H[0] * (H[4]*H[8] - H[5] * H[7]);
  det -= H[1] * (H[3]*H[8] - H[5] * H[6]);
  det += H[2] * (H[3]*H[7] - H[4] * H[6]);

  double tmp = 1.0/det;

  iH[0] = tmp * (H[4] * H[8] - H[5] * H[7]);
  iH[3] = tmp * (H[5] * H[6] - H[3] * H[8]);
  iH[6] = tmp * (H[3] * H[7] - H[4] * H[6]);

  iH[1] = tmp * (H[2] * H[7] - H[1] * H[8]);
  iH[4] = tmp * (H[0] * H[8] - H[2] * H[6]);
  iH[7] = tmp * (H[1] * H[6] - H[0] * H[7]);

  iH[2] = tmp * (H[1] * H[5] - H[2] * H[4]);
  iH[5] = tmp * (H[2] * H[3] - H[0] * H[5]);
  iH[8] = tmp * (H[0] * H[4] - H[1] * H[3]);
}

// Compute the image y of the vector x by the homography H.
void apply_homography(double y[2], const double x[2], const double H[9])
{
  double z[3];
  z[0] = H[0]*x[0] + H[1]*x[1] + H[2];
  z[1] = H[3]*x[0] + H[4]*x[1] + H[5];
  z[2] = H[6]*x[0] + H[7]*x[1] + H[8];
  y[0] = z[0]/z[2];
  y[1] = z[1]/z[2];
}


