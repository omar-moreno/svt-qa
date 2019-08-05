

# Find the required SVT DAQ files
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".o")

# Find the EVIO libraries
find_library(EVIO_LIB evio $ENV{EVIO_LIB_PATH})
find_library(EVIOXX_LIB evioxx $ENV{EVIO_LIB_PATH})


add_library(Processor Processor.cxx)

add_library(Process Process.cxx) 
target_link_libraries(Process
    Data
    AbstractDataReadEvio
    Phys2019DataReadEvio
    Phys2019SvtEvent
    ${ROOT_LIBRARIES}
    ${EVIO_LIB}
    ${EVIOXX_LIB}
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
    TupleBuilder
    QAUtils
)

add_library(ThresholdProcessor SHARED 
    ThresholdProcessor.cxx
)

set_property(TARGET ThresholdProcessor PROPERTY POSITION_INDEPENDENT_CODE ON) 
target_link_libraries(ThresholdProcessor
    Processor
    TupleBuilder
    QAUtils
)


add_executable(svt-qa svt_qa.cxx)
target_link_libraries(svt-qa 
    Process
    ConfigurePython
)
