/**
 * @file	TestRunDataReadEvio.h
 * @brief	Test run EVIO to Data event converter.
 * @author	Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date	November 13, 2014
 *
 */

#ifndef __TEST_RUN_DATA_READ_EVIO_H__
#define __TEST_RUN_DATA_READ_EVIO_H__

#include <AbstractDataReadEvio.h>

class TestRunDataReadEvio : public AbstractDataReadEvio { 

	public:

		/**
		 */
		TestRunDataReadEvio();
		
		/**
		 */
		~TestRunDataReadEvio();

	private: 

		/**
		 */
		void processDataBank(Data*, uint16_t, std::vector<uint32_t>*);

}; // TestRunDataReadEvio

#endif // __TEST_RUN_DATA_READ_EVIO_H__


