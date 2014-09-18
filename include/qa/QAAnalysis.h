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

//--- SVT DAQ ---//
//---------------//
#include <TriggerSample.h>

class QAAnalysis { 

    public: 

        virtual ~QAAnalysis(){}; 

        virtual void initialize() = 0; 
        virtual void processEvent(TriggerSample* samples) = 0;
        virtual void finalize() = 0; 
		virtual std::string toString() = 0; 

}; // QAAanalysis

#endif // __QA_ANALYSIS_H__

