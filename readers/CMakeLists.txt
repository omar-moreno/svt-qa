# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(processing 
	VERSION 1.0.0          
	DESCRIPTION "Framework used to define processing pipelines and modules."
        LANGUAGES CXX)

# Setup the Rogue CMake target
setup_rogue()

# Build the reader library
setup_library(module readers dependencies Rogue::Rogue)
