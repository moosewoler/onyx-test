#!/bin/sh
ARCHOPT="arm"

if [ "X${1}" != "X" ];then
    ARCHOPT=${1}
fi

#===============================================================================
#       functions
#===============================================================================
build_for_arm()
{
#    PATH='/opt/onyx/arm/bin:/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/:'${PATH}
#    #ONYX_SDK_ROOT='/opt/onyx/arm'
#    PKG_CONFIG_PATH='/opt/onyx/arm/lib/pkgconfig/'
#    QMAKESPEC='/opt/onyx/arm/mkspecs/qws/linux-arm-g++/'
    ARM_CC="/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-gcc"
    ARM_CXX="/opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-linux-g++"
    install -d build/arm
    cd build/arm && CC=${ARM_CC} CXX=${ARM_CXX} cmake ../.. -DBUILD_FOR_ARM:BOOL=ON
    make -j2
}

build_for_x86()
{
    install -d build/x86
    cd build/x86 && cmake ../.. -DBUILD_FOR_X86:BOOL=ON
    make -j2
}
#===============================================================================
#       end of functions
#===============================================================================

case ${ARCHOPT} in
    "arm")
        echo "Building for ${ARCHOPT} ..."
        build_for_arm
        exit 0
        ;;
    "x86")
        echo "Building for ${ARCHOPT} ..."
        build_for_x86
        exit 0
        ;;
    *)
        echo "ARCH option ${ARCHOPT} is wrong or not supported yet."
        echo "Build aborted. Exit."
        exit 1
        ;;
esac

