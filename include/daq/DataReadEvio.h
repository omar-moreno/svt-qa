/**
 *
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date:   October 16, 2014
 *
 */

#ifndef __DATA_READ_EVIO_H__
#define __DATA_READ_EVIO_H__

#include <DataRead.h>

#include <evioFileChannel.hxx>
#include <evioException.hxx>

using namespace evio; 

class DataReadEvio : public DataRead { 

	public:

		DataReadEvio();
		~DataReadEvio();

		bool next(Data*);
		bool open(string, bool);
		void close(); 

	private: 

		evioFileChannel* file_channel;  

}; // DataReadEvio

#endif // __DATA_READ_EVIO_H__
