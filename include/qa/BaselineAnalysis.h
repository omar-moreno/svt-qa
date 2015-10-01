/**
 *  @file   BaselineAnalysis.h
 *  @brief  Analysis class used to extract baseline and noise values for all
 *          SVT channels.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   May 5, 2014
 *
 */

#ifndef __BASELINE_ANALYSIS_H__
#define __BASELINE_ANALYSIS_H__

//----------------//
//   C++ StdLib   //
//----------------//
#include <string>
#include <unordered_map>

//------------//
//   SVT QA   //
//------------//
#include <QAAnalysis.h>
#include <QAUtils.h>
#include <CalibrationWriter.h>
#include <PlottingUtils.h>
#include <SamplesPlot.h>

//----------//
//   ROOT   //
//----------//
#include <TH2S.h>
#include <TH1D.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <PairHash.h>
#include <TFile.h>
#include <TF1.h>

class BaselineAnalysis : public QAAnalysis { 

    public: 

        /** Constructor */
        BaselineAnalysis(); 
        
        /**
         *  Parameterized constructor used when data from hybrids associated 
         *  with a single FEB is going to be processed.
         *
         *  @param feb_id : FEB ID
         */
        BaselineAnalysis(int feb_id);

        /**
         *  Parameterized constructor used when data from only a single hybrid
         *  is going to be processed.
         *
         *  @param feb_id : FEB ID
         *  @param hybrid_id : Hybrid ID
         */    
        BaselineAnalysis(int feb_id, int hybrid_id);    
        
        /**
         *  Destructor
         */
        ~BaselineAnalysis(); 

        /**
         *  Method called at the beginning of an analysis class.
         */
        void initialize();

        /**
         *  Method used to process events.
         */
        void processEvent(TriggerEvent* event);

        /**
         *
         */
        void processSamples(TriggerSample* samples);
    
        /**
         */
        void processBaselinePlot(int, int, SamplesPlot*);

        /**
         *  Setter used to specify that all plots should be made with the channel
         *  numbers in readout order instead of physical order.
         *
         *  @param readout_order : True if readout_order is desired, false 
         *                         otherwise.
         */
        void readoutOrder(bool readout_order);  

        /**
         *  Method after all events have been processed.  
         */
        void finalize();
        
        /**
         */
        std::string toString();


    private: 

        void addBaselineHistogram(int feb_id, int hybrid_id); 

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
        
        TriggerSample* trigger_samples;


        // ROOT output_file
        TFile* output_file;

        // Gaussian distribution
        TF1* gaussian; 

        // XML writer for SVT conditions
        CalibrationWriter* writer; 

        std::unordered_map <int, std::vector <SamplesPlot*>> baseline_map;

        // Name of the class
        std::string class_name;

        int channel_map[128];
        int feb_id; 
        int hybrid_id;

        int current_event; 

        bool readout_order;
};

#endif // __BASELINE_ANALYIS_H__

