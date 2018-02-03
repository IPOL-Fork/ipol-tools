/**
 * @file   main.cpp
 * @brief  Main executable file
 *
 *
 *
 * @author MARC LEBRUN  <marc.lebrun@cmla.ens-cachan.fr>
 */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "utilities.h"
#include "ksvd.h"

extern "C" {
#include "iio.h"
}

using std::cerr;
using std::endl;

const char *pick_option(int *c, char **v, const char *o, const char *d) {
  int id = d ? 1 : 0;
  for (int i = 0; i < *c - id; i++) {
    if (v[i][0] == '-' && 0 == strcmp(v[i] + 1, o)) {
      char *r = v[i + id] + 1 - id;
      for (int j = i; j < *c - id; j++)
        v[j] = v[j + id + 1];
      *c -= id + 1;
      return r;
    }
  }
  return d;
}

int main(int argc, char **argv) {
  bool useAcceleration = pick_option(&argc, argv, "a", NULL);
  if (argc != 4) {
    cerr << "usage: " << argv[0] << " input sigma output [-a]" << endl;
    cerr << "\t-a use acceleration" << endl;
    return EXIT_FAILURE;
  }

  //! read input image
  int height, width, chnls;
  float *img = iio_read_image_float_split(argv[1], &width, &height, &chnls);
  double fSigma = atof(argv[2]);

  int whc = width * height * chnls;
  float *img_denoised = new float[whc];
  double *img_noisy = new double[whc];

  for (int i = 0; i < whc; ++i) img_noisy[i] = img[i];

  //! Denoising
  ksvd_ipol((double) fSigma / 255.0l,
            img_noisy,
            img_denoised,
            width,
            height,
            chnls,
            useAcceleration);

  iio_save_image_float_split(argv[3], img_denoised, width, height, chnls);

  //! Free Memory
  delete[] img_denoised;
  free(img);

  return EXIT_SUCCESS;
}



