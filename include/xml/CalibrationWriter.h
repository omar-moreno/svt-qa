/**
 *
 *  @file   CalibrationWriter.cxx 
 *  @brief  XML writer used to write SVT baseline and noise conditions.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   September 15, 2014 
 */


#ifndef __CALIBRATION_WRITER__
#define __CALIBRATION_WRITER__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>

//--------------//
//--- SVT QA ---//
//--------------//
#include <SvtXmlWriter.h>
#include <FebNode.h>

class CalibrationWriter : public SvtXmlWriter { 

	public:

        /**
         *  Default Constructor
         */
		CalibrationWriter();

        /**
         *  Destructor
         */
		~CalibrationWriter();

        /**
         *  Open an XML calibration document
         *
         *  @param file_name - The name of the XML document
         */
        void open(std::string file_name);

        /**
         *  Write a baseline value to the XML document
         *
         *  @param feb_id - The FEB ID associated with the baseline value
         *  @param hybrid_id - The hybrid ID associated with the baseline value
         *  @param channel - The channel associated with the baseline value
         *  @param sample - The sample number associated with the baseline value
         *  @param baseline - The baseline value
         */
		void writeBaseline(int feb_id, int hybrid_id, int channel,
                           int sample, double baseline);
		
        /**
         *  Write a noise value to the XML document
         *
         *  @param feb_id - The FEB ID associated with the baseline value
         *  @param hybrid_id - The hybrid ID associated with the baseline value
         *  @param channel - The channel associated with the baseline value
         *  @param sample - The sample number associated with the baseline value
         *  @param noise - The noise value
         */
        void writeNoise(int feb_id, int hybrid_id, int channel, 
                        int sample, double noise);

    private:
    
        /**
         *  Create a FebNode for the specified FEB ID
         *
         *  @param feb_id : FEB ID 
         */
        void createFebNode(int feb_id);

        /**
         *  Get the FebNode corresponding to the given FEB ID
         *
         *  @param feb_id - The FEB ID of the FebNode of interest
         *  @return The FebNode corresponding to the given ID 
         */
        FebNode* getFebNode(int feb_id);

        // A mapping between a FEB ID and the corresponding FEB node
        std::unordered_map <int, FebNode*> feb_nodes;

}; // CalibrationWriter

#endif // __CALIBRATION_WRITER__
