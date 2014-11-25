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
	
		std::string histo_name = this->getName() + " - Sample " + PlottingUtils::toString(sample_n);
		plots.push_back(new TH2F(histo_name.c_str(), histo_name.c_str(), 640, 0, 640, 16384, 0, 16384));	
	}
}

SamplesPlot::~SamplesPlot() {

	plots.clear();

}

void SamplesPlot::fill(int sample_n, int channel, double pedestal) { 

	plots[sample_n]->Fill(channel, pedestal);	
}

void SamplesPlot::setXAxisTitle(int sample_n, std::string title) { 
	plots[sample_n]->GetXaxis()->SetTitle(title.c_str());
}

void SamplesPlot::setXAxisTitles(std::string title) { 
	for (int plot_n; plot_n < plots.size(); ++plot_n) { 
		this->setXAxisTitle(plot_n, title);
	}
}

void SamplesPlot::setYAxisTitle(int sample_n, std::string title) { 
	plots[sample_n]->GetYaxis()->SetTitle(title.c_str()); 
}

void SamplesPlot::setYAxisTitles(std::string title) { 
	for (int plot_n; plot_n < plots.size(); ++plot_n) { 
		this->setYAxisTitle(plot_n, title);
	}
}

TH2F* SamplesPlot::getPlot(int sample_n) { 

	return plots[sample_n]; 
}
