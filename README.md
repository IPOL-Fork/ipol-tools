% IPOL image tools.

# ABOUT

* Author : Nicola Pierazzo   <nicolapierazzo@gmail.com>
* Author : Gabriele Facciolo <gfacciol@gmail.com>
* Copyright : (C) 2017 IPOL Image Processing On Line http://www.ipol.im/
* Licence   : GPL v3+, see GPLv3.txt
* Based on the 2010 implementation of DCT denoising by:
  Guoshen Yu <yu@cmap.polytechnique.fr> and Guillermo Sapiro <guille@umn.edu>
* Latest version available at: https://github.com/zvezdochiot/ipol-tools

# OVERVIEW

This source code provides an algorithm described in the IPOL article: http://www.ipol.im/

# UNIX/LINUX/MAC USER GUIDE

The code is compilable on Unix/Linux and Mac OS. 

- Compilation. 
Automated compilation requires the Cmake and make.

- Dependencies.
This code requires the libiio (https://github.com/zvezdochiot/libiio).

- Image formats. 
Only the PNG, JPEG, and TIFF (float) formats are supported. 
 
-------------------------------------------------------------------------
Usage:
1. Download the library code and extract it. Go to that directory. 

```
git clone https://github.com/zvezdochiot/libiio
cd libiio
```

2. Compile the library (on Unix/Linux/Mac OS). 

```
ccmake .
cmake .
make
```

3. Install library

```
sudo make install
```

4. Download the code package and extract it. Go to that directory. 

```
git clone https://github.com/zvezdochiot/ipol-tools
cd ipol-tools
```

5. Go to utils directory. 

5. Compile the source code (on Unix/Linux/Mac OS). 
```
ccmake .
cmake .
make
```

To visualize tiff (float) images use PVFLIP (https://github.com/gfacciol/pvflip) 
or ImageJ (https://imagej.nih.gov/ij/index.html)


# ABOUT THIS FILE
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
