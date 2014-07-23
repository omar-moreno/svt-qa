/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 * @date: July 23, 2014
 */

#include <RootHistogramFactory.h>

RootHistogramFactory::~RootHistogramFactory(){}

TH1* RootHistogramFactory::create1DHistogram(std::string name, int bins_x, int min_x, int min_y)
{
	return new TH1F(name.c_str(), name.c_str(), bins_x, min_x, min_x);
}

TH2* RootHistogramFactory::create2DHistogram(std::string name, int bins_x, int x_min, int x_max,
		int bins_y, int y_min, int y_max)
{
	return new TH2F(name.c_str(), name.c_str(), bins_x, x_min, x_max, bins_y, y_min, y_max); 
}
