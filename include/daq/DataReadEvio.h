/**
 * @file	DataReadEvio.h
 * @brief	EVIO to TestRunSvtEvent converter.
 * @author	Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date	October 16, 2014
 *
 */

#ifndef __DATA_READ_EVIO_H__
#define __DATA_READ_EVIO_H__

#include <stdint.h>
#include <vector>

//-----------//
//--- DAQ ---//
//-----------//
#include <DataRead.h>
#include <Data.h>

//------------//
//--- EVIO ---//
//------------//
#include <evioFileChannel.hxx>
#include <evioException.hxx>
#include <evioUtil.hxx>

using namespace evio; 

class DataReadEvio : public DataRead { 

	public:

		DataReadEvio();
		~DataReadEvio();

		bool next(Data*);
		bool open(string, bool);
		void close();

		evioDOMTree* getPhysicsEvent();

		// Event Constants
		// TODO: These constants should eventually be moved to an external 
		//		 header file.
		static const int PHYSICS_EVENT_TAG;
		static const int SVT_BANK_TAG;

	private: 

		void processDataBank(Data*, uint16_t, std::vector<uint32_t>*);

		evioDOMTree* event;
		evioFileChannel* file_channel;
		evioDOMNodeListP data_banks;
		evioDOMNodeList::iterator data_iterator;

		std::list<Data*> data_list;

		int event_n;	

}; // DataReadEvio

#endif // __DATA_READ_EVIO_H__
