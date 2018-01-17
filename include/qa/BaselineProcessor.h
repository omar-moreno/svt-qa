/**
 *  @file BaselineProcessor.h
 *  @brief Processor that create a tuple out of an SVT baseline run. 
 *  @author Omar Moreno, SLAC National Accelerator Laboratory
 *
 */

#ifndef __BASELINE_PROCESSOR_H__
#define __BASELINE_PROCESSOR_H__

//----------------//
//   C++ StdLib   //
//----------------//
#include <string>
#include <unordered_map>

//---------//
//   DAQ   //
//---------//
#include "TrackerSample.h"
#include "TrackerEvent.h"

//------------//
//   SVT QA   //
//------------//
#include "QAUtils.h"
#include "TupleMaker.h"

class BaselineProcessor { 

    public: 

        /** Constructor */
        BaselineProcessor(); 
        
        /** Destructor */
        ~BaselineProcessor(); 

        /** Method called once before any events are processed. */
        void initialize();

        /** Method defining how an event should be processed. */
        void process(TrackerEvent* event);

        /** Method called once after all events have been processed. */
        void finalize();
       
        /** Method that prints this object. */ 
        std::string toString();

    private: 

        /** Print information about a set of samples. */
        void printSamples(TrackerSample* samples); 

        /** Object that encapsulates channel sample data. */        
        TrackerSample* _samples{new TrackerSample{}};

        /** Utility used to create ROOT ntuples. */
        TupleMaker* _tuple{new TupleMaker("baseline.root", "results")}; 
       
        /** Event counter */
        double _event_counter{0};  

        /** Name of the class */
        std::string class_name;

        /** Mapping between channel number to readout order number. */
        int channel_map[128];

};

#endif // __BASELINE_ANALYIS_H__

