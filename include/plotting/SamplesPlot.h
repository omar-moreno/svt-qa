/**
 *	@file	SamplesPlot.h
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 *	@date	September 30, 2014
 *
 */

#ifndef __SAMPLES_PLOT_H__
#define __SAMPLES_PLOT_H__

#include <string>
#include <vector>

#include <PlotUtils.h>

#include <TH2F.h>

class SamplesPlot { 
	
	public: 

		/**
		 */
		SamplesPlot(std::string);	
		
		/**
		 */
		~SamplesPlot(); 

		/**
		 */
		void fill(int, int, double);
		
		/**
		 */	
		void setName(const std::string name) { this->name = name; }; 	

		/**
		 *
		 */
		TH2F* getPlot(int); 

		/**
		 */
		std::string getName() const { return name; };
	
	private:

		std::vector <TH2F*> plots;	

		std::string name;

}; // SamplesPlot

#endif // __SAMPLES_PLOT_H__
