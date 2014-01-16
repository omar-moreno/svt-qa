#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: January 09, 2014
#

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 2.6)

# Set the name of the project
project(SVT_DAQ_ANALYSIS)

# Set the current version
set(SVT_DAQ_ANALYSIS_VERSION_MAJOR 1)
set(SVT_DAQ_ANALYSIS_VERSION_MINOR 0)

# Set the output directories of the binaries, and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${SVT_DAQ_ANALYSIS_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${SVT_DAQ_ANALYSIS_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${SVT_DAQ_ANALYSIS_BINARY_DIR}/lib")

# Include utils headers
set(UTILS_HDR_DIR "${SVT_DAQ_ANALYSIS_SOURCE_DIR}/include/utils")
include_directories("${UTILS_HDR_DIR}")

# Include tracker headers
set(TRACKER_HDR_DIR "$ENV{DAQ_HOME}/tracker")
include_directories("${TRACKER_HDR_DIR}")

# Include generic headers
set(GENERIC_HDR_DIR "$ENV{DAQ_HOME}/generic")
include_directories("${GENERIC_HDR_DIR}")


# Include the ROOT headers
include_directories("$ENV{ROOTSYS}/include")

# Add the utils subdirectory to the build
add_subdirectory("${SVT_DAQ_ANALYSIS_SOURCE_DIR}/src/utils")

# Add the analysis subdirectory to the build
add_subdirectory("${SVT_DAQ_ANALYSIS_SOURCE_DIR}/src/analysis")

