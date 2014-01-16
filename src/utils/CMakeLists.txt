#
#	@author: Omar Moreno <omoreno1@ucsc.edu>
#	@date: January 09, 2014
#

# Find the required ROOT libraries
find_library(ROOT_TREE_LIB Tree $ENV{ROOTSYS}/lib)
find_library(ROOT_CORE_LIB Core $ENV{ROOTSYS}/lib)
find_library(ROOT_RIO_LIB RIO $ENV{ROOTSYS}/lib)
find_library(ROOT_PHYSICS_LIB Physics $ENV{ROOTSYS}/lib)

# Create the PlotUtils library
add_library(PlotUtils PlotUtils.cpp)
target_link_libraries(PlotUtils ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB})

# Create the Apv25Utils library
add_library(Apv25Utils Apv25Utils.cpp)
target_link_libraries(Apv25Utils ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB})

# Create the SvtCalibrations library
add_library(SvtCalibrations SvtCalibrations.cpp)
