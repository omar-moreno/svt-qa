/**
 * 
 *  @file   SvtXmlReader.h
 *  @brief  XML reader used to read SVT conditions.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   November 15, 2014  
 */

#ifndef __SVT_XML_READER_H__
#define __SVT_XML_READER_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <stdexcept>
#include <iostream>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

class SvtXmlReader { 

	public: 

		/**
		 *	Constructor
		 */
		SvtXmlReader(std::string xml_file_name);

		/**
		 *	Destructor
		 */
		virtual ~SvtXmlReader();

		/**
		 *	Open the SVT XML document and parse it to a tree. 
		 *
		 *	@param xml_file_name - The name of the SVT XML document.
		 */
		void open(std::string xml_file_name);

	private:

		// Pointer to the XML document
		xmlDocPtr document; 
		
		// Pointer to the root element "system"
		xmlNodePtr system_node; 

		// Pointer to the config element
		xmlNodePtr config_node;

		// Pointer to the set of hybrid pointers
		xmlNodeSetPtr hybrid_nodes; 	

		std::string system_node_name;
		std::string config_node_name; 	
		std::string hybrid_node_name;

}; // SvtXmlReader

#endif // __SVT_XML_READER_H__
