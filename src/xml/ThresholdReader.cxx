
#include <ThresholdReader.h>

ThresholdReader::ThresholdReader() {}

ThresholdReader::~ThresholdReader() {}

void ThresholdReader::open(std::string file_name) { 
    
    // Open the XML document
    SvtXmlReader::open(file_name); 

    this->loadThresholds(this->getCurrentNode());
}

void ThresholdReader::loadThresholds(xmlNodePtr node) { 

    // Traverse the tree until the node containing all of the 
    // threshold elements is found
	while (xmlStrcmp(node->name, BAD_CAST "ThresholdSet")) {
	    std::cout << "[ ThresholdReader ]: Child element name: " 
                  << node->name << std::endl;
        node = node->xmlChildrenNode;
    }

    std::cout << "[ ThresholdReader ]: Retrieving Threshold set" << std::endl;

    int feb;
    int hybrid;
    int apv; 
    int channel;
    int threshold;
    node = node->xmlChildrenNode;
    while (node != NULL) { 
        
        this->parseThresholdNode(node, feb, hybrid, apv, channel, threshold);

        /*
        std::cout << "FEB: " << feb 
                  << " Hybrid: " << hybrid
                  << " APV: " << apv
                  << " channel: " << channel
                  << " Threshold: " << threshold 
                  << " Threshold from map: " << thresholds[feb][hybrid][apv][channel]
                  << " Threshold using get: " << this->getThreshold(feb, hybrid, apv, channel)
                  << std::endl;
        */
        node = node->next;
    }
}

void ThresholdReader::parseThresholdNode(xmlNodePtr node, int &feb, int &hybrid, int &apv, int &channel, int &threshold) { 

    std::string name = (const char*) node->name; 
    feb = (int) name[3] - '0';
    hybrid = (int) name[10] - '0';
    apv = (int) name[14] - '0';
    channel = (int) name[22] - '0';

    
   threshold = atoi((const char*) xmlNodeListGetString(document, node->xmlChildrenNode, 1));  

    thresholds[feb][hybrid][apv][channel] = threshold;
}

int ThresholdReader::getThreshold(int feb, int hybrid, int apv, int channel) { 
    return thresholds[feb][hybrid][apv][channel];
}

