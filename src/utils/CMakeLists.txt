
add_definitions("-std=c++11")

# Create the Apv25Utils library
add_library(Apv25Utils Apv25Utils.cxx)
target_link_libraries(Apv25Utils ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB})

add_library(QAUtils QAUtils.cxx)
