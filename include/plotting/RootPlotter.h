/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 * @date: July 23, 2014
 *
 */

#ifndef __ROOT_PLOTS_H__
#define __ROOT_PLOTS_H__

//--- C++ ---//
//-----------//
#include <string>
#include <map>
#include <iostream>

//--- ROOT ---//
//------------//
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

//--- Utils ---//
//-------------//
#include <RootHistogramFactory.h>
#include <RootGraphFactory.h>

class RootPlotter { 

	public: 

		// Constructor
		RootPlotter();

		// Destructor
		~RootPlotter(); 		

		TH1F* get1DHistogram(std::string, std::string, int, int, int);
		TH1F* get1DHistogram(std::string);
		TH2F* get2DHistogram(std::string, std::string, std::string, 
				int, int, int, int, int, int);
		TH2F* get2DHistogram(std::string);
		
		TGraph* getGraph(std::string, std::string, std::string);
		TGraph* getGraph(std::string);

		void SavePlots(std::string);	

	private: 

		std::map<std::string, TH1*> histogram1D_map;
		std::map<std::string, TH2*> histogram2D_map;
		std::map<std::string, TGraph*> graph_map;
			
		std::map<std::string, TH1*>::iterator histogram1D_iter;
		std::map<std::string, TH2*>::iterator histogram2D_iter;
		std::map<std::string, TGraph*>::iterator graph_iter;

		TCanvas* canvas; 	

}; // RootPlotter

#endif // __ROOT_PLOTS_H__
