cmake_minimum_required(VERSION 2.8)
project(nlmeans)

# The build type "Release" adds some optimizations
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif ()

# Are we using gcc?
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # GCC on MacOs needs this option to use the clang assembler
  if (APPLE)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wa,-q")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-q")
  endif ()
  # Optimize to the current CPU and enable warnings
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -Wall -Wextra")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra")
endif ()

# Enable C99
if (CMAKE_VERSION VERSION_LESS "3.1")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
else ()
  set (CMAKE_C_STANDARD 99)
endif ()

# Enable OpenMP
find_package (OpenMP REQUIRED)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

# Link LibTIFF, LibJPEG, LibPNG, OpenMP
find_path (IIO_INCLUDE_DIR iio.h)
find_library (IIO_LIBRARIES NAMES iio)
include_directories (SYSTEM ${iio_INCLUDE_DIR})
link_libraries (${IIO_LIBRARIES})
if (NOT IIO_INCLUDE_DIR)
  message (FATAL_ERROR "IIO not found.")
endif ()

set(SOURCE_FILES nlmeans.cpp libauxiliar.cpp libauxiliar.h libdenoising.cpp libdenoising.h)

add_executable(nlmeans ${SOURCE_FILES})


