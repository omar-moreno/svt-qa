
#ifndef __SVT_XML_WRITER_H__
#define __SVT_XML_WRITER_H__

//--------------//
//--- StdLib ---//
//--------------//
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>

class SvtXmlWriter { 

	public: 

		SvtXmlWriter(); 
		virtual ~SvtXmlWriter(); 

		void openDocument(std::string);
		void closeDocument();

		void addFeb(int);
		void addHybrid(int, int); 

		void addChannelData(std::string, int, int, int, int, int, double);	

	private:

		// Method to convert any value to a string 
		template<typename T>
			std::string numberToString(const T value)
			{
				std::ostringstream ss;
				ss << value;
				return ss.str();  
			}

		// Pointer to the XML document
		xmlDocPtr document; 
		
		// Pointer to the root element "system"
		xmlNodePtr system_node; 

		// Pointer to the config element
		xmlNodePtr config_node; 

		// A map between a FEB ID and the corresponding xmlNodePtr
		std::map<int, xmlNodePtr> feb_nodes; 

		// Name of the file the document will be saved to
		std::string file_name;

		// Number of bytes written
		int bytes_written;		

}; // SvtXmlWriter

#endif // __SVT_XML_WRITER_H__
