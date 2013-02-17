cmake_minimum_required (VERSION 2.6)
# Project Name.
project (onyx-test)

# Additional cmake modules
set     (CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/cmake_modules")

# pre-configure
include (mymacro)
message (STATUS "CMAKE_SOURCE_DIR  = ${CMAKE_SOURCE_DIR}")
message (STATUS "PROJECT_BINARY_DIR= ${PROJECT_BINARY_DIR}")
message (STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

PRE_CONFIGURE()

# configure source code
add_subdirectory(src)
