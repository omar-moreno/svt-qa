
# Find the required ROOT libraries
find_library(ROOT_TREE_LIB Tree $ENV{ROOTSYS}/lib)
find_library(ROOT_CORE_LIB Core $ENV{ROOTSYS}/lib)
find_library(ROOT_RIO_LIB RIO $ENV{ROOTSYS}/lib)
find_library(ROOT_PHYSICS_LIB Physics $ENV{ROOTSYS}/lib)
find_library(ROOT_GPAD_LIB Gpad $ENV{ROOTSYS}/lib)
find_library(ROOT_HIST_LIB Hist $ENV{ROOTSYS}/lib)
find_library(ROOT_GRAF_LIB Graf $ENV{ROOTSYS}/lib)
find_library(ROOT_MATHCORE_LIB MathCore $ENV{ROOTSYS}/lib)
find_library(ROOT_MATRIX_LIB Matrix $ENV{ROOTSYS}/lib)
find_library(ROOT_CINT_LIB Cint $ENV{ROOTSYS}/lib)
find_library(ROOT_THREAD_LIB Thread $ENV{ROOTSYS}/lib)
find_library(ROOT_SQLIO_LIB SQLIO $ENV{ROOTSYS}/lib)
find_library(ROOT_NET_LIB Net $ENV{ROOTSYS}/lib)

# Find the required SVT DAQ files
LIST(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".o")
find_library(TRACKER_EVENT_LIB TrackerEvent.o $ENV{DAQ_HOME}/.obj)
find_library(TRACKER_SAMPLE_LIB TrackerSample.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_READ_LIB DataRead.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_LIB Data.o $ENV{DAQ_HOME}/.obj)
find_library(XMLVARIABLES_LIB XmlVariables.o $ENV{DAQ_HOME}/.obj)

# Find xml libraries
find_library(XML_LIB xml2 /usr/lib)

add_executable(baseline baseline.cpp)
target_link_libraries(baseline ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB} PlotUtils Apv25Utils ${TRACKER_EVENT_LIB} ${TRACKER_SAMPLE_LIB} ${DATA_READ_LIB} ${DATA_LIB} ${ROOT_GPAD_LIB} ${ROOT_HIST_LIB} ${ROOT_GRAF_LIB} ${ROOT_MATHCORE_LIB} ${XMLVARIABLES_LIB} ${ROOT_MATRIX_LIB} ${ROOT_CINT_LIB} ${ROOT_THREAD_LIB} ${ROOT_SQLIO_LIB} ${XML_LIB} ${ROOT_NET_LIB}) 

add_executable(apv25_qa apv25_qa.cpp)
target_link_libraries(apv25_qa ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB} PlotUtils Apv25Utils ${TRACKER_EVENT_LIB} ${TRACKER_SAMPLE_LIB} ${DATA_READ_LIB} ${DATA_LIB} ${ROOT_GPAD_LIB} ${ROOT_HIST_LIB} ${ROOT_GRAF_LIB} ${ROOT_MATHCORE_LIB} ${XMLVARIABLES_LIB} ${ROOT_MATRIX_LIB} ${ROOT_CINT_LIB} ${ROOT_THREAD_LIB} ${ROOT_SQLIO_LIB} ${XML_LIB} ${ROOT_NET_LIB} SvtCalibrations) 

add_executable(apv25_gain apv25_gain.cxx)
target_link_libraries(apv25_gain ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB} PlotUtils ${TRACKER_EVENT_LIB} ${TRACKER_SAMPLE_LIB} ${DATA_READ_LIB} ${DATA_LIB} ${ROOT_GPAD_LIB} ${ROOT_HIST_LIB} ${ROOT_GRAF_LIB} ${ROOT_MATHCORE_LIB} ${XMLVARIABLES_LIB} ${ROOT_MATRIX_LIB} ${ROOT_CINT_LIB} ${ROOT_THREAD_LIB} ${ROOT_SQLIO_LIB} ${XML_LIB} ${ROOT_NET_LIB}) 
