/**
 *  @file   SimpleBaselineAnalysis.h
 *  @brief  Analysis class used to extract baseline and noise values for all
 *          SVT channels.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   February 19, 2015
 *
 */

#ifndef __SIMPLE_BASELINE_ANALYSIS_H__
#define __SIMPLE_BASELINE_ANALYSIS_H__


//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <vector>
#include <iostream>

//--------------//
//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <QAUtils.h>
#include <PlottingUtils.h>
#include <ThresholdWriter.h>

//------------//
//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TH2F.h>
#include <TH2S.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TF1.h>
#include <PairHash.h>

class SimpleBaselineAnalysis : public QAAnalysis { 

    public: 

        /**
         *  Default Constructor
         */
        SimpleBaselineAnalysis();

        /**
         *  Constructor used when data from only a single FEB is going to be 
         *  processed.
         *
         *  @param feb_id : FEB ID
         */
        SimpleBaselineAnalysis(int feb_id);

        /**
         *  Constructor used when data from only a single hybrid connected to
         *  the specified FEB is going to be processed.
         *
         *  @param feb_id - FEB ID
         *  @param hybrid_id - Hybrid ID
         */    
        SimpleBaselineAnalysis(int feb_id, int hybrid_id);  

        /**
         *  Destructor
         */
        ~SimpleBaselineAnalysis(); 

        /**
         */
        void initialize();

        /**
         *
         */
        void readoutOrder(bool readout_order);  

        /**
         */
        void processEvent(TriggerSample*);

        /**
         */
        void finalize();
        
        /**
         *  Set the number of RMS noise above baseline that the threshold 
         *  should be set to i.e. the multiplicative factor N in
         *  the equation threshold = baseline + N*noise. This method is 
         *  just a wrapper to the setThreshold method of the Threshold
         *  writer.
         *
         *  @param threshold : Number of RMS noise above baseline
         */
        void setThreshold(const double threshold);

        /**
         */
        std::string toString();

    private: 
        
        void addBaselineHistogram(int feb_id, int hybrid_id); 

        /**
         */
        void processBaselinePlot(int, int, TH2S*);


        /**
         * 
         */
        void getCalibrations(TH1D* baseline_histogram, 
                             double &mean, 
                             double &mean_error, 
                             double &noise);

        /**
         *
         */
        void getSimpleCalibrations(TH1D* baseline_histogram, 
                                   double &mean, 
                                   double &mean_error,
                                   double &noise);   
        

        // ROOT output_file
        TFile* output_file; 

        // Gaussian distribution
        TF1* gaussian; 

        // XML threshold writer for SVT conditions
        ThresholdWriter* writer;

        // Map of FEB ID to a vector containing plots for each of the hybrids
        std::unordered_map <int, std::vector <TH2S*>> baseline_map; 

        // The name of the class
        std::string class_name;
        
        int channel_map[128];
        int feb_id; 
        int hybrid_id;

        bool readout_order;
};

#endif // __SIMPLE_BASELINE_ANALYSIS_H__
