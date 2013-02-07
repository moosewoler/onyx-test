macro       (PRE_CONFIGURE)
    # set up cmake variables
    set (BUILD_FOR_ARM      ON)
    set (ONYX_SDK_ROOT      "/opt/onyx/arm")
    set (USE_CORTEX_A8      ON)
    set (LINK_ZLIB_DEFAULT  ON)
    set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
    set (LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/libs)
    set (TEST_OUTPUT_PATH       ${PROJECT_BINARY_DIR}/unittests)
    set (QT_PLUGINS_OUTPUT_PATH ${PROJECT_BINARY_DIR}/plugins)
    #enable_testing()

    # Check to use arm toolchain or not
    if(BUILD_FOR_ARM)
        USE_ARM_TOOLCHAIN()
    endif(BUILD_FOR_ARM)

    if(UNIX OR BUILD_FOR_ARM)
        add_definitions(-DSHARE_ROOT="/usr/share")
    else (UNIX OR BUILD_FOR_ARM)
        add_definitions(-DSHARE_ROOT="")
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif (UNIX OR BUILD_FOR_ARM)

    add_definitions(-DCONFIG_CTRL_IFACE)
    add_definitions(-DCONFIG_CTRL_IFACE_UNIX)

    # Project include directories.
    message             (STATUS "QT_LIBRARY_DIR is set to ${QT_LIBRARY_DIR}")
    include_directories (BEFORE ${CMAKE_SOURCE_DIR}/src/include ${CMAKE_FIND_ROOT_PATH}/include ~/Code/freescale/linux-2.6.35.3/include)
    link_directories    (${QT_LIBRARY_DIR})
endmacro    (PRE_CONFIGURE)
