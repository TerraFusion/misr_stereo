#! /bin/tcsh

# Upper-level directory for misr-stereo
set misrpath=/home/kenneth/misr-stereo
# Qt qmake executable for current machine
set qtmake=/usr/lib64/qt4/bin/qmake

cd $misrpath/hdfeos
$qtmake -o Makefile hdfeos.pro
make

cd $misrpath/gctp
$qtmake -o Makefile gctp.pro
make

cd $misrpath/stereo
$qtmake -o Makefile stereo.pro
make


