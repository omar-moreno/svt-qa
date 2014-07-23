/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 * @date: July 23, 2014
 */

#ifndef __ROOT_GRAPH_FACTORY_H__
#define __ROOT_GRAPH_FACTORY_H__

//--- C++ ---//
//-----------//
#include <string>

//--- Root ---//
//------------//
#include <TGraph.h>
#include <TAxis.h>

class RootGraphFactory { 

	public: 

		~RootGraphFactory();

		/**
		 *	Create a graph
		 */
		static TGraph* createGraph();

};

#endif  // __ROOT_GRAPH_FACTORY_H__
