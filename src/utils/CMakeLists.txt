
# Create the Apv25Utils library
#add_library(Apv25Utils Apv25Utils.cxx)
#target_link_libraries(Apv25Utils ${ROOT_TREE_LIB} ${ROOT_CORE_LIB} ${ROOT_RIO_LIB} ${ROOT_PHYSICS_LIB})

add_library(TupleBuilder TupleBuilder.cxx)
target_link_libraries(TupleBuilder
    ${ROOT_LIBRARIES}
)

add_library(QAUtils QAUtils.cxx)


add_executable(csv2tuple Csv2Tuple)
target_link_libraries(csv2tuple 
    ${ROOT_LIBRARIES}
)
