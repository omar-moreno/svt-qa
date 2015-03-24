
#include <ThresholdAnalysis.h>

ThresholdAnalysis::ThresholdAnalysis()
    : reader(new ThresholdReader()),
      total_raw_hit_counts(NULL),
      trigger_samples(new TriggerSample()),
      output_file(new TFile("threshold_results.root", "RECREATE")), 
      class_name("ThresholdAnalysis"), 
      feb_id(-1), hybrid_id(-1), hit_count(0), current_event(0),
      samples_above(3)
{ 
    for (int feb = 0; feb < 10; ++feb) {
        for (int hybrid = 0; hybrid < 4; ++hybrid) {
            this->addHistogram(feb, hybrid);
        }
    }
}

ThresholdAnalysis::ThresholdAnalysis(int feb_id) 
    : reader(new ThresholdReader()),
      total_raw_hit_counts(NULL),
      trigger_samples(new TriggerSample()),
      output_file(new TFile("threshold_results.root", "RECREATE")), 
      class_name("ThresholdAnalysis"), 
      feb_id(feb_id), hybrid_id(-1), hit_count(0), current_event(0),
      samples_above(3)
{ 
    for (int hybrid = 0; hybrid < 4; ++hybrid) { 
        this->addHistogram(feb_id, hybrid);
    }
}

ThresholdAnalysis::ThresholdAnalysis(int feb_id, int hybrid_id)
    : reader(new ThresholdReader()),
      total_raw_hit_counts(NULL),
      trigger_samples(new TriggerSample()),
      output_file(new TFile("threshold_results.root", "RECREATE")), 
      class_name("ThresholdAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id), hit_count(0), current_event(0),
      samples_above(3)
{
    this->addHistogram(feb_id, hybrid_id);    
}

ThresholdAnalysis::~ThresholdAnalysis() {}

void ThresholdAnalysis::initialize() { 
    
    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle(); 

    std::cout << "[ ThresholdAnalysis ]: Opening file: " << threshold_file << std::endl;
    reader->open(threshold_file); 
    
    total_raw_hit_counts = 
        new TH1S("raw_hit_counts", "raw_hit_counts", 23100, 0, 23100); 
    total_raw_hit_counts->GetXaxis()->SetTitle("SVT Hit Count");
    total_raw_hit_counts->SetTitle("");
}

void ThresholdAnalysis::processEvent(TriggerEvent* event) {
   
    if (current_event != event->getEventNumber()) { 
        current_event = event->getEventNumber();
        if (current_event != 1) { 
            total_raw_hit_counts->Fill(hit_count);
   
            std::unordered_map <int, std::vector <int>>::iterator hit_count_it = hit_counts.begin();
            for (hit_count_it; hit_count_it != hit_counts.end(); ++hit_count_it) { 
                for (int hybrid = 0; hybrid < hit_count_it->second.size(); ++hybrid) { 
                    hit_counts_map[hit_count_it->first][hybrid]->Fill(
                        hit_counts[hit_count_it->first][hybrid]);           
                } 
            }

        hit_count = 0;
        this->clearHitCounts();
        if(current_event%500 == 0) 
            cout << "[ ThresholdAnalysis ]: Processing event " << current_event << endl;
        }
    } 
   
    for (uint sample_set_n = 0; sample_set_n < event->count(); ++sample_set_n) {
        event->sample(sample_set_n, trigger_samples); 
        this->processSamples(trigger_samples);        
    }        
}

void ThresholdAnalysis::processSamples(TriggerSample* samples) {

    // If a FEB ID has been specified and it doesn't match the FEB ID 
    // associated with the sample, skip the event
    if ((feb_id != -1) && (samples->febAddress() != feb_id)) return;

    // If a hybrid ID has been specified and it doesn't match the hybrid ID
    // associated with the sample, skip the event 
    if ((hybrid_id != -1) && (samples->hybrid() != hybrid_id)) return;

    // If the sample is a header or a tail event, skip the event
    if(samples->head() || samples->tail()) return;
   
    //std::cout << "//=======================================//" << std::endl; 
    if (!passThresholdCut(samples, samples->febAddress(), samples->hybrid(), samples->apv(), samples->channel())) return;
    //std::cout << "//=======================================//" << std::endl; 

    //std::cout << "Pass cut!" << std::endl;
    ++hit_count;

    hit_counts[samples->febAddress()][samples->hybrid()]++;

    int channel = 0; 
    channel = QAUtils::getPhysicalChannel(samples->apv(), samples->channel()); 

    hit_counts_v_channel_map[samples->febAddress()][samples->hybrid()]->Fill(channel, 1); 
} 

void ThresholdAnalysis::finalize() {
  
    total_raw_hit_counts->Write();

    std::unordered_map <int, std::vector <TH1S*>>::iterator feb_it = hit_counts_map.begin();

    for (feb_it; feb_it != hit_counts_map.end(); ++feb_it) {
        for (int hybrid = 0; hybrid < feb_it->second.size(); ++hybrid) { 
    
            std::string file_name = "feb";
            if (feb_it->first < 10) file_name += "0";
            file_name += std::to_string(feb_it->first) + "_hybrid0" + std::to_string(hybrid);
            output_file->mkdir(file_name.c_str());
            output_file->cd(file_name.c_str());
  
        if (feb_it->first == 9 && hybrid >= 2) continue; 
        if (feb_it->first == 2 && hybrid >= 2) continue; 
        feb_it->second[hybrid]->Write(); 
        hit_counts_v_channel_map[feb_it->first][hybrid]->Write();
        }
    }   
    
   output_file->Close(); 
}

bool ThresholdAnalysis::passThresholdCut(TriggerSample* samples, int feb, int hybrid, int apv, int channel) {
   
    int threshold = reader->getThreshold(feb, hybrid, apv, channel);    

    int samples_above_count = 0;
    bool previous_sample_above = false;
    int consecutive_sample_count = 0;
    int max_consecutive_sample_count = 0;
    for (int sample_n = 0; sample_n < 6; sample_n++) { 
       // std::cout << "[ ThresoldAnalysis ]: Sample " << sample_n << " : " << samples->value(sample_n) 
        //          << " Threshold: " << threshold << std::endl; 

        if (samples->value(sample_n) > threshold) {
            samples_above_count++;
            if (previous_sample_above) { 
                consecutive_sample_count++;
            } else { 
                consecutive_sample_count = 1;
            }

            if( consecutive_sample_count > max_consecutive_sample_count) { 
                max_consecutive_sample_count = consecutive_sample_count;
            }
            previous_sample_above = true;
        }

        //std::cout << "[ ThresholdAnalysis ]: Samples Above: " << samples_above_count << std::endl;
       // std::cout << "[ ThresholdAnalysis ]: Max Consecutive Samples: " 
          //  << max_consecutive_sample_count << std::endl;
    }

    if (samples_above_count > 0 && consecutive_sample_count >= samples_above) {
        return true;
    } 

    return false;
}

void ThresholdAnalysis::clearHitCounts() { 

            std::unordered_map <int, std::vector <int>>::iterator hit_count_it 
                = hit_counts.begin();
            for (hit_count_it; hit_count_it != hit_counts.end(); ++hit_count_it) { 
                for (int hybrid = 0; hybrid < hit_count_it->second.size(); ++hybrid) {
                hit_counts[hit_count_it->first][hybrid] = 0;                
	    } 
    }
}

void ThresholdAnalysis::addHistogram(int feb_id, int hybrid_id) { 
    
    std::string plot_title = "FEB: " + std::to_string(feb_id) + 
                             " Hybrid: " + std::to_string(hybrid_id) + 
                             " - Hit Count";
    
    hit_counts_map[feb_id].push_back(new TH1S(plot_title.c_str(),
                                          plot_title.c_str(),
                                          700, 0, 700));         
    hit_counts_map[feb_id][hybrid_id]->GetXaxis()->SetTitle("Sensor Hit Count"); 
    hit_counts[feb_id].push_back(0);

    plot_title = "FEB: " + std::to_string(feb_id) + 
                             " Hybrid: " + std::to_string(hybrid_id) + 
                             " - Hit Count v Physical Channel";

    hit_counts_v_channel_map[feb_id].push_back(new TH2S(plot_title.c_str(),
							plot_title.c_str(), 
							640, 0, 640,
							20, 0, 20));
    hit_counts_v_channel_map[feb_id][hybrid_id]->GetXaxis()->SetTitle(""); 
    

    std::cout << "[ BaselineAnalysis ]: Created hit count histograms for FEB: "   
              << feb_id << " Hybrid: " << hybrid_id << std::endl;

}


std::string ThresholdAnalysis::toString() { 
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer;   
}
