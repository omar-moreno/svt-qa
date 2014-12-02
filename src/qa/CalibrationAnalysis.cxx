/**
 *  @file   CalibrationAnalysis.cxx
 *  @brief  Analysis class used to fit calibration pulses and extract
 *          parameters.
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 *  @date   October 01, 2014
 *
 */

#include <CalibrationAnalysis.h>

CalibrationAnalysis::CalibrationAnalysis() 
    : class_name("CalibrationAnalysis"), 
      feb_id(-1), hybrid_id(-1)
{}

CalibrationAnalysis::CalibrationAnalysis(int feb_id, int hybrid_id)
    : class_name("CalibrationAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id)
{}

CalibrationAnalysis::~CalibrationAnalysis() {}

void CalibrationAnalysis::initialize() { 

    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle();
}

void CalibrationAnalysis::processEvent(TriggerSample* samples) { 

    if(samples->head() || samples->tail()) return;

}

void CalibrationAnalysis::finalize() {}

std::string CalibrationAnalysis::toString() { 
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer; 
}
