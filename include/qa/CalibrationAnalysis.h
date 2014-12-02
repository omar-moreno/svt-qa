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

//--------------//
//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <PlottingUtils.h>

class CalibrationAnalysis : public QAAnalysis { 

    public: 

        /**
         *  Default Constructor
         */
        CalibrationAnalysis(); 

        /**
         *  Constructor
         *
         *  @param feb_id - FEB ID
         *  @param hybrid_id - Hybrid ID
         */    
        CalibrationAnalysis(int feb_id, int hybrid_id); 

        /**
         *  Destructor
         */
        ~CalibrationAnalysis(); 

		/**
		 */
        void initialize();

		/**
		 */
		void processEvent(TriggerSample*);

		/**
		 */
		void finalize();
		
		/**
		 */
		std::string toString();

    private: 

        std::string class_name; 

        int feb_id; 
        int hybrid_id; 

};  // CalibrationAnalysis

#endif // __CALIBRATION_ANALYSIS_H__
