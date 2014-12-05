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
	: document(NULL), system_node(NULL), config_node(NULL),
	  system_node_name("system"), config_node_name("config"),
	  feb_node_name("//Feb")
{}

SvtXmlReader::SvtXmlReader(std::string xml_file_name)
	: document(NULL), system_node(NULL), config_node(NULL),
	  system_node_name("system"), config_node_name("config"), 
	  feb_node_name("//Feb")
{
	// Open the file and parse it into a tree
	this->open(xml_file_name); 
}

SvtXmlReader::~SvtXmlReader() {

	feb_nodes.clear();
}

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

	// The root node "system" should only have a single child node named "config".
	// Get the child node and verify that it's name is "config"
	config_node = system_node->xmlChildrenNode;
	std::cout << "[ SvtXmlReader ]: Child element name: " << config_node->name << std::endl;
	if (xmlStrcmp(config_node->name, (const xmlChar *) config_node_name.c_str())) {
		xmlFreeDoc(document);
		throw std::runtime_error("[ ERROR ]: Root node is of the wrong type."); 
	}

	// Instantiate the xpath context to be used when retrieving all FEB nodes
	xmlXPathContextPtr context = xmlXPathNewContext(document);
	if (context == NULL) { 
		throw std::runtime_error("[ ERROR ]: Cannot create XPath context."); 	
	}

    // Get all of the FEB nodes in the document and free the context.  If no
	// FEB were found, throw an exception
	xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*) feb_node_name.c_str(), context);
	xmlXPathFreeContext(context);
	if (result == NULL) { 
		throw std::runtime_error("[ ERROR ]: xmlXPathEvalExpression returned NULL"); 	
	}
	std::cout << "[ SvtXmlReader ]: Total FEB nodes found: " << result->nodesetval->nodeNr << std::endl;

	// Loop through the FEB nodes that were found and create FebNode objects
	for (int feb_node_n = 0; feb_node_n < result->nodesetval->nodeNr; ++feb_node_n) {

	    FebNode* feb_node = new FebNode(result->nodesetval->nodeTab[feb_node_n]);
	    feb_nodes[feb_node->getFebID()] = feb_node;
	}
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

FebNode* SvtXmlReader::getFebNode(int feb_id) {

    // If a document hasn't been created, throw a runtime exception
    if (document == NULL) {
        throw std::runtime_error("[ ERROR ]: The document has not been opened.");
    }

    return feb_nodes[feb_id];
}

