
add_executable(CalibrationWriterTest CalibrationWriterTest.cxx)
target_link_libraries(CalibrationWriterTest CalibrationWriter)

# Find the EVIO libraries
find_library(EVIO_LIB evio $ENV{CODA}/Linux-x86_64/lib)
find_library(EVIOXX_LIB evioxx $ENV{CODA}/Linux-x86_64/lib)

# Find the SVT DAQ libraries
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".o")
find_library(DATAREAD_LIB DataRead.o $ENV{DAQ_HOME}/.obj)
find_library(XMLVARIABLES_LIB XmlVariables.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_LIB Data.o $ENV{DAQ_HOME}/.obj)
find_library(TESTRUN_SVT_EVENT_LIB TestRunSvtEvent.o $ENV{DAQ_HOME}/.obj)
find_library(TRACKER_SAMPLE_LIB TestRunSvtTrackerSample.o $ENV{DAQ_HOME}/.obj)

# Find bz2 libraries
find_library(BZ2_LIB bz2 /usr/lib)

# Find xml libraries
find_library(XML_LIB xml2 /usr/lib)

# Find rt libraries
find_library(RT_LIB rt /usr/lib)

# Find the expat libraries
find_library(EXPAT_LIB expat /usr/lib64)

add_executable(DataReadEvioTest DataReadEvioTest.cxx)
target_link_libraries(DataReadEvioTest DataReadEvio)
target_link_libraries(DataReadEvioTest ${TRACKER_SAMPLE_LIB})
target_link_libraries(DataReadEvioTest ${TESTRUN_SVT_EVENT_LIB})
target_link_libraries(DataReadEvioTest ${DATA_LIB})
target_link_libraries(DataReadEvioTest ${DATAREAD_LIB})
target_link_libraries(DataReadEvioTest ${XMLVARIABLES_LIB})
target_link_libraries(DataReadEvioTest ${BZ2_LIB})
target_link_libraries(DataReadEvioTest ${XML_LIB})
target_link_libraries(DataReadEvioTest ${RT_LIB})
target_link_libraries(DataReadEvioTest ${EVIOXX_LIB})
target_link_libraries(DataReadEvioTest ${EVIO_LIB})
target_link_libraries(DataReadEvioTest ${EXPAT_LIB})
