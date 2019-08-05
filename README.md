svt-qa
======

Quality assurance and analysis framework for the Heavy Photon Search Silicon
Vertex Tracker

## Prerequisites

You will need the following build tools available in your environment before 
beginning the installation.

### Linux

The software has been built and tested on RHEL6 and OpenSuse 42.x.  

### CMake

You should have at least CMake 3.0 installed on your machine, and preferably a
current version from the [CMake website](https://cmake.org).

### GCC

You will need a version of GCC that supports the C++-11 standard i.e. a GCC 
version greater than 4.8. 


## External Packages

You will first need to install or have available at your site a number of
external dependencies before building the actual framework.

## EVIO

## ROOT

HPS is standardizing on ROOT 6, and no support for ROOT 5 is planned.

ROOT has many installation options and optional dependencies, and 
the [building ROOT documentation](https://root.cern.ch/building-root) covers 
this in full detail.

These commands should install a current version of ROOT locally:

``` bash
wget https://root.cern.ch/download/root_v6.06.08.source.tar.gz
tar -zxvf root_v6.06.08.source.tar.gz
mkdir root-6.06.08-build
cd root-6.06.08-build
cmake -Dgdml=ON ../root-6.06.08
make 
export ROOTDIR=$PWD
```

Depending on what extra tools you want to use in ROOT, you should supply your
own extra CMake arguments to enable them.

### Calculating Thresholds

After taking a baseline run, the following command can be executed in a terminal to generate a file containing the thresholds

    svt_qa -i <EVIO file> -e -t <threshold level>

The '-e' flag is indicating that the input file format is EVIO.  The threshold level is in units of RMS noise.  Typically, this will be a value between 1 and 4. 


