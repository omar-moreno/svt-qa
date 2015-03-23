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

class SvtXmlReader { 

    public: 

        /**
         *  Default Constructor
         */
        SvtXmlReader();

        /**
         *  Constructor
         */
        SvtXmlReader(std::string xml_file_name);

        /**
         *  Destructor
         */
        virtual ~SvtXmlReader();

        /**
         *  Open the SVT XML document and parse it to a tree. 
         *
         *  @param xml_file_name : The name of the SVT XML document.
         */
        void open(std::string xml_file_name);

        /**
         *  Method to check if an XML document has been opened
         *
         *  @return true if it has been opened, false otherwise
         */
        bool isOpen(); 

        /**
         *  Close the SVT XML reader
         */
        void close();

        /**
         *  Get a pointer to the current node in the XML document tree.
         *
         *  @return pointer to the current node in the XML document tree 
         */
        xmlNodePtr getCurrentNode(); 

    protected: 
        // Pointer to the XML document
        xmlDocPtr document; 
        
        // Pointer to the current node in the XML document tree
        xmlNodePtr current_node; 

    private:

        
        // Pointer to the root element "system"
        xmlNodePtr system_node; 

        // The root element name
        std::string system_node_name;

}; // SvtXmlReader

#endif // __SVT_XML_READER_H__
