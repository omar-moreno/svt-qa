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
    : trigger_samples(new TriggerSample()),
      class_name("CalibrationAnalysis"), 
      feb_id(-1), hybrid_id(-1), calibration_group(0)
{
    for (int feb = 0; feb < 10; ++feb) {
        for (int hybrid = 0; hybrid < 4; ++hybrid) {
        }
    }
}

CalibrationAnalysis::CalibrationAnalysis(int feb_id)
    : trigger_samples(new TriggerSample()),
      class_name("CalibrationAnalysis"), 
      feb_id(feb_id), hybrid_id(-1), calibration_group(0)
{
    for (int hybrid = 0; hybrid < 4; ++hybrid) { 
    }
}

CalibrationAnalysis::CalibrationAnalysis(int feb_id, int hybrid_id)
    : trigger_samples(new TriggerSample()), 
      class_name("CalibrationAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id), calibration_group(0)
{
}

CalibrationAnalysis::~CalibrationAnalysis() {}

void CalibrationAnalysis::initialize() { 

    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle();
}

void CalibrationAnalysis::processEvent(TriggerEvent* event) { 
    
    for (uint sample_set_n = 0; sample_set_n < event->count(); ++sample_set_n) {
        event->sample(sample_set_n, trigger_samples); 
        this->processSamples(trigger_samples);        
    }        
}

void CalibrationAnalysis::processSamples(TriggerSample* samples) { 
    
    // If a FEB ID has been specified and it doesn't match the FEB ID 
    // associated with the sample, skip the event
    if ((feb_id != -1) && (samples->febAddress() != feb_id)) return;

    // If a hybrid ID has been specified and it doesn't match the hybrid ID
    // associated with the sample, skip the event 
    if ((hybrid_id != -1) && (samples->hybrid() != hybrid_id)) return;

    // If the sample is a header or a tail event, skip the event
    if(samples->head() || samples->tail()) return;


    // Only process channels which are part of the current calibration group
    if (!(samples->channel() - calibration_group)%8 == 0) return;  

    // Get the physical channel number
	int channel = QAUtils::getPhysicalChannel(samples->apv(), samples->channel()); 
    
    // For now, use the first sample as the pedesal
    int pedestal = samples->value(0);   
        
    for (int sample_n = 0; sample_n < 6; ++sample_n) {
        samples_map[feb_id][hybrid_id][channel]->Fill(sample_n*25,
                samples->value(sample_n) - pedestal); 
    }
}

void CalibrationAnalysis::finalize() {

}

std::string CalibrationAnalysis::toString() { 
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer; 
}


bool CalibrationAnalysis::passSlopeCut(TriggerSample* samples) { 
    
    for (int sample_n = 0; sample_n < 6; ++sample_n) { 
        if (samples->value(sample_n + 1) > samples->value(sample_n)) return true;
    }

    return false; 
}

void CalibrationAnalysis::bookHistogram(int feb_id, int hybrid_id) { 

    std::string plot_title = "FEB: " + std::to_string(feb_id) + 
                             " Hybrid: " + std::to_string(hybrid_id) + 
                             " Samples";
   
    std::vector <TH2S*> samples_plot_vector;
    for (int channel = 0; channel < 640; ++channel) { 
        samples_plot_vector.push_back( new TH2S(plot_title.c_str(), 
                                           plot_title.c_str(), 
                                           6, 0, 150, 
                                           1200, -400, 2000)); 
        samples_plot_vector[channel]->GetXaxis()->SetTitle("time [ns]"); 
        samples_plot_vector[channel]->GetYaxis()->SetTitle("Pedestal Subtracted Amplitude [ADC Counts]");

    } 
    samples_map[feb_id].push_back(samples_plot_vector);
    std::cout << "[ CalibrationAnalysis ]: Created samples histogram for FEB " 
        << feb_id << " Hybrid: " << hybrid_id << std::endl;
}


