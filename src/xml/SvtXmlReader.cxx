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

SvtXmlReader::SvtXmlReader(std::string xml_file_name)
	: document(NULL), system_node(NULL), config_node(NULL),
	  hybrid_nodes(NULL), 
	  system_node_name("system"), config_node_name("config"), 
	  hybrid_node_name("//Hybrid") 
{
	// Open the file and parse it into a tree
	this->open(xml_file_name); 
}

SvtXmlReader::~SvtXmlReader() {

	// Free the document
	xmlFreeDoc(document);	
}

void SvtXmlReader::open(std::string xml_file_name) {

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

	// The root node "system" has only child node "config".  Get the child node and verify
	// that it's name is "config"
	config_node = system_node->xmlChildrenNode;
	std::cout << "[ SvtXmlReader ]: Child element name: " << config_node->name << std::endl;
	if (xmlStrcmp(config_node->name, (const xmlChar *) config_node_name.c_str())) {
		xmlFreeDoc(document);
		throw std::runtime_error("[ ERROR ]: Root node is of the wrong type."); 
	}

	xmlXPathContextPtr context = xmlXPathNewContext(document);
	if (context == NULL) { 
		throw std::runtime_error("[ ERROR ]: Cannot create XPath context."); 	
	}

	xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*) hybrid_node_name.c_str(), context);
	xmlXPathFreeContext(context);
	if (result == NULL) { 
		throw std::runtime_error("[ ERROR ]: xmlXPathEvalExpression returned NULL"); 	
	}
	
	std::cout << "[ SvtXmlReader ]: Total Hybrid nodes found: " << result->nodesetval->nodeNr << std::endl;

	hybrid_nodes = result->nodesetval; 
	if (xmlXPathNodeSetIsEmpty(hybrid_nodes)) {
		xmlXPathFreeObject(result);
		throw std::runtime_error("[ ERROR ]: Couldn't find any nodes with name " + hybrid_node_name);
	}
}

