# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(rogue 
	VERSION 0.1      
	DESCRIPTION "Module containing classes that interface with Rogue" 
        LANGUAGES CXX)

# Setup the Rogue CMake target
setup_rogue()

# Build the reader library
setup_library(module rogue dependencies Rogue::Rogue)

setup_test(dependencies rogue::rogue)
