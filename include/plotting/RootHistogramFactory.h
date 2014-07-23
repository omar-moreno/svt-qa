/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 * @date: July 23, 2014
 */

#ifndef __ROOT_HISTOGRAM_FACTORY_H__
#define __ROOT_HISTOGRAM_FACTORY_H__

//--- C++ ---//
//-----------//
#include <string>

//--- Root ---//
//------------//
#include <TH1F.h>
#include <TH2F.h>

class RootHistogramFactory { 

	public: 

		~RootHistogramFactory();
	
		/**
		 * Create 1D Histograms
		 */
		static TH1* create1DHistogram(std::string, int, int, int);

		/**
		 * Create 2D Histograms
		 */
		static TH2* create2DHistogram(std::string, int, int, int, int, int, int);
};

#endif // __ROOT_HISTOGRAM_FACTORY_H__

