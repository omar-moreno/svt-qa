/**
 * 
 *  @file   SvtXmlWriter.h
 *  @brief  XML writer used to write SVT conditions.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   September 15, 2014  
 */

#ifndef __SVT_XML_WRITER_H__
#define __SVT_XML_WRITER_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <stdexcept>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <FebNode.h>

class SvtXmlWriter { 

	public: 

        /**
         *  Default Constructor
         */
		SvtXmlWriter(); 

        /**
         *  Destructor
         */
		virtual ~SvtXmlWriter(); 

        /**
         *  Open the SVT XML document
         *
         *  @param file_name - The name of the XML document
         */
		void open(std::string file_name);
		
        /**
         *  Close the SVT XML document
         */
        void close();

        /**
         *  Get the FebNode corresponding to the given FEB ID
         *
         *  @param feb_id - The FEB ID of the FebNode of interest
         *  @return The FebNode corresponding to the given ID 
         */
        FebNode* getFebNode(int feb_id);

	private:

		// Pointer to the XML document
		xmlDocPtr document; 
		
		// Pointer to the root element "system"
		xmlNodePtr system_node; 

		// Pointer to the config element
		xmlNodePtr config_node; 

        // A mapping between a FEB ID and the corresponding FEB node
        std::unordered_map <int, FebNode*> feb_nodes;

		// Name of the file the document will be saved to
		std::string file_name;

		// Number of bytes written
		int bytes_written;		

}; // SvtXmlWriter

#endif // __SVT_XML_WRITER_H__
