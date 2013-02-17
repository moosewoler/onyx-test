# Define x86 tool chain
MACRO (USE_X86_TOOLCHAIN)
    link_directories(${ONYX_SDK_ROOT}/lib)

    set(ONYX_SDK_ROOT           "/opt/some/arch")
    set(CMAKE_FIND_ROOT_PATH    ${ONYX_SDK_ROOT})
    set(CMAKE_INSTALL_PREFIX    ${CMAKE_FIND_ROOT_PATH})

    # For libraries and headers in the target directories
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

    # Set up development build mode
    set (CMAKE_CXX_FLAGS "-s ")

    set (CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Devel."
        FORCE)

    #set (PDF_LIB libfpdfemb_arm.a)
    #set (TTS_LIB AiSound4)

    if (LINK_ZLIB_DEFAULT)
        set     (ADD_LIB m rt pthread dl z)
    else (LINK_ZLIB_DEFAULT)
        set     (ADD_LIB m rt pthread dl)
    endif (LINK_ZLIB_DEFAULT)

    add_definitions(-DBUILD_FOR_ARM)
    add_definitions(-DENABLE_EINK_SCREEN)
    add_definitions(-DBS60_INIT_MAIN)
    add_definitions(-DBS80_INIT_MAIN)
    add_definitions(-DBS97_INIT_MAIN)
    add_definitions(-DSFM_M25P20) 

ENDMACRO(USE_X86_TOOLCHAIN)
