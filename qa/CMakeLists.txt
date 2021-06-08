# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(qa 
	VERSION 0.1      
	DESCRIPTION "Module containing quality assurance code." 
        LANGUAGES CXX)

# Search for Boost
find_package(
  Boost REQUIRED
  COMPONENTS program_options)

add_executable(svt-qa ${PROJECT_SOURCE_DIR}/src/qa/svt_qa.cxx)

target_link_libraries(svt-qa PRIVATE Boost::program_options)

install(TARGETS svt-qa DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
