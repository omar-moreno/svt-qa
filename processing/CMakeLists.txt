# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(processing 
	VERSION 1.0.0          
	DESCRIPTION "Framework used to define processing pipelines and modules."
        LANGUAGES CXX)

# Search for the Python3 library
find_package (Python3 COMPONENTS Interpreter Development)

# Setup the Rogue CMake target
setup_rogue()

file(GLOB CONFIG_FILES CONFIGURE_DEPENDS 
	${PROJECT_SOURCE_DIR}/src/processing/config/[a-zA-Z]*.cxx
	${PROJECT_SOURCE_DIR}/src/processing/[a-zA-Z]*.cxx)

# Setup the library
setup_library(module processing
	      dependencies Python3::Python Rogue::Rogue rogue::rogue
	      sources ${CONFIG_FILES})

# Compiling the Framework library requires features introduced by the cpp 17
# standard.
set_target_properties(
  processing
  PROPERTIES CXX_STANDARD 17
             CXX_STANDARD_REQUIRED YES
             CXX_EXTENSIONS NO)

setup_python(package_name ${PYTHON_PACKAGE_NAME}/processing)
