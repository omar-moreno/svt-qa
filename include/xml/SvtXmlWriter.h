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
		virtual void open(std::string file_name);

        /**
         *  Method to check if XML document has been opened
         *
         *  @return true if it has been opened, false otherwise
         */
        bool isOpen();

        /**
         *  Close the SVT XML document
         */
        void close();

        /**
         *  Get a pointer to the current node in the XML document tree.
         *
         *  @return pointer to the current node in the XML document tree 
         */
        xmlNodePtr getCurrentNode(); 

    protected:

        
        // Pointer to the current node in the XML document tree
        xmlNodePtr current_node; 

	private:

		// Pointer to the XML document
		xmlDocPtr document; 

		// Pointer to the root element "system"
		xmlNodePtr system_node;

		// Name of the file the document will be saved to
		std::string file_name;

		// Number of bytes written
		int bytes_written;		

}; // SvtXmlWriter

#endif // __SVT_XML_WRITER_H__
