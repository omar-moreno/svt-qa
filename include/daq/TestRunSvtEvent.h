/**
 *	@file		TestRunSvtEvent.h
 *	@brief		HPS test run event container.
 *	@author		Omar Moreno <omoreno1@ucsc.edu>
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date		November 12, 2014
 *
 *	HPS Test run event container. Event Data consists of the following:
 *	  Z[xx:xx] = Zeros
 *    Frame Size = 1 x 32-bits (32-bit dwords)
 *    Header = 8 x 32-bits
 *       Header[0] = T[0], Z[14:0], FpgaAddress[15:0] - T = 1 For TI FPGA
 *       Header[1] = Sequence[31:0]
 *
 *  The rest of the event header depends on the T flag, For T = 0:
 *
 *       Header[2] = TempB[15:0], TempA[15:0] -- Hybrid 0
 *       Header[3] = TempD[15:0], TempC[15:0] -- Hybrid 0
 *       Header[4] = TempF[15:0], TempE[15:0] -- Hybrid 1
 *       Header[5] = TempH[15:0], TempG[15:0] -- Hybrid 1
 *       Header[6] = TempJ[15:0], TempI[15:0] -- Hybrid 2
 *       Header[7] = TempL[15:0], TempK[15:0] -- Hybrid 2
 *
 *       Samples... (See TrackerSample.h)
 *
 *    For T = 1:
 *
 *       Header[2] = TBD, Waiting for clarification from JLAB
 *       Header[3] = TBD, Waiting for clarification from JLAB
 *       Header[4] = TBD, Waiting for clarification from JLAB
 *       Header[5] = TBD, Waiting for clarification from JLAB
 *       Header[6] = TBD, Waiting for clarification from JLAB
 *       Header[7] = TBD, Waiting for clarification from JLAB
 *
 *    Tail = 1 x 32-bits
 *       
 *       Tail[1] = Z[31:0]
 */

#ifndef _TEST_RUN_SVT_EVENT_H_
#define _TEST_RUN_SVT_EVENT_H_


#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

//-----------//
//--- DAQ ---//
//-----------//
#include <Data.h>
#include <TestRunSvtSample.h>

class TestRunSvtEvent : public Data {

	public:

		//! Constructor
		TestRunSvtEvent();

		//! Deconstructor
		~TestRunSvtEvent();

		//! Is frame TI frame?
		bool isTiFrame();

		//! Get FpgaAddress value from header.
		/*!
		 * Returns fpgaAddress
		 */
		uint fpgaAddress();

		//! Get sequence count from header.
		/*!
		 * Returns sequence count
		 */
		uint sequence();

		//! Get trigger block from header.
		/*!
		 * Returns trigger block
		 */
		uint * tiData();

		//! Get temperature values from header.
		/*!
		 * Returns temperature value.
		 * \param index temperature index, 0-11.
		 */
		double temperature(uint);

		//! Get sample count
		/*!
		 * Returns sample count
		 */
		uint count();

		//! Get the sample at index
		/*!
		 * Returns the sample at index
		 */
		void sample(uint, TestRunSvtSample*);

	public:

		// Temperature Constants
		static const double coeffA_     = -1.4141963e1;
		static const double coeffB_     =  4.4307830e3;
		static const double coeffC_     = -3.4078983e4;
		static const double coeffD_     = -8.8941929e6;
		static const double beta_       = 3750;
		static const double constA_     = 0.03448533;
		static const double t25_        = 10000.0;
		static const double k0_         = 273.15;
		static const double vmax_       = 2.5;
		static const double vref_       = 2.5;
		static const double vrefNew_    = 2.048;
		static const double rdiv_       = 10000;
		static const double minTemp_    = -50;
		static const double maxTemp_    = 150;
		static const double incTemp_    = 0.01;
		static const uint   adcCnt_     = 4096;

		// Temperature lookup table
		double tempTable_[adcCnt_];
		double tempTableNew_[adcCnt_];

		// Frame Constants
		static const unsigned int headSize_   = 7;
		static const unsigned int tailSize_   = 1;
		static const unsigned int sampleSize_ = 4;

		// Update
		void update();

};  // TestRunSvtEvent

#endif // _TEST_RUN_SVT_EVENT_H_
