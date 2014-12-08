/**
 *  @file   BaselineAnalysis.h
 *  @brief  Analysis class used to extract baseline and noise values for all
 *          SVT channels.
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 *  @date   May 5, 2014
 *
 */

#ifndef __BASELINE_ANALYSIS_H__
#define __BASELINE_ANALYSIS_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <unordered_map>
#include <vector>

//--------------//
//--- SVT QA ---//
//--------------//
#include <QAAnalysis.h>
#include <QAUtils.h>
#include <CalibrationWriter.h>
#include <PlottingUtils.h>
#include <SamplesPlot.h>

//------------//
//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>

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

		/**
         * Default Constructor
		 */
		BaselineAnalysis();	
	
        /**
         *  Constructor
         *
         *  @param feb_id - FEB ID
         *  @param hybrid_id - Hybrid ID
         */    
		BaselineAnalysis(int feb_id, int hybrid_id);	
	
		/**
         * Destructor
		 */
		~BaselineAnalysis(); 

		/**
		 */
        void initialize();

		/**
		 */
		void processEvent(TriggerSample*);
	
		/**
		 */
		void processBaselinePlot(int, int, SamplesPlot*);

		/**
		 *
		 */
		void readoutOrder(bool readout_order);	
        

		/**
		 */
		void finalize();
		
		/**
		 */
		std::string toString(); 

    private: 

        TCanvas* canvas;

        CalibrationWriter* writer; 
		
		std::unordered_map <std::pair <int, int>, SamplesPlot*, pairHash> baseline_map; 

		std::string class_name;

		//std::vector <int> channel_map;
		int channel_map[128];
		int feb_id; 
		int hybrid_id;

		bool readout_order;
};

#endif // __BASELINE_ANALYIS_H__

