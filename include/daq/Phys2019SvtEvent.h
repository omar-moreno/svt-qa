/**
 *
 */

#ifndef __PHYS2019_SVT_EVENT_H__
#define __PHYS2019_SVT_EVENT_H__

#include <vector>
#include <iostream>

#include "Data.h"
#include "TrackerSample.h"


class Phys2019SvtEvent : public Data { 

    public: 

        /** Default Constructor */
        Phys2019SvtEvent();

        /** Destructor */
        ~Phys2019SvtEvent();

        /** 
         * Copy data from buffer.  At the moment, only copy the multi-samples
         * from each of the RSSI event frames. The set of samples will be 
         * stored in a vector until they are ready for use. 
         */
        void copy( uint* data, uint size ); 

        /**
         * Get the total number of multi-samples in this event.
         */
        uint count(); 

        /**
         *
         */
        TrackerSample* sample(uint index); 

        void clearSamples(); 

    private: 

        TrackerSample sample_; 
            
        /**
         *
         */
        std::vector<std::vector<uint>> multisamples; 
};

#endif // __PHYS2019_SVT_EVENT_H__
