# RoboPeak Project
# www.RoboPeak.com
#
#!/bin/sh

#Assumes win32 is the only target platform

cd bin/win32
. ./setenv.sh

echo "Phase1. extracting the full POSIX env."
7z x posix.7z

echo "Phase2. extracting the GCC-AVR( WinAVR for win32 )."
7z x winavr-20090313.7z

