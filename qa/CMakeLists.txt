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

# Setup the Rogue CMake target
setup_rogue()

add_executable(svt-qa ${PROJECT_SOURCE_DIR}/src/qa/svt_qa.cxx)

target_link_libraries(svt-qa 
		      PRIVATE Boost::program_options processing::processing) 

install(TARGETS svt-qa DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

set_target_properties(
  svt-qa
  PROPERTIES CXX_STANDARD 17
             CXX_STANDARD_REQUIRED YES
             CXX_EXTENSIONS NO)
