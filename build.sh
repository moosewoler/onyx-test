#!/bin/sh

PATH='/opt/onyx/arm/bin:/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/:'${PATH}
#ONYX_SDK_ROOT='/opt/onyx/arm'
PKG_CONFIG_PATH='/opt/onyx/arm/lib/pkgconfig/'
QMAKESPEC='/opt/onyx/arm/mkspecs/qws/linux-arm-g++/'
ARM_CC="/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc"
ARM_CXX="/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-g++"

install -d build/arm
cd build/arm && CC=${ARM_CC} CXX=${ARM_CXX} cmake ../..
make -j2
