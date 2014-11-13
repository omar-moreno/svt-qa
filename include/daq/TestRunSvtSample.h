/**
 *	@file		TestRunSvtSample.h
 *	@brief		HPS test run SVT sample container.
 *	@author		Omar Moreno <omoreno1@ucsc.edu>
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date		November 12, 2014
 *
 *
 *	HPS test run SVT sample container.  Sample Data consists of the following:
 *	  Z[xx:xx] = Zeros, O[xx:xx] = Ones
 *
 *    Sample[0] = O[0], Error[0], Hybrid[1:0], Drop[0], ApvChip[2:0], Z[0], Channel[6:0], FpgaAddress[15:0]
 *    Sample[1] = Z[1:0], Sample1[13:0]], Z[1:0], Sample0[13:0]
 *    Sample[2] = Z[1:0], Sample3[13:0]], Z[1:0], Sample2[13:0]
 *    Sample[3] = Z[1:0], Sample5[13:0]], Z[1:0], Sample4[13:0]
 *
 */

#ifndef _TEST_RUN_SVT_SAMPLE_H_
#define _TEST_RUN_SVT_SAMPLE_H_

#include <sys/types.h>
#include <string>
#include <cstring>

class TestRunSvtSample {

   public:

      //! Constructor for static pointer
      TestRunSvtSample();

      //! Constructor with copy
      TestRunSvtSample(uint*);

      //! Set data pointer.
      /*!
       * \param data Data pointer.
      */
      void setData(uint*);

      //! Get hybrid index.
      /*!
       * Returns hybrid index.
      */
      uint hybrid();

      //! Get error flag
      /*!
       * Returns apv error flag
      */
      bool error();

      //! Get threshold drop flag
      /*!
       * Returns threshold drop flag
      */
      bool drop();

      //! Get apv index.
      /*!
       * Returns apv index.
      */
      uint apv();

      //! Get channel index.
      /*!
       * Returns channel index.
      */
      uint channel();

      //! Get FpgaAddress value from header.
      /*!
       * Returns fpgaAddress
      */
      uint fpgaAddress();

      //! Get adc value at index.
      /*!
       * Returns adc value value.
       * \param index sub-sample index, 0-5.
      */
      uint value(uint);

   private: 

      // Data pointer
      uint* data;

}; // TestRunSvtSample

#endif // _TEST_RUN_SVT_SAMPLE_H_
