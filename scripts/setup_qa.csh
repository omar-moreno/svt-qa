
# Installation directory of all packages
setenv SVTQA_HOME /home/phansson/svt-qa
setenv PACKAGES_HOME /home/omoreno/packages 

#---------------#
#--- SVT DAQ ---#
#---------------#
setenv DAQ_HOME ${PACKAGES_HOME}/daqV2
#setenv DAQ_HOME ${CLAS}/slac_svt/svtdaq/daq
if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${DAQ_HOME}/.obj
else 
	setenv LD_LIBRARY_PATH ${DAQ_HOME}/.obj
endif 

#------------#
#--- EVIO ---#
#------------#
setenv EVIO_LIB_PATH ${PACKAGES_HOME}/evio-4.3/Linux-x86_64/lib
setenv EVIO_INCLUDE_PATH ${PACKAGES_HOME}/evio-4.3/Linux-x86_64/include

if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${EVIO_LIB_PATH}
else 
	setenv LD_LIBRARY_PATH ${EVIO_LIB_PATH}
endif 

#-----------#
#--- GCC ---#
#-----------#
#setenv CC /u/apps/gcc/4.8.2/bin/gcc
#setenv CXX /u/apps/gcc/4.8.2/bin/g++
setenv CC /u/apps/gcc/4.9.2/bin/gcc
setenv CXX /u/apps/gcc/4.9.2/bin/g++

if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/u/apps/gcc/4.8.2/lib64
else 
	setenv LD_LIBRARY_PATH /u/apps/gcc/4.8.2/lib64
endif 

#-----------#
#--- bz2 ---#
#-----------#
setenv BZ2_LIB_PATH ${PACKAGES_HOME}/bzip/lib

if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${BZ2_LIB_PATH}
else 
	setenv LD_LIBRARY_PATH ${BZ2_LIB_PATH}
endif 

#---------------#
#--- libxml2 ---#
#---------------#
setenv LIBXML_LIB_PATH ${PACKAGES_HOME}/libxml2/lib 
setenv LIBXML_BIN_PATH ${PACKAGES_HOME}/libxml2/bin
setenv LIBXML_INCLUDE_PATH ${PACKAGES_HOME}/libxml2/include

if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${LIBXML_LIB_PATH}
else 
	setenv LD_LIBRARY_PATH ${LIBXML_LIB_PATH}
endif 

if ($?PATH) then
	setenv PATH ${PATH}:${LIBXML_BIN_PATH}
else 
	setenv PATH ${LIBXML_BIN_PATH}
endif


#---------------#
#--- yaml ---#
#---------------#
setenv YAML_CPP_INCLUDE_PATH ${PACKAGES_HOME}/yaml-cpp/include 
setenv YAML_CPP_LIB_PATH ${PACKAGES_HOME}/yaml-cpp/build

if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${YAML_CPP_LIB_PATH}
else 
	setenv LD_LIBRARY_PATH ${YAML_CPP_LIB_PATH}
endif 


#----------#
#--- qt ---#
#----------#
setenv QTDIR /home/rherbst/qt-4.6.3

if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${QTDIR}/lib
else 
	setenv LD_LIBRARY_PATH ${QTDIR}/lib
endif 

if ($?PATH) then
	setenv PATH ${PATH}:${QTDIR}/bin
else 
	setenv PATH ${QTDIR}/bin
endif

#-------------#
#--- expat ---#
#-------------#
setenv EXPAT_LIB_PATH ${PACKAGES_HOME}/expat/lib 
setenv EXPAT_INCLUDE_PATH ${PACKAGES_HOME}/expat/include
if ($?LD_LIBRARY_PATH) then
	setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${EXPAT_LIB_PATH}
else 
	setenv LD_LIBRARY_PATH ${EXPAT_LIB_PATH}
endif 

# Setup the path
if ($?PATH) then
	setenv PATH ${PATH}:${PACKAGES_HOME}/scons-2.3.3/bin:${PACKAGES_HOME}/Python-2.7.5/bin:${PACKAGES_HOME}/cmake/bin:${SVTQA_HOME}/build/bin
else
	setenv PATH ${PACKAGES_HOME}/scons-2.3.3/bin:${PACKAGES_HOME}/Python-2.7.5/bin:${PACKAGES_HOME}/cmake/bin:${SVTQA_HOME}/build/bin

endif

# Setup python path
if ($?PYTHONPATH) then
	setenv PYTHONPATH ${PYTHONPATH}:${ROOTSYS}/lib:${DAQ_HOME}/python/lib/python:${DAQ_HOME}/python/lib64/python
else
	setenv PYTHONPATH ${ROOTSYS}/lib:${DAQ_HOME}/python/lib/python:${DAQ_HOME}/python/lib64/python
endif

