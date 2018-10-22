
add_definitions("-std=c++11")

find_library(ROOT_HIST_LIB Hist $ENV{ROOTSYS}/lib)

#add_library(CalGroupsPlot CalGroupsPlot.cxx)
#target_link_libraries(CalGroupsPlot 
#    ${ROOT_CORE_LIB}
#    ${ROOT_HIST_LIB}
#    ${ROOT_GPAD_LIB}
#    ${ROOT_GRAF_LIB}
#)
