/**
 *
 *  @file   FebNode.h
 *  @brief  A class used to describe a FEB in an XML tree.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   November 30, 2014 
 */

#ifndef __FEB_NODE_H__
#define __FEB_NODE_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <HybridNode.h>

class FebNode {

	public: 

        /**
         *  Constructor
         *
         *  @param parent_node - The parent node of this node
         *  @param feb_id - The FEB ID associated with this node
         */
		FebNode(xmlNodePtr parent_node, int feb_id);

		/**
		 *  Constructor
		 *
		 *  @param feb_node - The FEB node
		 */
		FebNode(xmlNodePtr feb_node);
		
        /**
         *  Destructor
         */
        ~FebNode(); 

        /**
         *  Get the HybridNode corresponding to the given hybrid ID.
         *
         *  @param  hybrid_id - The hybrid ID of the HybridNode of interest
         *  @return The HybridNode corresponding to the given ID
         */
        HybridNode* getHybridNode(int hybrid_id);

        /**
         *  Get the FEB ID
         *
         *  @return FEB ID
         */
        int getFebID() const { return feb_id; };
	
	private:

        // Pointer to parent
        xmlNodePtr parent_node;  

        // Pointer to a FEB node
		xmlNodePtr feb_node;

        // Front End Board ID
        int feb_id;

        // A collection of Hybrid nodes corresponding to this FEB
        std::vector <HybridNode*> hybrid_nodes; 

}; // FebNode 

#endif // __FEB_NODE_H__
