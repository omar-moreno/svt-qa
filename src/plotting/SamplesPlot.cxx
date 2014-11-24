/**
 *	@file	SamplesPlot.cxx
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 *	@date	September 30, 2014
 *
 */

#include <SamplesPlot.h>

SamplesPlot::SamplesPlot(std::string name) 
	: name(name) { 
	
	for (int sample_n = 0; sample_n < 6; ++sample_n) { 
	
		std::string histo_name = this->getName() + " - Sample " + PlotUtils::toString(sample_n);
		plots.push_back(new TH2F(histo_name.c_str(), histo_name.c_str(), 640, 0, 640, 16384, 0, 16384));	
	}

}

SamplesPlot::~SamplesPlot() {

	plots.clear();

}


void SamplesPlot::fill(int sample_n, int channel, double pedestal) { 

	plots[sample_n]->Fill(channel, pedestal);	
}

TH2F* SamplesPlot::getPlot(int sample_n) { 

	return plots[sample_n]; 
}
