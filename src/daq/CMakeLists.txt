
# Find the EVIO libraries
find_library(EVIO_LIB evio $ENV{EVIO_LIB_PATH})
find_library(EVIOXX_LIB evioxx $ENV{EVIO_LIB_PAHT})

# Find the expat libraries
find_library(EXPAT_LIB expat $ENV{EXPAT_LIB_PATH})

add_library(Data Data.cxx)

add_library(TrackerSample TrackerSample.cxx)

add_library(Phys2019SvtEvent Phys2019SvtEvent.cxx)
target_link_libraries(Phys2019SvtEvent
    Data
    TrackerSample
)

add_library(AbstractDataReadEvio AbstractDataReadEvio.cxx)
target_link_libraries(AbstractDataReadEvio
    Data
    ${EXPAT_LIB}
    ${EVIOXX_LIB}
    ${EVIO_LIB}
)

add_library(Phys2019DataReadEvio Phys2019DataReadEvio.cxx)
target_link_libraries(Phys2019DataReadEvio AbstractDataReadEvio)

