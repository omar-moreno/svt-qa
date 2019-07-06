
# Find the EVIO libraries
find_library(EVIO_LIB evio $ENV{EVIO_LIB_PATH})
find_library(EVIOXX_LIB evioxx $ENV{EVIO_LIB_PAHT})

# Find the SVT DAQ libraries
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".o")
find_library(DATAREAD_LIB DataRead.o $ENV{DAQ_HOME}/.obj)
find_library(DATA_LIB Data.o $ENV{DAQ_HOME}/.obj)
find_library(TRACKER_SAMPLE_LIB TrackerSample.o $ENV{DAQ_HOME}/.obj)

# Find the expat libraries
find_library(EXPAT_LIB expat $ENV{EXPAT_LIB_PATH})

add_library(TestRunSvtSample TestRunSvtSample.cxx)

add_library(TestRunSvtEvent TestRunSvtEvent.cxx)
target_link_libraries(TestRunSvtEvent ${DATA_LIB})

add_library(Phys2019SvtEvent Phys2019SvtEvent.cxx)
target_link_libraries(TestRunSvtEvent 
    ${DATA_LIB}
    ${TRACKER_SAMPLE_LIB}
)

add_library(AbstractDataReadEvio AbstractDataReadEvio.cxx)
target_link_libraries(AbstractDataReadEvio
   	${DATAREAD_LIB}
	${DATA_LIB}
	${EXPAT_LIB}
	${EVIOXX_LIB}
	${EVIO_LIB}
)

add_library(DataReadEvio DataReadEvio.cxx)
target_link_libraries(DataReadEvio AbstractDataReadEvio)

add_library(Phys2019DataReadEvio Phys2019DataReadEvio.cxx)
target_link_libraries(Phys2019DataReadEvio AbstractDataReadEvio)

#add_library(TestRunDataReadEvio TestRunDataReadEvio.cxx)
#target_link_libraries(TestRunDataReadEvio AbstractDataReadEvio)
