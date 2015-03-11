/**
 *
 *  @file   ThresholdWriter.cxx
 *  @brief  Class use to write SVT thresholds to an XML file.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   February 04, 2015
 */

#include <ThresholdWriter.h>

ThresholdWriter::ThresholdWriter() {}

ThresholdWriter::~ThresholdWriter() {}

void ThresholdWriter::open(std::string file_name) { 

    // Open the XML document
    SvtXmlWriter::open(file_name); 

    // Create a node "config" that is a child of the root node "system"
    current_node = xmlNewChild(this->getCurrentNode(), NULL, BAD_CAST "config", NULL); 
    
    // Create a node "DataDpm" that is a child of the node "config"
    current_node = xmlNewChild(this->getCurrentNode(), NULL, BAD_CAST "DataDpm", NULL);

    // Create a node "RceCore" that is a child of the node "DataDpm"
    current_node = xmlNewChild(this->getCurrentNode(), NULL, BAD_CAST "RceCore", NULL);

    // Create a node "ThresholdSet" that is a child of the node "RceCore".
    // The children of this node will contain all of the threshold elements.
    current_node = xmlNewChild(this->getCurrentNode(), NULL, BAD_CAST "ThresholdSet", NULL);

}

void ThresholdWriter::writeThreshold(int feb_id, int hybrid_id, int channel,
        double baseline, double noise) { 

    // Get the APV25 number from the physical channel number
    int apv = QAUtils::getApv25FromPhysicalChannel(channel);

    // Get the APV25 channel number from the physical channel number
    int apv_channel = QAUtils::getApv25ChannelFromPhysicalChannel(channel);

    // Construct the threshold node name
    std::string threshold_node_name = "Feb" + std::to_string(feb_id) +
                                      "Hybrid" + std::to_string(hybrid_id) +
                                      "Apv" + std::to_string(apv) + 
                                      "Channel" + std::to_string(apv_channel);

    // Calculate the threshold
    int threshold_value = ceil(baseline + 2*noise); 

    // Add a threshold node with the calculated threshold
    xmlNewChild(this->getCurrentNode(), NULL, BAD_CAST threshold_node_name.c_str(), 
            BAD_CAST std::to_string(threshold_value).c_str());

}
