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
    : parent_node(parent_node), hybrid_id(hybrid_id), context(NULL)
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

HybridNode::HybridNode(xmlNodePtr hybrid_node)
    : parent_node(NULL), hybrid_node(NULL), context(NULL)
{

    // Check that the node corresponds to a hybrid node.  If it isn't, throw
    // an exception
    if (xmlStrcmp(hybrid_node->name, (const xmlChar*) "Hybrid")) {
        throw std::runtime_error("[ ERROR ]: Expecting Hybrid node.");
    }

    // Set the pointer to the parent node
    this->parent_node = hybrid_node->parent;

    // Set the pointer to the hybrid node
    this->hybrid_node = hybrid_node;

    // Set the hybrid ID
    xmlChar* hybrid_id_string = xmlGetProp(hybrid_node, (const xmlChar*) "id");
    hybrid_id = atoi((char*) hybrid_id_string);
    xmlFree(hybrid_id_string);
    std::cout << "[ HybridNode ]: Hybrid ID: " << hybrid_id << std::endl;

    // Iterate through all of the child nodes and them to the vector of channels
    xmlNodePtr channel_node = hybrid_node->xmlChildrenNode;
    while (channel_node != NULL) {

        //std::cout << "[ Hybrid Node ]: Channel node name: " << channel_node->name
        //          << " ID: " << xmlGetProp(channel_node, (const xmlChar*) "id")
        //          << std::endl;
        channel_nodes.push_back(channel_node);
        channel_node = channel_node->next;
    }

    // Create the XPath context.  This is used when using xpath to retrieve
    // data
	context = xmlXPathNewContext(parent_node->doc);
	if (context == NULL) {
		throw std::runtime_error("[ ERROR ]: Cannot create XPath context.");
	}
}

HybridNode::~HybridNode() {

    // Free the XPath context
	xmlXPathFreeContext(context);
}

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

/*
double HybridNode::getChannelData(std::string name, int channel, int sample) {

    
    // Get the channel node that contains the data
    xmlNodePtr channel_node = channel_nodes[channel];

    // Get the child node with the given name and sample ID attribute
    std::string xpath_expression = name + "[@id='" + std::to_string(sample) + "']";
    xmlXPathObjectPtr result
        = xmlXPathNodeEval(channel_node, (const xmlChar*) xpath_expression.c_str(), context);
    std::cout << "[ HybridNode ]: Number of nodes found: " << result->nodesetval->nodeNr << std::endl;

    xmlChar* char_value = xmlNodeGetContent(result->nodesetval->nodeTab[0]);
    double value = strtod((const char*) char_value, NULL);

    xmlXPathFreeObject(result);
    xmlFree(char_value);

    return value;
}*/
