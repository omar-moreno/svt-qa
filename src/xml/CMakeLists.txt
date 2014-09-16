
# Find xml libraries
find_library(XML_LIB xml2 /usr/lib)

add_library(SvtXmlWriter SvtXmlWriter.cxx)
target_link_libraries(SvtXmlWriter ${XML_LIB})

add_library(CalibrationWriter CalibrationWriter.cxx)
target_link_libraries(CalibrationWriter SvtXmlWriter)
