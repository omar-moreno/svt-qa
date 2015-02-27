/**
 * 
 *  @file   SvtXmlWriter.cxx
 *  @brief  XML writer used to write SVT conditions.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   September 15, 2014  
 */

#include <SvtXmlWriter.h>

SvtXmlWriter::SvtXmlWriter()
	: document(NULL), system_node(NULL), current_node(NULL), 
	  file_name(""), 
	  bytes_written(-1)	  
{}

SvtXmlWriter::~SvtXmlWriter() {}

void SvtXmlWriter::open(std::string file_name) {

	LIBXML_TEST_VERSION;

	// Create a new document
	document = xmlNewDoc(BAD_CAST "1.0");

	// Set the file name that the document will be saved to
	this->file_name = file_name; 

	//--- Build the SVT document ---//
	//------------------------------//
	
	// Create and set the root node "system"
	system_node = xmlNewNode(NULL, BAD_CAST "system");
	xmlDocSetRootElement(document, system_node);

    // Set the current node to point to the system node
    current_node = system_node; 
	
}

bool SvtXmlWriter::isOpen() { 
    
    if (document == NULL) return false;
   
    return true;
}

void SvtXmlWriter::close() {

    // If a document hasn't been created, throw a runtime exception
	if(document == NULL){
		throw std::runtime_error("[ ERROR ]: A document hasn't been created.");
	}

	// Save the document to the file name specified by the user.  If the file
	// can't be saved, notify the user and exit.
	bytes_written = xmlSaveFormatFile(file_name.c_str(), document, 1); 
	if(bytes_written < 0){
	    throw std::runtime_error("[ ERROR ]: Could not write file to "
                                    + file_name + "."); 
	}

	// Free the document
	xmlFreeDoc(document); 
}

xmlNodePtr SvtXmlWriter::getCurrentNode() { 
    return current_node; 
}

/*

FebNode* SvtXmlWriter::getFebNode(int feb_id) { 
  
    // If a document hasn't been created, throw a runtime exception
    if (document == NULL) { 
        throw std::runtime_error("[ ERROR ]: Cannot created a FEB node. The document has not been opened."); 
    }

    // Check if the FebNode already exists.  If it doesn't, create it.
    if (feb_nodes.find(feb_id) == feb_nodes.end()) { 
        feb_nodes[feb_id] = new FebNode(config_node, feb_id);
        std::cout << "[ SvtXmlWriter ]: FebNode with ID " << feb_id 
                  << " was created." << std::endl;
   
    }

    return feb_nodes[feb_id];
} */
