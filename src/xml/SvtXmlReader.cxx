/**
 * 
 *  @file   SvtXmlReader.cxx
 *  @brief  XML reader used to read SVT conditions.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   November 15, 2014  
 */

#include <SvtXmlReader.h>

SvtXmlReader::SvtXmlReader()
    : document(NULL), system_node(NULL), system_node_name("system")
{}

SvtXmlReader::SvtXmlReader(std::string xml_file_name)
    : document(NULL), system_node(NULL), system_node_name("system")
{
    // Open the file and parse it into a tree
    this->open(xml_file_name); 
}

SvtXmlReader::~SvtXmlReader() {}

void SvtXmlReader::open(std::string xml_file_name) {

    // If a document is already opened, throw an exception
    if (document != NULL) { 
        throw std::runtime_error("[ ERROR ]: A document is already opened.");
    }

    LIBXML_TEST_VERSION;

    // Disable whitespace parsing
    xmlKeepBlanksDefault(0);
    
    // Open the XML document and parse it into a tree
    document = xmlReadFile(xml_file_name.c_str(), NULL, 0);
    
    // If the document can't be opened, throw a runtime exception
    if (document == NULL) { 
        throw std::runtime_error("[ ERROR ]: Failed to parse file " + xml_file_name); 
    }

    // Get the root node from the tree.  The root node should have a name of "system"
    system_node = xmlDocGetRootElement(document);

    // Make sure the document is not empty
    if (system_node == NULL) { 
        xmlFreeDoc(document); 
        throw std::runtime_error("[ ERROR ]: XML document doesn't contain a system node.");
    }

    // Check whether the root element of the document is actually "system"
    if (xmlStrcmp(system_node->name, (const xmlChar *) system_node_name.c_str())) {
        xmlFreeDoc(document);
        throw std::runtime_error("[ ERROR ]: Root node is of the wrong type."); 
    }
    std::cout << "[ SvtXmlReader ]: Root element name: " << system_node->name << std::endl;

    // Set the current node to point to the system node
    current_node = system_node; 
}

void SvtXmlReader::close() {

    // If a document hasn't been created, throw a runtime exception
    if(document == NULL){
        throw std::runtime_error("[ ERROR ]: A document hasn't been created.");
    }

    // Free the document
    xmlFreeDoc(document);

    // Cleanup the parser
    xmlCleanupParser();
}

xmlNodePtr SvtXmlReader::getCurrentNode() { 
    return current_node; 
}

