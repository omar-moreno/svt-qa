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

#include <stdint.h>

#include <DataRead.h>
#include <TestRunSvtEvent.h>

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

		//TestRunSvtEvent* processDataBank(uint16_t, std::vector<uint32_t>*);
		void processDataBank(Data*, uint16_t, std::vector<uint32_t>*);

		evioDOMTree* event;
		evioFileChannel* file_channel;
		evioDOMNodeListP fpga_banks;
		evioDOMNodeList::iterator fpga_iterator;

		std::list<TestRunSvtEvent*> data_list;

		int event_n;	

}; // DataReadEvio

#endif // __DATA_READ_EVIO_H__
