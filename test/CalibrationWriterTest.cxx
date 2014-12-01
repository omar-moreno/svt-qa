/**
 *	@file	    CalibrationWriterTest.cxx
 *	@brief		Test to check the functionality of the class CalibrationWriter.
 *	@author		Omar Moreno <omoreno1@ucsc.edu>
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date		September 15, 2014
 *
 */

#include <CalibrationWriter.h>

int main() {

    // Instantiate the writer
    CalibrationWriter* writer = new CalibrationWriter();
    
    // Open a file to write calibrations to
    writer->open("test.xml");

    // Loop through all FEB's, hybrid's, channel's and samples and write
    // a baseline and noise value to the XML document
    for(int feb_id =  0; feb_id < 10; ++feb_id){
        for(int feb_hybrid_id = 0; feb_hybrid_id < 4; ++feb_hybrid_id){
            for(int channel = 0; channel < 640; ++channel){
                for(int sample_n = 0; sample_n < 6; ++sample_n){
                    writer->writeBaseline(feb_id, feb_hybrid_id, channel, sample_n, channel); 
                    writer->writeNoise(feb_id, feb_hybrid_id, channel, sample_n, channel); 
                }
            }
        }
    }

    // Close the document
    writer->close();

    return 0; 	
}
