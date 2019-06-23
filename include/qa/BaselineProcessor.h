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
#include <memory>
#include <vector>
#include <fstream>

//------------//
//   SVT QA   //
//------------//
#include "Processor.h"
//#include "QAUtils.h"
//#include "TupleMaker.h"

#include "TF1.h"

// Forward declarations
class Process; 
class TrackerSample;
class TTree;
class TupleBuilder; 
class TH1; 
class TH2S; 
class TH1D; 

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
        void initialize(TTree* tree);

        /** Method defining how an event should be processed. */
        //void process(TrackerEvent* event);
        void process(Phys2019SvtEvent* event);

        /** Method called once after all events have been processed. */
        void finalize();
       
    private: 

        void addBaselineHistogram(int feb_id, int hybrid_id); 

        /** Print information about a set of samples. */
        void printSamples(TrackerSample* samples); 


        void getSimpleCalibrations(TH1D* baseline_histogram, double &mean_baseline, 
                                             double &mean_baseline_error, double &noise); 

        void getCalibrations(TH1D* baseline_histogram, double &mean_baseline, double &mean_baseline_error, double &noise); 

        void processBaselinePlot(int feb, int hybrid, TH2S* baseline_plot); 

        /** Event counter */
        int event_count_{0}; 

        /** Ntuple builder. */
        std::unique_ptr<TupleBuilder> tuple_; 

        /** Map of baseline plots to FEB/Hybrid. */
        std::unordered_map <int, std::vector <TH2S*>> baseline_map;        

        /** Object that encapsulates channel sample data. */        
        //TrackerSample* samples_{new TrackerSample{}};

        /** Container for baseline plots. */
        ///std::vector<TH2S*> plots; 
       
        /** Container for baseline plots in readout order. */
        //std::vector<TH2S*> rplots; 

        TF1* gaussian{new TF1("gaussian", "gaus")};  

        /** Mapping between channel number to readout order number. */
        int channel_map[128];

        /** Output file for thresholds. */
        std::ofstream tfile; 

}; // BaselineProcessor

#endif // __BASELINE_PRCESSOR_H__

