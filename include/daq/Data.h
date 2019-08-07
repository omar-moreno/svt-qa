//-----------------------------------------------------------------------------
// File          : Data.h
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
#ifndef __DATA_H__
#define __DATA_H__

#include <string>
#include <sys/types.h>
#include <bzlib.h>
using namespace std;

#ifdef __CINT__
#define uint unsigned int
#endif

//! Class to contain generic register data.
class Data {

      // Allocation
      uint alloc_;

   protected:

      // Data container
      uint *data_;

      // Size value
      uint size_;

      // Update frame state
      virtual void update();

   public:

      // Data types. 
      // Count is n*32bits for type = 0, byte count for all others
      enum DataType {
         RawData     = 0,
         XmlConfig   = 1,
         XmlStatus   = 2,
         XmlRunStart = 3,
         XmlRunStop  = 4,
         XmlRunTime  = 5
      };

      //! Constructor
      /*! 
       * \param data Data pointer
       * \param size Data size
      */
      //Data ( uint *data, uint size );

      //! Constructor
      Data ();

      //! Deconstructor
      virtual ~Data ();

      //! Read data from file descriptor
      /*! 
       * \param fd File descriptor
       * \param size Data size
      */
      bool read ( int fd, uint size );

      //! Copy data from buffer
      /*! 
       * \param data Data pointer
       * \param size Data size
      */
      virtual void copy ( uint *data, uint size );

      //! Get pointer to data buffer
      uint *data ( );

      //! Get data size
      uint size ( );

      virtual void clearSamples(); 

};
#endif
