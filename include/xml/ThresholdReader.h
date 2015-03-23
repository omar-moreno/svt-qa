

#ifndef __THRESHOLD_READER_H__
#define __THRESHOLD_READER_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <cmath>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <SvtXmlReader.h>

class ThresholdReader : public SvtXmlReader { 

    public:

        /**
         *  Default Constructor
         */
        ThresholdReader();

        /**
         *   Destructor
         */  
        ~ThresholdReader();

        /**
         *  Open the SVT XML document
         *
         *  @param file_name - The name of the XML document
         */
        void open(std::string file_name);

        int getThreshold(int feb, int hybrid, int apv, int channel);

    private:
    
        void loadThresholds(xmlNodePtr node); 

        void parseThresholdNode(xmlNodePtr node, int &feb, int &hybrid, int &apv, int &channel, int &threshold);

        int thresholds[10][4][5][128];

        int feb;
        int hybrid;
        int apv; 
        int channel;
        int threshold;
};

#endif
