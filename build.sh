#!/bin/bash

# based on https://gist.github.com/kevinmehall/e965b1432c200f95f06f

# realative to build folder
TARGET_CROSS="../tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-"

export CC="${TARGET_CROSS}gcc"
export CXX="${TARGET_CROSS}g++"
export AR=${TARGET_CROSS}ar
export RANLIB=${TARGET_CROSS}ranlib
export STRIP=${TARGET_CROSS}strip
export OBJCOPY=${TARGET_CROSS}objcopy
export OBJDUMP=${TARGET_CROSS}objdump
export NM=${TARGET_CROSS}nm
export AS=${TARGET_CROSS}as

node-gyp clean configure build --verbose --arch=armv6 --target=v8.11.4 -j 4
