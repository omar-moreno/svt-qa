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
      output_file(new TFile("calibration_results.root", "RECREATE")), 
      class_name("CalibrationAnalysis"), 
      feb_id(-1), hybrid_id(-1), calibration_group(0), current_event(0)
{
    // TODO: Abstract this out 
    for (int feb = 0; feb < 10; ++feb) {
        for (int hybrid = 0; hybrid < 4; ++hybrid) {
            this->bookHistogram(feb, hybrid); 
        }
    }
}

CalibrationAnalysis::CalibrationAnalysis(int feb_id)
    : trigger_samples(new TriggerSample()),
      output_file(new TFile("calibration_results.root", "RECREATE")), 
      class_name("CalibrationAnalysis"), 
      feb_id(feb_id), hybrid_id(-1), calibration_group(0), current_event(0)
{
    // TODO: Abstract this out 
    for (int hybrid = 0; hybrid < 4; ++hybrid) { 
        this->bookHistogram(feb_id, hybrid); 
    }
}

CalibrationAnalysis::CalibrationAnalysis(int feb_id, int hybrid_id)
    : trigger_samples(new TriggerSample()), 
      output_file(new TFile("calibration_results.root", "RECREATE")), 
      class_name("CalibrationAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id), calibration_group(0), current_event(0)
{
    // TODO: Abstract this out 
    this->bookHistogram(feb_id, hybrid_id); 
}

CalibrationAnalysis::~CalibrationAnalysis() {}

void CalibrationAnalysis::initialize() { 

    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle();
}

void CalibrationAnalysis::processEvent(TriggerEvent* event) { 
  
    if (current_event != event->getEventNumber()) { 
        current_event = event->getEventNumber();
        if (current_event != 1) { 
        
            if(current_event%500 == 0) 
            	std::cout << "[ CalibrationAnalysis ]: Processing event " << current_event << std::endl;
        
	    /*if (current_event%3000 == 0) { 
		std::cout << "[ CalibrationAnalysis ]: Incrementing calibration group " << std::endl;
	    	calibration_group++;
            }*/
	}
    }

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

	/*
    std::cout << "APV channel: " << samples->channel() << std::endl; 
    std::cout << "APV: " << samples->apv() << std::endl;
	*/

    int channel = (samples->apv()*128) + samples->channel(); 
    // Only process channels which are part of the current calibration group
    if ((channel - calibration_group)%8 == 0) {

    	// Get the physical channel number
    	channel = QAUtils::getPhysicalChannel(samples->apv(), samples->channel());
    	//std::cout << "Channel: " << channel << std::endl; 
    
    	// For now, use the first sample as the pedesal
    	int pedestal = samples->value(0);   
        
    	for (int sample_n = 0; sample_n < 6; ++sample_n) {
		int sample = samples->value(sample_n) - pedestal;
		samples_map[samples->febAddress()][samples->hybrid()][channel]->Fill(sample_n*25, 
					samples->value(sample_n) - pedestal); 
   	 }
    }
}

void CalibrationAnalysis::finalize() {

    std::unordered_map <int, std::vector < std::vector<TH2S* >>>::iterator feb_it 
        = samples_map.begin();

    for (feb_it; feb_it != samples_map.end(); ++feb_it) { 
        for (int hybrid = 0; hybrid < feb_it->second.size(); ++hybrid) { 
	        if (feb_it->first == 9 && hybrid >= 2) continue; 
	        if (feb_it->first == 2 && hybrid >= 2) continue; 
            
            std::string file_name = "feb";
            if (feb_it->first < 10) file_name += "0";
            file_name 
                += std::to_string(feb_it->first) + "_hybrid0" + std::to_string(hybrid);
            
            output_file->mkdir(file_name.c_str());
            output_file->cd(file_name.c_str());
            
            for (int channel = 0; channel < feb_it->second[hybrid].size(); 
                    ++channel) {

               feb_it->second[hybrid][channel]->Write(); 
            }    
        }
    }

    output_file->Close(); 
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
                             " Hybrid: " + std::to_string(hybrid_id);   
    
    std::vector <TH2S*> samples_plot_vector;
    for (int channel = 0; channel < 640; ++channel) {
	std::string channel_plot_title = plot_title + " Channel: " + std::to_string(channel) + " Samples";
        samples_plot_vector.push_back( new TH2S(channel_plot_title.c_str(), 
                                           channel_plot_title.c_str(), 
                                           6, 0, 150, 
                                           1200, -400, 5000));
        samples_plot_vector[channel]->GetXaxis()->SetTitle("time [ns]"); 
        samples_plot_vector[channel]->GetYaxis()->SetTitle("Pedestal Subtracted Amplitude [ADC Counts]");

    } 
    samples_map[feb_id].push_back(samples_plot_vector);
    std::cout << "[ CalibrationAnalysis ]: Created samples histogram for FEB " 
        << feb_id << " Hybrid: " << hybrid_id << std::endl;
}


