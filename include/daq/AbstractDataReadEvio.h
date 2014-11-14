/**
 * @file	AbstractDataReadEvio.h
 * @brief	Abstract class containing common methods used in converting
 *			EVIO to Data objects.
 * @author	Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date	November 13, 2014
 *
 */

#ifndef __ABSTRACT_DATA_READ_EVIO_H__
#define __ABSTRACT_DATA_READ_EVIO_H__

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

class AbstractDataReadEvio : public DataRead { 

	public: 

		/**
		 */
		AbstractDataReadEvio(int, int);
		
		/**
		 */
		virtual ~AbstractDataReadEvio();

		// TODO: Make these classes virtual in DataRead
		/**
		 */
		bool next(Data*);
		
		/**
		 */
		bool open(string, bool);
		
		/**
		 */
		void close();

		/**
		 */
		evioDOMTree* getPhysicsEvent();

	private:

		/**
		 */
		virtual void processDataBank(Data*, uint16_t, std::vector<uint32_t>*) = 0;

		evioDOMTree* event;
		evioFileChannel* file_channel;
		evioDOMNodeListP data_banks;
		evioDOMNodeList::iterator data_iterator;

		int event_n;	
		
		//-----------------------//
		//--- Event constants ---//
		//-----------------------//

		/**
		 */
		int physics_event_tag;

		/**
		 */
		int svt_bank_tag;		

}; // AbstractDataReadEvio

#endif // __ABSTRACT_DATA_READ_EVIO_H__
