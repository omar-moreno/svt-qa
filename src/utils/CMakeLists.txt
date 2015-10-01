
add_definitions("-std=c++11")

# Find the required ROOT libraries
find_library(ROOT_TREE_LIB Tree $ENV{ROOTSYS}/lib)
find_library(ROOT_CORE_LIB Core $ENV{ROOTSYS}/lib)
find_library(ROOT_RIO_LIB RIO $ENV{ROOTSYS}/lib)
find_library(ROOT_PHYSICS_LIB Physics $ENV{ROOTSYS}/lib)
find_library(ROOT_HIST_LIB Hist $ENV{ROOTSYS}/lib)
find_library(ROOT_GPAD_LIB Gpad $ENV{ROOTSYS}/lib)

find_library(YAML_CPP_LIB yaml-cpp $ENV{YAML_CPP_LIB_PATH})

add_library(PairHash PairHash.cxx)

# Create the PlotUtils library
add_library(PlottingUtils PlottingUtils.cxx)
target_link_libraries(PlottingUtils ${ROOT_TREE_LIB})
target_link_libraries(PlottingUtils ${ROOT_CORE_LIB})
target_link_libraries(PlottingUtils ${ROOT_RIO_LIB})
target_link_libraries(PlottingUtils ${ROOT_PHYSICS_LIB})

# Create the Apv25Utils library
add_library(Apv25Utils Apv25Utils.cxx)
target_link_libraries(Apv25Utils ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB})

add_library(QAUtils QAUtils.cxx)

# Create the SvtCalibrations library
add_library(SvtCalibrations SvtCalibrations.cxx)

add_library(RootFileReader RootFileReader.cxx)
target_link_libraries(RootFileReader 
    ${ROOT_CORE_LIB}
    ${ROOT_GPAD_LIB}
    ${ROOT_TREE_LIB}
    ${ROOT_RIO_LIB}
    ${ROOT_PHYSICS_LIB}
    ${ROOT_HIST_LIB}
)

add_library(ComparePlots ComparePlots.cxx)
target_link_libraries(ComparePlots 
    ${ROOT_CORE_LIB}
    ${ROOT_GPAD_LIB}
    ${ROOT_TREE_LIB}
    ${ROOT_RIO_LIB}
    ${ROOT_PHYSICS_LIB}
    ${ROOT_HIST_LIB}
    ${ROOT_GRAF_LIB}
    RootFileReader
)

add_executable(compare_plots compare_plots.cxx)
target_link_libraries(compare_plots 
   ${YAML_CPP_LIB}
   ComparePlots   
)
