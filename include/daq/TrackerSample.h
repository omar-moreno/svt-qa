//-----------------------------------------------------------------------------
// File          : TrackerSample.h
// Author        : Ryan Herbst  <rherbst@slac.stanford.edu>
// Created       : 08/26/2011
// Project       : Heavy Photon API
//-----------------------------------------------------------------------------
// Description :
// Sample Container
// Sample Data consists of the following: Z[xx:xx] = Zeros, O[xx:xx] = Ones
//    Sample[0] = O[0], Error[0], Hybrid[1:0], Drop[0], ApvChip[2:0], Z[0], Channel[6:0], FpgaAddress[15:0]
//    Sample[1] = Z[1:0], Sample1[13:0]], Z[1:0], Sample0[13:0]
//    Sample[2] = Z[1:0], Sample3[13:0]], Z[1:0], Sample2[13:0]
//    Sample[3] = Z[1:0], Sample5[13:0]], Z[1:0], Sample4[13:0]
//-----------------------------------------------------------------------------
// Copyright (c) 2011 by SLAC. All rights reserved.
// Proprietary and confidential to SLAC.
//-----------------------------------------------------------------------------
// Modification history :
// 08/26/2011: created
//-----------------------------------------------------------------------------
#ifndef __TRACKER_SAMPLE_H__
#define __TRACKER_SAMPLE_H__
#include <sys/types.h>
using namespace std;

//! Tracker Event Container Class
class TrackerSample {

      // Local data
      uint ldata_[4];

      // Data pointer
      uint *data_;

   public:

      //! Constructor for static pointer
      TrackerSample ();

      //! Constructor with copy
      TrackerSample ( uint *data );

      //! Set data pointer.
      /*!
       * \param data Data pointer.
      */
      void setData ( uint *data );

      //! Get hybrid index.
      /*!
       * Returns hybrid index.
      */
      uint hybrid ( );

      //! Get error flag
      /*!
       * Returns apv error flag
      */
      bool error ( );

      //! Get threshold drop flag
      /*!
       * Returns threshold drop flag
      */
      bool drop ( );

      //! Get apv index.
      /*!
       * Returns apv index.
      */
      uint apv ( );

      //! Get channel index.
      /*!
       * Returns channel index.
      */
      uint channel ( );

      //! Get FpgaAddress value from header.
      /*!
       * Returns fpgaAddress
      */
      uint fpgaAddress ( );

      //! Get adc value at index.
      /*!
       * Returns adc value value.
       * \param index sub-sample index, 0-5.
      */
      uint value ( uint index );

};

#endif
