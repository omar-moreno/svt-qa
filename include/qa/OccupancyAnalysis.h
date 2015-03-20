/**
 *  @file   OccupancyAnalysis.h
 *  @brief  Analysis class used to study the SVT occupancies.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   March 16, 2014
 */

#ifndef __OCCUPANCY_ANALYSIS_H__
#define __OCCUPANCY_ANALYSIS_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <iostream>
#include <unordered_map>

//--------------//
//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <PlottingUtils.h>

//------------//
//--- ROOT ---//
//------------//
#include <TH1S.h>
#include <TFile.h>

class OccupancyAnalysis : public QAAnalysis { 

    public:
        /**
         *  Default Constructor
         */
        OccupancyAnalysis();

        /**
         *  Constructor used when data from only a single FEB is going to be 
         *  processed.
         *
         *  @param feb_id : FEB ID
         */
        OccupancyAnalysis(int feb_id);


        /**
         *  Constructor used when data from only a single hybrid connected to
         *  the specified FEB is going to be processed.
         *
         *  @param feb_id - FEB ID
         *  @param hybrid_id - Hybrid ID
         */    
        OccupancyAnalysis(int feb_id, int hybrid_id);

        /**
         *  Destructor
         */
        ~OccupancyAnalysis();

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

        /**
         *
         */
        void finalize();

        /**
         *
         */
        std::string toString();

    private:

        void addOccupancyHistogram(int feb_id, int hybrid_id);

        void clearHitCounts(); 

        TH1S* total_raw_hit_counts; 
        
        TriggerSample* trigger_samples; 

        // ROOT output_file
        TFile* output_file; 
        
        // Map of FEB ID to a vector containing plots for each of the hybrids
        std::unordered_map <int, std::vector <TH1S*>> hit_counts_map;
        std::unordered_map <int, std::vector<int>> hit_counts;  
        
        std::string class_name; 

        int feb_id; 
        int hybrid_id;
        int hit_count; 
        int current_event; 

};  // OccupancyAnalysis

#endif  // __OCCUPANCY_ANALYSIS_H__

