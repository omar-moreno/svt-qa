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

}; // CalibrationWriter

#endif // __CALIBRATION_WRITER__
