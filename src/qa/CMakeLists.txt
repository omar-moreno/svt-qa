

add_definitions("-std=c++11")

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
find_library(ROO_FIT_LIB RooFit $ENV{ROOTSYS}/lib)
find_library(ROO_FIT_CORE_LIB RooFitCore $ENV{ROOTSYS}/lib)

# Find the required SVT DAQ files
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".o")
find_library(TRIGGER_SAMPLE_LIB TriggerSample.o $ENV{DAQ_HOME}/.obj)
find_library(TRIGGER_EVENT_LIB TriggerEvent.o $ENV{DAQ_HOME}/.obj)
find_library(TRACKER_EVENT_LIB TrackerEvent.o $ENV{DAQ_HOME}/.obj)
find_library(TRACKER_SAMPLE_LIB TrackerSample.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_READ_LIB DataRead.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_LIB Data.o $ENV{DAQ_HOME}/.obj)
find_library(XMLVARIABLES_LIB XmlVariables.o $ENV{DAQ_HOME}/.obj)


# Find xml libraries
find_library(XML_LIB xml2 /usr/lib)

# Find rt libraries
find_library(RT_LIB rt /usr/lib)

# Find bz2 libraries
find_library(BZ2_LIB bz2 $ENV{BZ2_LIB_PATH})

# Find the EVIO libraries
find_library(EVIO_LIB evio $ENV{EVIO_LIB_PATH})
find_library(EVIOXX_LIB evioxx $ENV{EVIO_LIB_PATH})

add_library(BaselineAnalysis BaselineAnalysis.cxx)
target_link_libraries(BaselineAnalysis 
    ${ROOT_RIO_LIB}
    ${ROOT_TREE_LIB}
    ${ROOT_CORE_LIB}
    ${ROOT_HIST_LIB}
    ${ROOT_GPAD_LIB}
    QAUtils
    CalibrationWriter
    SamplesPlot
    PairHash
)

add_library(SimpleBaselineAnalysis SimpleBaselineAnalysis.cxx)
target_link_libraries(SimpleBaselineAnalysis 
    ${ROOT_RIO_LIB}
    ${ROOT_TREE_LIB}
    ${ROOT_CORE_LIB}
    ${ROOT_HIST_LIB}
    ${ROOT_GPAD_LIB}
    QAUtils
    ThresholdWriter
    PairHash
)

add_library(OccupancyAnalysis OccupancyAnalysis.cxx)
target_link_libraries(OccupancyAnalysis
    ${ROOT_RIO_LIB}
    ${ROOT_HIST_LIB}
    PlottingUtils
)

#add_library(CalibrationAnalysis CalibrationAnalysis.cxx)
#target_link_libraries(CalibrationAnalysis
#    ${ROOT_CORE_LIB}
#    ${ROOT_GPAD_LIB}
#    ${ROOT_HIST_LIB}
#    QAUtils
#    CalGroupsPlot
#    PlottingUtils
#    Apv25Utils
#    PairHash
#)

add_executable(svt_qa svt_qa.cxx)
target_link_libraries(svt_qa 
    DataReadEvio
    ${TRACKER_EVENT_LIB}
    ${TRACKER_SAMPLE_LIB}
    ${TRIGGER_EVENT_LIB}
    ${TRIGGER_SAMPLE_LIB}
    ${XMLVARIABLES_LIB}
    ${BZ2_LIB}
    ${XML_LIB}
    ${RT_LIB}
    ${EVIOXX_LIB}
    ${EVIO_LIB}
    ${EXPAT_LIB}
    BaselineAnalysis
    SimpleBaselineAnalysis
    OccupancyAnalysis
    #    CalibrationAnalysis
)
