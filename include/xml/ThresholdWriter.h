/**
 *  @file   ThresholdWriter.h
 *  @brief  Class use to write SVT thresholds to an XML file.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   February 04, 2015
 */

#ifndef __THRESHOLD_WRITER_H__
#define __THRESHOLD_WRITER_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>

//--------------//
//--- LibXML ---//
//--------------//
#include <libxml/tree.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <SvtXmlWriter.h>
#include <QAUtils.h>

class ThresholdWriter : public SvtXmlWriter { 

    public:

        /**
         *  Default Constructor
         */
        ThresholdWriter();

        /**
         *  Destructor
         */
        ~ThresholdWriter(); 

        /**
         *  Open the SVT XML document
         *
         *  @param file_name - The name of the XML document
         */
        void open(std::string file_name);

        /**
         *  Set the number of RMS noise above baseline that the threshold 
         *  should be set to i.e. the factor multiplicative factor N in
         *  the equation threshold = baseline + N*noise. 
         *
         *  @param threshold : Number of RMS noise above baseline
         */
        void setThreshold(const double threshold) { this->threshold = threshold; }; 

        /**
         *  Write a threshold value to an XML document
         *
         *  @param feb_id : The FEB ID associated with the threshold value
         *  @param hybrid_id : The hybrid ID associated with the threshold value
         *  @param apv : The APV25 ID associated with the threshold value
         *  @param channel : The channel associated with the threshold value
         *  @param sample : The sample number associated with the threshold value
         *  @param baseline : The baseline value to be used in the threshold calculation
         *  @param noise : The noise value to be used in the threshold calculation
         */
        void writeThreshold(int feb_id, int hybrid_id, int channel, 
                double baseline, double noise); 


    private:

        double threshold;

}; // ThesholdWriter

#endif // __THRESHOLD_WRITER_H__
