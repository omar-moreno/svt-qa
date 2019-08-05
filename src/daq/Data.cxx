//-----------------------------------------------------------------------------
// File          : Data.cpp
// Author        : Ryan Herbst  <rherbst@slac.stanford.edu>
// Created       : 04/12/2011
// Project       : General Purpose
//-----------------------------------------------------------------------------
// Description :
// Generic data container
//-----------------------------------------------------------------------------
// Copyright (c) 2011 by SLAC. All rights reserved.
// Proprietary and confidential to SLAC.
//-----------------------------------------------------------------------------
// Modification history :
// 04/12/2011: created
//-----------------------------------------------------------------------------

#include "Data.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;


// Update frame state
void Data::update() { }

// Constructor

/*Data::Data ( uint *data, uint size ) {
   size_  = size;
   alloc_ = size;
   data_  = (uint *)malloc(alloc_ * sizeof(uint));
   memcpy(data_,data,size_*sizeof(uint));
   update();
}*/

// Constructor
Data::Data () {
   size_  = 0;
   alloc_ = 1;
   data_  = (uint *)malloc(sizeof(uint));
   update();
}

// Deconstructor
Data::~Data ( ) {
   free(data_);
}

// Read data from file descriptor
bool Data::read ( int fd, uint size ) {
   if ( size > alloc_ ) {
      free(data_);
      alloc_ = size;
      data_ = (uint *)malloc(alloc_ * sizeof(uint));
   }
   size_ = size;
   if ( ::read(fd, data_, size_*(sizeof(uint))) != (int)(size_ *sizeof(uint))) {
      size_ = 0;
      update();
      return(false);
   }
   update();
   return(true);
}

// Copy data from buffer
void Data::copy ( uint *data, uint size ) {
   if ( size > alloc_ ) {
      free(data_);
      alloc_ = size;
      data_ = (uint *)malloc(alloc_ * sizeof(uint));
   }
   size_ = size;

   // Copy data
   memcpy(data_,data,size_*sizeof(uint));
   update();
}

// Get pointer to data buffer
uint *Data::data ( ) {
   return(data_);
}

// Get data size
uint Data::size ( ) {
   return(size_);
}

void Data::clearSamples() { } 

