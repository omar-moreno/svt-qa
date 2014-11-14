/**
 * @file	DataReadEvio.h
 * @brief	EVIO to Data event converter.
 * @author	Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date	October 16, 2014
 *
 */

#ifndef __DATA_READ_EVIO_H__
#define __DATA_READ_EVIO_H__

#include <AbstractDataReadEvio.h>

class DataReadEvio : public AbstractDataReadEvio { 

	public:

		/**
		 */
		DataReadEvio();
		
		/**
		 */
		~DataReadEvio();

	private: 

		/**
		 */
		void processDataBank(Data*, uint16_t, std::vector<uint32_t>*);

}; // DataReadEvio

#endif // __DATA_READ_EVIO_H__
