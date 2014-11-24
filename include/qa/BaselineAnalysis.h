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
#include <unordered_map>
#include <vector>

//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <QAUtils.h>
#include <CalibrationWriter.h>
#include <PlotUtils.h>
#include <SamplesPlot.h>

//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TH2F.h>
#include <TH1D.h>

class pairHash { 	

	public: 
		//template <typename T, typename U> 
		//std::size_t operator() (const std::pair <T, U> &p) const {
		std::size_t operator() (const std::pair <int, int> &p) const {
				// This is just a copy of what boost::hash does.  
				size_t seed = std::hash<int>()(p.first);
				return std::hash<int>()(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2);	
			}
};


class BaselineAnalysis : public QAAnalysis { 

	public: 

		BaselineAnalysis();	
		BaselineAnalysis(int, int);	
		~BaselineAnalysis(); 

        void initialize();
		void processEvent(TriggerSample*); 
        void finalize();
		std::string toString(); 

        //void findCalibrations(TH1*, double &, double &);

    private: 

        TCanvas* canvas;

        CalibrationWriter* writer; 
		
		//std::unordered_map <std::pair <int, int>, TH2F*, pairHash> baseline_map; 
		std::unordered_map <std::pair <int, int>, SamplesPlot*, pairHash> baseline_map; 

		std::string class_name;

		int feb_id; 
		int hybrid_id;
};

#endif // __BASELINE_ANALYIS_H__

