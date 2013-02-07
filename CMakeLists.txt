cmake_minimum_required (VERSION 2.6)
# Project.
project (hoser2.0)

# Additional cmake modules
set     (CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/cmake_modules")

# pre-configure
include (mymacro)
include (arm_toolchain)
include (FindThreads)
include (enable_qt)
include (qt4_wrap_ts)
include (onyx_test)
include (misc)
include (tc)
include (strict_warning)

message (STATUS "${CMAKE_SOURCE_DIR}")
message (STATUS "${PROJECT_BINARY_DIR}")

PRE_CONFIGURE()

# configure source code
add_subdirectory(src)
