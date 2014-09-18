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

//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <QAUtils.h>

//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TH2F.h>

class BaselineAnalysis : public QAAnalysis { 

	public: 

		BaselineAnalysis(); 
		~BaselineAnalysis(); 

        void initialize();
		void processEvent(TriggerSample*); 
        void finalize();

    private: 

        // TODO: Use a histogram factory to handle booking of histograms directly
        TCanvas* canvas;
        TH2F* baseline_h;  

};

#endif // __BASELINE_ANALYIS_H__
