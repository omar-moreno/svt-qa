/**
 *
 *  @file   HybridNode.cxx
 *  @brief  A class used to describe a hybrid in an XML tree.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   November 30, 2014 
 */

#include <HybridNode.h>

HybridNode::HybridNode(xmlNodePtr parent_node, int hybrid_id) 
    : parent_node(parent_node), hybrid_id(hybrid_id)
{
    // Create the Hybrid node
    hybrid_node = xmlNewChild(parent_node, NULL, BAD_CAST "Hybrid", NULL);

    // Add the Hybrid ID to the Hybrid node as an attribute
    xmlNewProp(hybrid_node, BAD_CAST "id", 
                BAD_CAST std::to_string(hybrid_id).c_str()); 



    for (int channel = 0; channel < 640; ++channel) { 

        channel_nodes.push_back(xmlNewChild(hybrid_node, NULL,
                    BAD_CAST "channel", NULL));
        
        // Add the channel id to the channel node as an attribute
        xmlNewProp(channel_nodes[channel], BAD_CAST "id", 
                BAD_CAST std::to_string(channel).c_str());
    }
}

HybridNode::~HybridNode() {}

void HybridNode::writeChannelData(std::string type, int channel, int sample,
                                    double value){

    // Get the channel node that data will be written to
    xmlNodePtr channel_node = channel_nodes[channel]; 

    // Add a node of the given type to the channel along with the specified
    // value
    xmlNodePtr data_node = xmlNewChild(channel_node, NULL, 
            BAD_CAST type.c_str(), BAD_CAST std::to_string(value).c_str());

    // Add the channel index to the channel node as an attribute
    xmlNewProp(data_node, BAD_CAST "id", 
                BAD_CAST std::to_string(sample).c_str());
}
