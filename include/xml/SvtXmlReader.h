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
#include <unordered_map>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <FebNode.h>

class SvtXmlReader { 

	public: 

        /**
         *  Default Constructor
         */
        SvtXmlReader();

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

        /**
         *  Close the SVT XML reader
         */
        void close();

        /**
         *  Get the FebNode corresponding to the given FEB ID
         *
         *  @param feb_id - The FEB ID of the FebNode of interest
         *  @return The FebNode corresponding to the given ID
         */
		FebNode* getFebNode(int feb_id);

		/**
		 *  Get all FebNode's as a vector
		 *
		 *  @return A vector containing all FebNodes
		 */
		std::vector<FebNode*> getFebNodes();

	private:

		// Pointer to the XML document
		xmlDocPtr document; 
		
		// Pointer to the root element "system"
		xmlNodePtr system_node; 

		// Pointer to the config element
		xmlNodePtr config_node;

        // A mapping between a FEB ID and the corresponding FEB node
        std::unordered_map <int, FebNode*> feb_nodes;

        // A vector containing all FebNodes
        std::vector<FebNode*> feb_node_collection;

		std::string system_node_name;
		std::string config_node_name; 	
		std::string feb_node_name;

}; // SvtXmlReader

#endif // __SVT_XML_READER_H__
