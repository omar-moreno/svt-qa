
#include <SvtXmlWriter.h>

SvtXmlWriter::SvtXmlWriter()
	: document(NULL), system_node(NULL), config_node(NULL), 
	  file_name(""), 
	  bytes_written(-1)	  
{}

SvtXmlWriter::~SvtXmlWriter()
{}

void SvtXmlWriter::openDocument(std::string file_name)
{

	LIBXML_TEST_VERSION;

	// Create a new document
	document = xmlNewDoc(BAD_CAST "1.0");

	// Set the file name that the document will be saved to
	this->file_name = file_name; 

	//--- Build and SVT document ---//
	//------------------------------//
	
	// Create and set the root node "system" 
	system_node = xmlNewNode(NULL, BAD_CAST "system");
	xmlDocSetRootElement(document, system_node);

	// Create a child node "config" that is attached to the node "system"
	config_node = xmlNewChild(system_node, NULL, BAD_CAST "config", NULL);
	
}


void SvtXmlWriter::closeDocument()
{
	// If a document hasn't been created, notify the user and exit
	if(document == NULL){
		std::cerr << "[ERROR]: A document hasn't been created." << std::endl;
		return;
	}

	// Save the document to the file name specified by the user.  If the file
	// can't be saved, notify the user and exit.
	bytes_written = xmlSaveFormatFile(file_name.c_str(), document, 1); 
	if(bytes_written < 0){
		std::cerr << "[ERROR]: Could not write file to " << file_name << "." << std::endl; 
		return;
	}

	// Free the document
	xmlFreeDoc(document); 
}


void SvtXmlWriter::addFeb(int feb_id)
{	
	if(document == NULL || system_node == NULL || config_node == NULL){
		std::cerr << "[ERROR]: Cannot create FEB node. Either the system/configuration nodes "
			      << "have not been created or the document has not been opened." << std::endl;
		return;
	}
	
	// Create a FEB node as a child of the config node
	xmlNodePtr feb_node = xmlNewChild(config_node, NULL, BAD_CAST "FrontEndTestFpga", NULL);
	feb_nodes.insert(std::pair<int,xmlNodePtr>(feb_id, feb_node)); 

	// Add the FEB ID to FEB node as an attribute
	xmlNewProp(feb_nodes[feb_id], BAD_CAST "index", 
				BAD_CAST numberToString<int>(feb_id).c_str()); 
}

void SvtXmlWriter::addHybrid(int feb_id, int feb_hybrid_id)
{
	if(document == NULL || system_node == NULL || config_node == NULL){
		std::cerr << "[ERROR]: Cannot create FEB node. Either the system/configuration nodes "
			      << "have not been created or the document has not been opened." << std::endl;
		return;
	}

	// Get the FEB node associated with the given hybrid.  If the FEB node doesn't exist, 
	// create it
	std::map<int, xmlNodePtr>::iterator node_iterator = feb_nodes.find(feb_id); 
	if(node_iterator == feb_nodes.end()){
		// Create a FEB node with the appropriate identifier
		addFeb(feb_id);  
	}
	xmlNodePtr feb_node = feb_nodes[feb_id]; 

	// Create a hybrid node as a child of the FEB node
	xmlNodePtr hybrid_node = xmlNewChild(feb_node, NULL, BAD_CAST "Hybrid", NULL);
	
	// Add the hybrid ID to the hybrid node as an attribute
	xmlNewProp(hybrid_node, BAD_CAST "index", 
				BAD_CAST numberToString<int>(feb_hybrid_id).c_str());

	// Create 5 APV25 nodes as children of the hybrid node
	xmlNodePtr apv_node = NULL;
	for(int apv_n = 0; apv_n < 5; ++apv_n){
		apv_node = 	xmlNewChild(hybrid_node, NULL, BAD_CAST "apv25", NULL);

		// Add the APV25 index to the APV25 node as an attribute
		xmlNewProp(apv_node, BAD_CAST "index", BAD_CAST numberToString<int>(apv_n).c_str());
	
		// Create 128 channel nodes as children of the apv25 node
		xmlNodePtr channel_node = NULL;
		for(int channel_n = 0; channel_n < 128; ++channel_n){
			channel_node = 	xmlNewChild(apv_node, NULL, BAD_CAST "channel", NULL);

			// Add the channel index to the channel node as an attribute
			xmlNewProp(channel_node, BAD_CAST "index", 
						BAD_CAST numberToString<int>(channel_n).c_str());
		}	
	}
}

void SvtXmlWriter::addChannelData(std::string type, 
		int feb_id, int feb_hybrid_id, int apv, int channel, int sample, double value)
{
	if(document == NULL || system_node == NULL || config_node == NULL){
		std::cerr << "[ERROR]: Cannot create FEB node. Either the system/configuration nodes "
			      << "have not been created or the document has not been opened." << std::endl;
	}

	// Get the FEB node associated with the channel data.  If the FEB node doesn't exist, 
	// notify the user and exit
	std::map<int, xmlNodePtr>::iterator node_iterator = feb_nodes.find(feb_id); 
	if(node_iterator == feb_nodes.end()){
		std::cerr << "[ERROR]: Cannot add channel data. " 
				  << "The FEB node has not been created." << std::endl;
	
		return;
	}
	xmlNodePtr feb_node = feb_nodes[feb_id]; 
	
	// Loop through all of the childen (Hybrid nodes) of the feb_node and get
	// the hybrid node whose id matches the given id.
	xmlChar* index = NULL;
	xmlNodePtr hybrid_node = feb_node->children;
	index = xmlGetProp(feb_node, BAD_CAST "index");
	while(hybrid_node != NULL){
		index = xmlGetProp(hybrid_node, BAD_CAST "index");
		if(xmlStrEqual(index, BAD_CAST numberToString<int>(feb_hybrid_id).c_str())){
			xmlFree(index);
			break;
		}
		hybrid_node = hybrid_node->next;
	}	
	
	// Loop through all of the children (apv25 nodes) of the hybrid_node and get
	// the apv25 node whose id matches the apv25 number
	xmlNodePtr apv25_node = hybrid_node->children;
	while(apv25_node != NULL){
		index = xmlGetProp(apv25_node, BAD_CAST "index");
		if(xmlStrEqual(index, BAD_CAST numberToString<int>(apv).c_str())){
			xmlFree(index);
			break;
		}
		apv25_node = apv25_node->next;
	}

	// Loop through all of the children (channel nodes) of the apv25_node and
	// get the channel node whose id matches the channel number.
	xmlNodePtr channel_node = apv25_node->children;
	while(channel_node != NULL){
		index = xmlGetProp(channel_node, BAD_CAST "index");
		if(xmlStrEqual(index, BAD_CAST numberToString<int>(channel).c_str())){
			xmlFree(index);
			break;
		}
		channel_node = channel_node->next;
	}

	// Add a node of the given type to the channel along with the specified
	// value
	xmlNodePtr data_node = xmlNewChild(channel_node, NULL, 
			BAD_CAST type.c_str(), BAD_CAST numberToString<double>(value).c_str());
	// If a sample number is specified, add the sample number as an attribute
	if(sample != -1){
		// Add the channel index to the channel node as an attribute
		xmlNewProp(data_node, BAD_CAST "index", 
					BAD_CAST numberToString<int>(sample).c_str());
	}
}


