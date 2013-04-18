#!/bin/sh
#For using under win32 only!

export HOME_ROOT=`pwd`
export PATH=$HOME_ROOT/scripts:$PATH

cd bin/win32
. ./setenv.sh

if [ "$REDIRECT_PATH" == "" ];then
  cd ../..
else
  cd $REDIRECT_PATH
fi

echo "Arduino-Lite Console"
echo "Powered by RoboPeak Project (robopeak.com)"
echo "------------------------------------------"
echo ""
echo "Entering bash..."
echo "Current Dir is: "
echo "   "`pwd`

if [ "$SH_CMD" == "" ];then
 sh
else
  $SH_CMD
fi
