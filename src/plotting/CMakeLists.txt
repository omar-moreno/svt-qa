
find_library(ROOT_HIST_LIB Hist $ENV{ROOTSYS}/lib)

add_library(SamplesPlot SamplesPlot.cxx)
target_link_libraries(SamplesPlot ${ROOT_HIST_LIB})
target_link_libraries(SamplesPlot PlotUtils)
