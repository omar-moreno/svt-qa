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

//------------//
//   SVT QA   //
//------------//
#include "Processor.h"
#include "QAUtils.h"
#include "SamplesPlot.h"
//#include "TupleMaker.h"

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"

class BaselineProcessor : public Processor { 

    public: 

        /** 
         * Class constructor.
         * @param name Name for this instance of the class.
         * @param process The Process class associated with Processor, provided by the framework.
         */
        BaselineProcessor(const std::string& name, Process& process); 
        
        /** Destructor */
        ~BaselineProcessor(); 

        /** Method called once before any events are processed. */
        void initialize();

        /** Method defining how an event should be processed. */
        void process(TrackerEvent* event);

        /** Method called once after all events have been processed. */
        void finalize();
       
        /** Method that prints this object. */ 
        //std::string toString();

    private: 

        /** Print information about a set of samples. */
        void printSamples(TrackerSample* samples); 


        void getSimpleCalibrations(TH1D* baseline_histogram, double &mean_baseline, 
                                             double &mean_baseline_error, double &noise); 

        void getCalibrations(TH1D* baseline_histogram, double &mean_baseline, double &mean_baseline_error, double &noise); 

        /** Object that encapsulates channel sample data. */        
        TrackerSample* samples_{new TrackerSample{}};

        SamplesPlot* plots; 
        
        TF1* gaussian{new TF1("gaussian", "gaus")};  


        /** Utility used to create ROOT ntuples. */
        //TupleMaker* _tuple{new TupleMaker("baseline.root", "results")}; 
       
        /** Mapping between channel number to readout order number. */
        int channel_map[128];

};

#endif // __BASELINE_ANALYIS_H__

