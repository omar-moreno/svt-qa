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

#include <TH2S.h>

class SamplesPlot { 
	
	public: 

		/**
         *  Constructor
         *
         *  @param n_plots : The number of samples that the plots will contain
         *  @param name : The name of the plots
		 */
		SamplesPlot(int n_samples, std::string name);	
		
		/**
         *  Destructor
		 */
		~SamplesPlot(); 

		/**
         * 
		 */
		void fill(int sample_n, int, double);
		
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
		TH2S* getPlot(int);

		/**
		 *
		 */	
		TH2S* getSumOfPlots();

		/**
		 */
		std::string getName() const { return name; };

		/**
		 *
		 */
		int getNumberOfPlots() const { return n_plots; };
	
	private:


		TH2S* sum_plot; 	
		std::vector <TH2S*> plots;
		
		std::string name;
		std::string x_axis_title;
		std::string y_axis_title;

		int n_plots; 

}; // SamplesPlot

#endif // __SAMPLES_PLOT_H__
