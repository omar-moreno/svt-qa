svt-qa
======

Quality assurance framework for the Heavy Photon Search Silicon Vertex Tracker.

## Prerequisites

You will need the following build tools available in your environment before 
beginning the installation.

### CMake

The minimum version of CMake required is 3.12 and preferably a
current version from the [CMake website](https://cmake.org).

### GCC

You will need a version of GCC that supports the C++17 standard. A GCC 
version greater than 6.3 should be enough.


## External Packages

## Rogue

Processing of the binary data format generated by Rogue requires building within a specific Anaconda environment.  
The procedure for building the Rogue environment can be found at:

## EVIO

### Calculating Thresholds

After taking a baseline run, the following command can be executed in a terminal to generate a file containing the thresholds

    svt_qa -i <EVIO file> -e -t <threshold level>

The '-e' flag is indicating that the input file format is EVIO.  The threshold level is in units of RMS noise.  Typically, this will be a value between 1 and 4. 


