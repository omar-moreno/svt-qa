

add_definitions("-std=c++11")

# Find the required SVT DAQ files
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".o")
#find_library(TRIGGER_SAMPLE_LIB TriggerSample.o $ENV{DAQ_HOME}/.obj)
#find_library(TRIGGER_EVENT_LIB TriggerEvent.o $ENV{DAQ_HOME}/.obj)
find_library(TRACKER_EVENT_LIB TrackerEvent.o $ENV{DAQ_HOME}/.obj)
find_library(TRACKER_SAMPLE_LIB TrackerSample.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_READ_LIB DataRead.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_LIB Data.o $ENV{DAQ_HOME}/.obj)
find_library(XMLVARIABLES_LIB XmlVariables.o $ENV{DAQ_HOME}/.obj)


# Find xml libraries
find_library(XML_LIB xml2 $ENV{LIBXML_LIB_PATH})

# Find rt libraries
find_library(RT_LIB rt /usr/lib)

# Find bz2 libraries
find_library(BZ2_LIB bz2 $ENV{BZ2_LIB_PATH})

# Find the EVIO libraries
find_library(EVIO_LIB evio $ENV{EVIO_LIB_PATH})
find_library(EVIOXX_LIB evioxx $ENV{EVIO_LIB_PATH})

add_library(Processor Processor.cxx)
target_link_libraries(Processor
    ${TRACKER_EVENT_LIB}
)

add_library(Process Process.cxx) 
target_link_libraries(Process
    Processor
    ${DATA_READ_LIB}
    ${XMLVARIABLES_LIB}
    ${BZ2_LIB}
    ${XML_LIB}
    ${DATA_LIB}
    ${TRACKER_SAMPLE_LIB}
    ${RT_LIB}
    ${ROOT_LIBRARIES}
)

add_library(ProcessorFactory ProcessorFactory.cxx)

add_library(ConfigurePython ConfigurePython.cxx)
target_link_libraries(ConfigurePython 
    ${PYTHON_LIBRARIES}
    Process
    ProcessorFactory
)

add_library(BaselineProcessor SHARED 
    BaselineProcessor.cxx
)
set_property(TARGET BaselineProcessor PROPERTY POSITION_INDEPENDENT_CODE ON) 
target_link_libraries(BaselineProcessor
    Processor
    SamplesPlot
    QAUtils
    ${TRACKER_EVENT_LIB}
    ${DATA_LIB}
    ${BZ2_LIB}
)

add_executable(svt_qa svt_qa.cxx)
target_link_libraries(svt_qa 
    ConfigurePython
    Process
)
