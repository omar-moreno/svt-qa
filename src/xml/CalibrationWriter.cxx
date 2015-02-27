/**
 *
 *  @file   CalibrationWriter.cxx 
 *  @brief  XML writer used to write SVT baseline and noise conditions.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   September 15, 2014 
 */

#include <CalibrationWriter.h>

CalibrationWriter::CalibrationWriter() {}

CalibrationWriter::~CalibrationWriter() {

    // Destroy all FEB nodes stored in the map
    std::unordered_map <int, FebNode*>::iterator feb_it = feb_nodes.begin();
    for (feb_it; feb_it != feb_nodes.end(); ++feb_it) { 
        delete feb_it->second;
    } 
    feb_nodes.clear();  
}

void CalibrationWriter::open(std::string file_name) { 

    // Open the XML document
    SvtXmlWriter::open(file_name);

    //  Create all of the FEB and hybrid nodes
    for (int feb = 0; feb < 10; feb++) { 
        this->createFebNode(feb);
    }

}

void CalibrationWriter::writeBaseline(int feb_id, int hybrid_id, int channel,
        int sample, double baseline) {
        
    this->getFebNode(feb_id)->getHybridNode(hybrid_id)->writeChannelData(
              "baseline", channel, sample, baseline);
}

void CalibrationWriter::writeNoise(int feb_id, int hybrid_id, int channel,
                                   int sample, double noise) {
    this->getFebNode(feb_id)->getHybridNode(hybrid_id)->writeChannelData(
        "noise", channel, sample, noise);
}

void CalibrationWriter::createFebNode(int feb_id) { 
    
    // If a document hasn't been created, throw a runtime exception
    if (!this->isOpen()) { 
        throw std::runtime_error("[ ERROR ]: The XML document has not been opened."); 
    }
    
    // Check if the FebNode already exists.  If it doesn't, create it.
    if (feb_nodes.find(feb_id) == feb_nodes.end()) { 
        feb_nodes[feb_id] = new FebNode(this->getCurrentNode(), feb_id);
        std::cout << "[ SvtXmlWriter ]: FebNode with ID " << feb_id 
                  << " was created." << std::endl;
   
    }
}

FebNode* CalibrationWriter::getFebNode(int feb_id) { 
    
    // If a document hasn't been created, throw a runtime exception
    if (!this->isOpen()) { 
        throw std::runtime_error("[ ERROR ]: The XML document has not been opened."); 
    }
    
    return feb_nodes[feb_id];
}

