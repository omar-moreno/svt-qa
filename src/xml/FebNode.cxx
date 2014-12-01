/**
 *
 *  @file   FebNode.cxx
 *  @brief  A class used to describe a FEB in an XML tree.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   November 30, 2014 
 */

#include <FebNode.h>

FebNode::FebNode(xmlNodePtr parent_node, int feb_id)
    : parent_node(parent_node), feb_node(NULL), feb_id(feb_id)
{
    // Create the FEB node 
    feb_node = xmlNewChild(parent_node, NULL, BAD_CAST "Feb", NULL);

    // Add the FEB ID to FEB node as an attribute
    xmlNewProp(feb_node, BAD_CAST "id", 
                BAD_CAST std::to_string(feb_id).c_str()); 

    
    // Create hybrid nodes for all hybrids typically associated with a FEB
    for (int hybrid_id = 0; hybrid_id < 4; ++hybrid_id) { 
        
        hybrid_nodes.push_back(new HybridNode(feb_node, hybrid_id));
    }
}

FebNode::~FebNode() {
    // Destroy all of the HybridNodes associated with this FebNode
    hybrid_nodes.clear();
}

HybridNode* FebNode::getHybridNode(int hybrid_id) { 
    return hybrid_nodes[hybrid_id];
}


