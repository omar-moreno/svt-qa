/**
 *  @author: Omar Moreno <omoreno1@ucsc.edu>
 *  @section institution
 *           Santa Cruz Institute for Particle Physics
 *           University of California, Santa Cruz
 *  @date:   May 14, 2014
 *
 */

#ifndef __QA_ANALYSIS_H__
#define __QA_ANALYSIS_H__

//---------------//
//--- SVT DAQ ---//
//---------------//
#include "TriggerSample.h"
#include "TriggerEvent.h"

class QAAnalysis { 

    public: 

        /**
         *  Destructor
         */
        virtual ~QAAnalysis(){}; 

        /**
         *
         */
        virtual void initialize() = 0; 
        
        /**
         *
         */
        virtual void processEvent(TriggerEvent* event) = 0;
        
        /**
         *
         */
        virtual void processSamples(TriggerSample* samples) = 0;

        /**
         *
         */ 
        virtual void finalize() = 0; 
        
        /**
         *
         */
        virtual std::string toString() = 0; 

}; // QAAanalysis

#endif // __QA_ANALYSIS_H__

