#
#   CMakeLists.txt
#   Omar Moreno, SLAC National Accelerator Laboratory
#

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 2.6)

# Set the name of the project
project(SVT_QA)

# Set the current version
set(SVT_QA_VERSION_MAJOR 1)
set(SVT_QA_VERSION_MINOR 0)

# Set the output directories of the binaries, and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${SVT_QA_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${SVT_QA_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${SVT_QA_BINARY_DIR}/lib")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl")

# Tell CMake where to find ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Locate the ROOT package
find_package(ROOT REQUIRED)

# Define ROOT macros
include(${ROOT_USE_FILE})

# Include the libXML headers
include_directories("$ENV{LIBXML_INCLUDE_PATH}")

# Python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#include_directories("$ENV{YAML_CPP_INCLUDE_PATH}")

# Include tracker headers
set(TRACKER_HDR_DIR "$ENV{DAQ_HOME}/tracker")
include_directories("${TRACKER_HDR_DIR}")

# Include generic headers
set(GENERIC_HDR_DIR "$ENV{DAQ_HOME}/generic")
include_directories("${GENERIC_HDR_DIR}")

# Include utils headers
set(UTILS_HDR_DIR "${SVT_QA_SOURCE_DIR}/include/utils")
include_directories("${UTILS_HDR_DIR}")

# Include EVIO headers
include_directories("$ENV{EVIO_INCLUDE_PATH}")

# Include the QA headers 
set(QA_HDR_DIR "${SVT_QA_SOURCE_DIR}/include/qa")
include_directories("${QA_HDR_DIR}")

# Include the potting headers
set(PLOTTING_HDR_DIR "${SVT_QA_SOURCE_DIR}/include/plotting")
include_directories("${PLOTTING_HDR_DIR}")

# Include the xml headers
set(XML_HDR_DIR "${SVT_QA_SOURCE_DIR}/include/xml")
include_directories("${XML_HDR_DIR}")

# Include the daq headers
set(DAQ_HDR_DIR "${SVT_QA_SOURCE_DIR}/include/daq")
include_directories("${DAQ_HDR_DIR}")

# Add the xml directory to the build 
add_subdirectory("${SVT_QA_SOURCE_DIR}/src/xml")

# Add the utils subdirectory to the build
add_subdirectory("${SVT_QA_SOURCE_DIR}/src/utils")

# Add the plotting subdirectory to the build
add_subdirectory("${SVT_QA_SOURCE_DIR}/src/plotting")

# Add the analysis subdirectory to the build
add_subdirectory("${SVT_QA_SOURCE_DIR}/src/qa")

# Add the daq directory to the build
add_subdirectory("${SVT_QA_SOURCE_DIR}/src/daq")

# Add the test directory to the build
#add_subdirectory("${SVT_QA_SOURCE_DIR}/test")
