
# Find the EVIO libraries
find_library(EVIO_LIB evio $ENV{CODA}/Linux-x86_64/lib)
find_library(EVIOXX_LIB evioxx $ENV{CODA}/Linux-x86_64/lib)

# Find the SVT DAQ libraries
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".o")
find_library(DATAREAD_LIB DataRead.o $ENV{DAQ_HOME}/.obj)
find_library(TESTRUN_SVT_EVENT_LIB TestRunSvtEvent.o $ENV{DAQ_HOME}/.obj)

add_library(DataReadEvio DataReadEvio.cxx)
target_link_libraries(DataReadEvio ${DATAREAD_LIB})
target_link_libraries(DataReadEvio ${TESTRUN_SVT_EVENT_LIB})
target_link_libraries(DataReadEvio ${EVIOXX_LIB})
target_link_libraries(DataReadEvio ${EVIO_LIB})
