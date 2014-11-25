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

#include <PlottingUtils.h>

#include <TH2F.h>

class SamplesPlot { 
	
	public: 

		/**
		 */
		SamplesPlot(int, std::string);	
		
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
		void setXAxisTitles(std::string);

		/**
		 *
		 */
		void setYAxisTitles(std::string); 	

		/**
		 *
		 */
		TH2F* getPlot(int);

		/**
		 *
		 */	
		TH2F* getSumOfPlots();

		/**
		 */
		std::string getName() const { return name; };

		/**
		 *
		 */
		int getNumberOfPlots() const { return n_plots; };
	
	private:


		TH2F* sum_plot; 	
		std::vector <TH2F*> plots;
		
		std::string name;
		std::string x_axis_title;
		std::string y_axis_title;

		int n_plots; 

}; // SamplesPlot

#endif // __SAMPLES_PLOT_H__
