#! /bin/tcsh

#setenv DISPLAY:0.0
setenv LD_LIBRARY_PATH {$LD_LIBRARY_PATH}:/usr/local/szip2.1/lib
# above line should be equivalent to
#setenv LD_LIBRARY_PATH /usr/local/szip2.1/lib:/usr/local/cuda-6.5/lib64:/usr/lib64:/usr/lib

#while ( 1 )
  cd /home/mstereo/a/kenneth/misr-stereo-2013.11.24
  bin/misr-stereo /home/mstereo/a/sbansal6/ellipsoids/73138 AN AA
#end

