/**
 *
 *  @file   HybridNode.h
 *  @brief  A class used to describe a hybrid in an XML tree.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   November 30, 2014 
 */

#ifndef __HYBRID_NODE_H__
#define __HYBRID_NODE_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <iostream>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>
#include <libxml/xpath.h>

class HybridNode {

    public: 

        /**
         *  Constructor
         *
         *  @param parent_node - The parent node of this node
         *  @param hybrid_id - The Hybrid ID associated with this node
         */
        HybridNode(xmlNodePtr parent_node, int hybrid_id);

        /**
         *  Constructor
         *
         *  @param hybrid_node - The hybrid node
         */
        HybridNode(xmlNodePtr hybrid_node);

        /**
         *  Destructor
         */
        ~HybridNode();

        /**
         *  Add a child node of the given name holding a text node with the 
         *  given value to the parent node (channel) with ID attribute channel.
         *  The child node also has an attribute sample denoting what sample
         *  the text node corresponds to.
         *  
         *  @param name - Name of the child node
         *  @param channel - ID of the parent node
         *  @param sample - Sample ID of the child node
         *  @param value - Text node being held by the child node
         */
        void writeChannelData(std::string name, 
                              int channel, int sample, double value);

        /**
         *  Return the text node held by a child node of the given name/ID
         *  attribute (sample) and with a parent node (channel) with ID
         *  attribute (channel) as a double.
         *
         *  @param name - Name of the child node
         *  @param channel - ID of the parent node
         *  @param sample - Sample ID of the child node
         *  @return Text node being held by the child node as a double
         */
        //double getChannelData(std::string name,
        //                        int channel, int sample);


        /**
         *  Get the Hybrid ID
         *
         *  @return The Hybrid ID
         */
        int getHybridID() const { return hybrid_id; };  

    private:

        // Hybrid ID
        int hybrid_id; 

        // Pointer to parent
        xmlNodePtr parent_node;

        // Pointer to Hybrid node
        xmlNodePtr hybrid_node;

        // A collection of channel nodes
        std::vector<xmlNodePtr> channel_nodes;

        // XPath context
        xmlXPathContextPtr context;

}; // HybridNode

#endif // __HYBRID_NODE_H__
