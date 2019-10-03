cmake_minimum_required(VERSION 2.6)

# Enable C99
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
else()
  set(CMAKE_C_STANDARD 99)
endif()

# Find OPENMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# include source code directory
set(SRC src)
include_directories(${SRC})

# include external code directory
set(EXTERNAL ${SRC}/external)
include_directories(${EXTERNAL})

# include NFFT
add_subdirectory(${EXTERNAL}/nfft-3.5.0)
include_directories(${NFFT3_INCPATH})
link_directories(   ${NFFT3_LIBPATH})

# set libraries
set(LIBS m iio fftw3 fftw3_threads)

# geometric transformation using NFFT
add_executable(tpi_transformhomographic ${SRC}/main_transform.c ${SRC}/homographic_transform.c ${SRC}/fft_core.c ${SRC}/homography_core.c)
add_dependencies(tpi_transformhomographic nfft-3.5.0)
target_link_libraries(tpi_transformhomographic ${LIBS} nfft3_threads)

# shift interpolation
add_executable(tpi_transformshift ${SRC}/main_shift.c ${SRC}/shift_transform.c ${SRC}/fft_core.c)
target_link_libraries(tpi_transformshift ${LIBS})

# upsampling
add_executable(tpi_samplingup ${SRC}/main_upsampling.c ${SRC}/resampling.c ${SRC}/fft_core.c)
target_link_libraries(tpi_samplingup ${LIBS})

# downsampling
add_executable(tpi_samplingdown ${SRC}/main_downsampling.c ${SRC}/resampling.c ${SRC}/fft_core.c)
target_link_libraries(tpi_samplingdown ${LIBS})
