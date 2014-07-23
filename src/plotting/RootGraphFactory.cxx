/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 * @date: July 23, 2014
 */

#include <RootGraphFactory.h>

RootGraphFactory::~RootGraphFactory(){}

TGraph* RootGraphFactory::createGraph()
{
	return new TGraph();
}

