cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 99)
# Setting project name
project(aptdemod)

# Checking if build type was selected
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to Release as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type: Debug, Release" FORCE)
endif()

# Build flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_DEBUG_FLAGS} -g -O0 -Wall -Werror -Wextra -fsanitize=address -fsanitize=undefined -pg")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -s")

# Find/build libraries
find_library(SNDFILE sndfile REQUIRED)
find_library(FFTW fftw3 REQUIRED)
message(STATUS "Building Unity.")
add_library(Unity STATIC lib/Unity-2.6.1/src/unity.c)
target_include_directories(Unity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/Unity-2.6.1/src)

# Error check find/build libraries
if(SNDFILE)
    set(SNDFILE_FOUND TRUE)
endif()

if(FFTW)
    set(FFTW_FOUND TRUE)
endif()

if(SNDFILE_FOUND)
    MESSAGE(STATUS "sndfile was found!")
    else()
        MESSAGE(WARNING "sndfile was not found!")
endif(SNDFILE_FOUND)

if(FFTW_FOUND)
    message(STATUS "FFTW was found!")
    else()
        message(WARNING "FFTW was not found!")
endif()

# Setting local source files
set(EXE_C_SOURCE_FILES src-core/main.c src-core/audio.c src-core/demod.c src-core/utils.c src-core/image.c)
add_executable(${PROJECT_NAME} ${EXE_C_SOURCE_FILES})

# Including header files
target_include_directories(${PROJECT_NAME} PUBLIC /usr/include ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries
if(SNDFILE_FOUND AND FFTW_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC -L"/usr/lib/x86_64-linux-gnu/" ${SNDFILE} ${FFTW} -lm)
    message(STATUS "Build complete!")
    else()
        MESSAGE(WARNING "External libraries were not found!")
endif()

# Build program binary according to build type selected
# TODO: Do I need to separate the test binaries if they both do the same thing?
# Building library to link to unity
add_library(MYLIB STATIC ${EXE_C_SOURCE_FILES})
target_include_directories(MYLIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Add external libraries to MYLIB
target_include_directories(MYLIB PRIVATE /usr/include)
target_link_libraries(MYLIB PUBLIC ${SNDFILE} ${FFTW} -lm)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add tester executable
    add_executable(aptdemod-debug-test test/Testdemod.c)
    target_include_directories(aptdemod-debug-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(aptdemod-debug-test PRIVATE MYLIB Unity)
    # Add tests
    enable_testing()
    add_test(NAME UnitTests COMMAND aptdemod-debug-test)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Add tester executable
    add_executable(aptdemod-release-test test/Testdemod.c)
    target_include_directories(aptdemod-release-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(aptdemod-release-test PRIVATE MYLIB Unity)
    # Add tests
    enable_testing()
    add_test(NAME UnitTests COMMAND aptdemod-release-test)
endif()

####################################################################################################

# Custom clean target, invoke using "cmake --build . --target clean-all"
if(MSVC)
    # For Windows (Visual Studio)
    add_custom_target(clean-all COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR})
else()
    # For Linux or macOS (or Unix-like systems)
    add_custom_target(clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*)
endif()