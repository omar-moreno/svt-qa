/**
 *	@file		TestRunSvtEvent.h
 *	@brief		HPS test run event container.
 *	@author		Omar Moreno <omoreno1@ucsc.edu>
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date		November 12, 2014
 */


#ifndef __TEST_RUN_SVT_EVENT_H__
#define __TEST_RUN_SVT_EVENT_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

//---------------//
//--- SVT DAQ ---//
//---------------//
#include <Data.h>
#include <TestRunSvtSample.h>

/**
 *  HPS Test run event container. Event Data consists of the following:
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
class TestRunSvtEvent : public Data {

	public:

        /**
         *  Default Constructor
         */
		TestRunSvtEvent();

        /**
         *  Deconstructor
         */
		~TestRunSvtEvent();

        /**
         *  Check whether the data corresponds to a TI frame
         */
		bool isTiFrame();

		/**
         *  Get the FPGA address value from the header
         *
         *  @return The FPGA address
         */
        uint fpgaAddress();

		/**
         *  Get the sequence count from the header.
         *
         *  @return The sequence count
         */
        uint sequence();
        
        /**
         *  Get the trigger block from the header.
         *
         *  @return The trigger block
         */
		uint * tiData();

	    /**
         *  Get the temperature at the given index from the header.
         *
         *  @param index - temperature index in the range 0-11
         *  @return temperature in C
         */
        double temperature(uint index);

		/**
         *  Get the total number of sample sets in the event
         *
         *  @return The sample set count
         */
        uint count();

		/**
         *  Get the sample set at the given index.
         *
         *  @param index - sample set index less than this->count()
         *  @param sample - The test run sample use to encapsulate the data
         */
        void sample(uint index, TestRunSvtSample* sample);

	public:

		// Temperature Constants
		static const double coeffA_;
		static const double coeffB_;
		static const double coeffC_;
		static const double coeffD_;
		static const double beta_;
		static const double constA_;
		static const double t25_;
		static const double k0_;
		static const double vmax_;
		static const double vref_;
		static const double vrefNew_;
		static const double rdiv_;
		static const double minTemp_;
		static const double maxTemp_;
		static const double incTemp_;
		static constexpr uint adcCnt_ = 4096;

		// Temperature lookup table
		double tempTable_[adcCnt_];
		double tempTableNew_[adcCnt_];

		// Frame Constants
		static const unsigned int headSize_;
		static const unsigned int tailSize_;
		static const unsigned int sampleSize_;

		// Update
		void update();

};  // TestRunSvtEvent

#endif // _TEST_RUN_SVT_EVENT_H_
