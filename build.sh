#! /bin/bash

# Script to build misr-stereo on keeling.earth.illinois.edu
#
# Current prerequisite software versions (7/12/2012):
#  freeglut-2.6.0-1 freeglut-devel-2.6.0-1 (RPMs)
#  hdf-4.2.5-1 hdf-devel-4.2.5-1 (RPMs)
#  libXmu-1.0.5-1 libXmu-devel-1.0.5-1 (RPMs)
#  qt-devel-4.6.2-24 (RPM)
#  szip 2.1 (built from source)
#
# This directory (in which to build bin/misr-stereo)
misrpath=/home/landon/misr_stereo

# Path to the HDF 4 include files
hdfincdir=/usr/local/include/
# Path to the HDF 4 libraries
hdflibdir=/usr/local/lib/
# Path to the "qmake" Qt executable
qtbin=/usr/bin/
# Base directory for szip (contains include/, lib/)
szbase=/usr/local/

cat > $misrpath/stereo/config.h << EOF
#ifndef PACKAGE_DATA_DIR
#define PACKAGE_DATA_DIR "$misrpath"
#endif
EOF


make MISRDIR="$misrpath" HDF4INC="$hdfincdir" HDF4LIB="$hdflibdir" QTBIN="$qtbin" SZIPDIR="$szbase" bin/misr-stereo

echo 
echo "To run misr-stereo, the following must appear in your LD_LIBRARY_PATH:"
echo $szbase "/lib"
echo "Running to a remote display may also require the environment setting:"
echo "setenv LIBGL_ALWAYS_INDIRECT 1"
echo
echo "Using a 3D TV as a 2nd display may require the environment setting:"
echo "setenv DISPLAY :0.1"
echo
echo "'MISRDIR' (/stereo/stereo.pro [line 13]) must be set to misr-stereo directory" 
echo " ex. MISRDIR = /home/mstereo/a/<USERNAME>/misr-stereo"
echo
echo "Usage: ./bin/misr-stereo <orbit_dir> <lcam> <rcam>"
echo "  <orbit_dir> - Directory containing MISR *.hdf files"
echo "  <lcam> - Camera for left view (ex. AN)"
echo "  <rcam> - Camera for right view (ex. AA)"
echo
