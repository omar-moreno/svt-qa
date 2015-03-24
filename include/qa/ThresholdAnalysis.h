
#ifndef __THRESHOLD_ANALYSIS_H__
#define __THRESHOLD_ANALYSIS_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <iostream>
#include <unordered_map>
#

//--------------//
//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <ThresholdReader.h>
#include <PlottingUtils.h>
#include <QAUtils.h>

//------------//
//--- ROOT ---//
//------------//
#include <TH1S.h>
#include <TH2S.h>
#include <TFile.h>

class ThresholdAnalysis : public QAAnalysis { 

    public:

        /**
         *  Default Constructor
         */
        ThresholdAnalysis();
        /**
         *  Constructor used when data from only a single FEB is going to be 
         *  processed.
         *
         *  @param feb_id : FEB ID
         */
        ThresholdAnalysis(int feb_id);


        /**
         *  Constructor used when data from only a single hybrid connected to
         *  the specified FEB is going to be processed.
         *
         *  @param feb_id - FEB ID
         *  @param hybrid_id - Hybrid ID
         */    
        ThresholdAnalysis(int feb_id, int hybrid_id);

        /**
         *  Destructor
         */
        ~ThresholdAnalysis();

        /**
         *
         */
        void initialize();

        /**
         *
         */
        void processEvent(TriggerEvent* event);

        /**
         *
         */
        void processSamples(TriggerSample* samples);

        void setThresholdFile(std::string threshold_file) { this->threshold_file = threshold_file; }; 

        /**
         *
         */
        void finalize();

        /**
         *
         */
        std::string toString();

    private: 

        bool passThresholdCut(TriggerSample* samples, int feb, int hybrid, int apv, int channel);

        void clearHitCounts(); 
        void addHistogram(int feb_id, int hybrid_id);

        ThresholdReader* reader; 

        TH1S* total_raw_hit_counts; 
        
        TriggerSample* trigger_samples; 

        // ROOT output_file
        TFile* output_file; 
        
        // Map of FEB ID to a vector containing plots for each of the hybrids
        std::unordered_map <int, std::vector <TH1S*>> hit_counts_map;
        std::unordered_map <int, std::vector<TH2S*>> hit_counts_v_channel_map;
        std::unordered_map <int, std::vector<int>> hit_counts; 

        std::string class_name; 
        std::string threshold_file; 

        int feb_id; 
        int hybrid_id;
        int hit_count; 
        int current_event; 
        int samples_above;

}; // ThresholdAnalysis

#endif // __THRESHOLD_ANALYSIS_H__ 
