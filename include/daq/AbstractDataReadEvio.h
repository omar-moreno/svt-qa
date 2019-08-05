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

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <stdint.h>
#include <vector>

//-----------//
//--- DAQ ---//
//-----------//
#include <Data.h>

//------------//
//--- EVIO ---//
//------------//
#include <evioFileChannel.hxx>
#include <evioException.hxx>
#include <evioUtil.hxx>

using namespace evio; 

class AbstractDataReadEvio { 

	public: 

		/**
		 *  Default Constructor
         */
		AbstractDataReadEvio();
		
		/**
		 *  Destructor
         */
		virtual ~AbstractDataReadEvio();

		// TODO: Make these classes virtual in DataRead
		/**
         *  Read the next Data object in the event.
         *
         *  @param data : 
         *  @return
		 */
		bool next(Data* data);
		
		/**
         *  Open an EVIO file for reading.
         *
         *  @param evio_file_name : The EVIO file name to open
         *  @param compressed : True if the file is compressed, false otherwise
         *  @return True if the file was successfully opened, false otherwise
		 */
		bool open(std::string evio_file_name);
		
		/**
         *  Close the EVIO file channel.
		 */
		void close();

		/**
         *  
		 */
		evioDOMTree* getPhysicsEvent();

        /**
         *
         */
        evioDOMNodeListP getDataBanks(evioDOMNode* roc_banks); 

	private:

		/**
		 */
		virtual void processDataBank(Data*, uint16_t, std::vector<uint32_t>*) = 0;


		/**
		 *
		 */
		virtual int getMaxRocBankTag() = 0; 

		/**
		 *
		 */
		virtual int getMinRocBankTag() = 0;

		/**
		 *
		 */
		virtual int getPhysicsBankTag() = 0;

		evioDOMTree* event;
		evioFileChannel* file_channel;
		evioDOMNodeListP data_banks;
		evioDOMNodeListP roc_banks;
		evioDOMNodeList::iterator data_iterator;

		int event_n;	
		
}; // AbstractDataReadEvio

#endif // __ABSTRACT_DATA_READ_EVIO_H__
