/**
 *
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *  @date:   May 5, 2014
 *
 */

#ifndef __BASELINE_ANALYSIS_H__
#define __BASELINE_ANALYSIS_H__

//--- StdLib ---//
//--------------//
#include <string>

//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <QAUtils.h>
#include <CalibrationWriter.h>

//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TH2F.h>
#include <TF1.h>
#include <TH1D.h>

class BaselineAnalysis : public QAAnalysis { 

	public: 

		BaselineAnalysis();	
		BaselineAnalysis(int, int);	
		~BaselineAnalysis(); 

        void initialize();
		void processEvent(TriggerSample*); 
        void finalize();
		std::string toString(); 

        void findCalibrations(TH1*, double &, double &);

    private: 

        // TODO: Use a histogram factory to handle booking of histograms directly
        TCanvas* canvas;
        TH2F* baseline_h; 
        TF1* gaussian;         

        CalibrationWriter* writer; 

		std::string class_name;

		int feb_id; 
		int hybrid_id;
};

#endif // __BASELINE_ANALYIS_H__
