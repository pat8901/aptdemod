cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pg")

# Setting project name
project(aptdemod)

# Find libraries. find_package can not be used as the libraries do not have their own CMakeLists.txt files.
find_library(SNDFILE sndfile REQUIRED)
find_library(FFTW fftw3 REQUIRED)

if(SNDFILE)
    set(SNDFILE_FOUND TRUE)
endif()

if(SNDFILE_FOUND)
    MESSAGE(STATUS "sndfile was found!")
    else()
        MESSAGE(WARNING "sndfile not found!")
endif(SNDFILE_FOUND)

if(FFTW)
    set(FFTW_FOUND TRUE)
endif()

if(FFTW_FOUND)
    message(STATUS "FFTW was found!")
    else()
        message(WARNING "FFTW was not found!")
endif()

# find_package(SndFile REQUIRED)
# find_package(FFTW REQUIRED)

# Setting local source files
set(EXE_C_SOURCE_FILES src-core/main.c src-core/audio.c src-core/demod.c src-core/utils.c src-core/image.c)

# Setting external libs include directories. This may not be necessary
set(SNDFILE_INCLUDE_DIR /usr/include)
set(FFTW_INCLUDE_DIR /usr/include)

add_executable(${PROJECT_NAME} ${EXE_C_SOURCE_FILES})

# Including header files
target_include_directories(${PROJECT_NAME} PUBLIC /usr/include ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${SNDFILE_INCLUDE_DIR})

# Processing external libaries
if(SNDFILE_FOUND AND FFTW_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC -L"/usr/lib/x86_64-linux-gnu/" ${SNDFILE} ${FFTW} -lm)
    message(STATUS "Build complete!")
    else()
        MESSAGE(WARNING "External libaries were not found!")
endif()
