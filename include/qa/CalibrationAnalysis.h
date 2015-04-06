/**
 *  @file   CalibrationAnalysis.h
 *  @brief  Analysis class used to fit calibration pulses and extract
 *          parameters.
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 *  @date   October 01, 2014
 *
 */

#ifndef __CALIBRATION_ANALYSIS_H__
#define __CALIBRATION_ANALYSIS_H__

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
#include <QAUtils.h>

//------------//
//--- ROOT ---//
//------------//
#include <TH2S.h>
#include <TFile.h>

class CalibrationAnalysis : public QAAnalysis { 

    public: 

        /**
         *  Default Constructor
         */
        CalibrationAnalysis(); 

		/**
         *  Parameterized constructor used when data from hybrids associated 
         *  with a single FEB is going to be processed.
         *
         *  @param feb_id : FEB ID
         */
        CalibrationAnalysis(int feb_id);

		/**
         *  Parameterized constructor used when data from hybrids associated 
         *  with a single FEB is going to be processed.
         *
         *  @param feb_id : FEB ID
         */
        CalibrationAnalysis(int feb_id, int hybrid_id); 

        /**
         *  Destructor
         */
        ~CalibrationAnalysis(); 

		/**
		 *  Method called at the beginning of an analysis class.
         */
        void initialize();
        
		/**
         *  Method used to process events.
         *
         *  @param event : A trigger event 
		 */
		void processEvent(TriggerEvent* event);

		/**
         *  Method used to process sets of samples.
         *
         *  @param samples : A set of samples 
		 */
        void processSamples(TriggerSample* samples);

        /**
         *
         */
        void setCalibrationGroup(int calibration_group) { 
            this->calibration_group = calibration_group; }; 
    
		/**
         *  Method called at the end of an analysis when all events have been
         *  processed.
		 */
		void finalize();
		
		/**
         *  String representation of this object
		 */
		std::string toString();

    private: 

        /**
         *
         */
        bool passSlopeCut(TriggerSample* samples); 

        /**
         *
         */
        void bookHistogram(int feb_id, int hybrid_id); 

        std::unordered_map <int, std::vector < std::vector<TH2S* >>> samples_map;

        TriggerSample* trigger_samples; 
        
        // ROOT output file
        TFile* output_file; 
   
        std::string class_name; 
        
        int feb_id; 
        int hybrid_id; 

        int calibration_group; 

};  // CalibrationAnalysis

#endif // __CALIBRATION_ANALYSIS_H__
