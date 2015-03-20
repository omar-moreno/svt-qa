/**
 *  @file   OccupancyAnalysis.cxx
 *  @brief  Analysis class used to study the SVT occupancies.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   March 16, 2014
 */

#include <OccupancyAnalysis.h>

OccupancyAnalysis::OccupancyAnalysis()
    : total_raw_hit_counts(NULL),
      trigger_samples(new TriggerSample()),
      output_file(new TFile("occupancy_results.root", "RECREATE")), 
      class_name("OccupancyAnalysis"), 
      feb_id(-1), hybrid_id(-1), hit_count(0), current_event(0)
{ 
    for (int feb = 0; feb < 10; ++feb) {
        for (int hybrid = 0; hybrid < 4; ++hybrid) {
            this->addOccupancyHistogram(feb, hybrid); 
        }
    }
}

OccupancyAnalysis::OccupancyAnalysis(int feb_id) 
    : total_raw_hit_counts(NULL),
      trigger_samples(new TriggerSample()),
      output_file(new TFile("occupancy_results.root", "RECREATE")), 
      class_name("OccupancyAnalysis"), 
      feb_id(feb_id), hybrid_id(-1), hit_count(0), current_event(0)
{ 
    for (int hybrid = 0; hybrid < 4; ++hybrid) { 
        this->addOccupancyHistogram(feb_id, hybrid); 
    }
}

OccupancyAnalysis::OccupancyAnalysis(int feb_id, int hybrid_id)
    : total_raw_hit_counts(NULL),
      trigger_samples(new TriggerSample()),
      output_file(new TFile("occupancy_results.root", "RECREATE")), 
      class_name("OccupancyAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id), hit_count(0), current_event(0)
{
    this->addOccupancyHistogram(feb_id, hybrid_id); 
}

OccupancyAnalysis::~OccupancyAnalysis() {
    //delete output_file;
    //delete total_raw_hit_counts;   
}

void OccupancyAnalysis::initialize() {
    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle(); 
            
    total_raw_hit_counts = 
        new TH1S("raw_hit_counts", "raw_hit_counts", 23100, 0, 23100); 

}

void OccupancyAnalysis::processEvent(TriggerEvent* event) {
   
    if (current_event != event->getEventNumber()) { 
        current_event = event->getEventNumber();
        if (current_event != 1) { 
            total_raw_hit_counts->Fill(hit_count);
            //cout << "[ OccupancyAnalsis ]: Hit Counts: " << hit_count << endl; 
        
   
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
            cout << "[ OccupancyAnalysis ]: Processing event " << current_event << endl;
        }
    } 
    
    for (uint sample_set_n = 0; sample_set_n < event->count(); ++sample_set_n) {
        event->sample(sample_set_n, trigger_samples); 
        this->processSamples(trigger_samples);        
    }        
}

void OccupancyAnalysis::processSamples(TriggerSample* samples) {

    // If a FEB ID has been specified and it doesn't match the FEB ID 
    // associated with the sample, skip the event
    if ((feb_id != -1) && (samples->febAddress() != feb_id)) return;

    // If a hybrid ID has been specified and it doesn't match the hybrid ID
    // associated with the sample, skip the event 
    if ((hybrid_id != -1) && (samples->hybrid() != hybrid_id)) return;

    // If the sample is a header or a tail event, skip the event
    if(samples->head() || samples->tail()) return;
    
    ++hit_count;

    hit_counts[samples->febAddress()][samples->hybrid()]++;
} 

void OccupancyAnalysis::finalize() {
  
    total_raw_hit_counts->Write();

    std::unordered_map <int, std::vector <TH1S*>>::iterator feb_it = hit_counts_map.begin();

    for (feb_it; feb_it != hit_counts_map.end(); ++feb_it) {
        for (int hybrid = 0; hybrid < feb_it->second.size(); ++hybrid) { 
    
            std::string file_name = "feb";
            if (feb_it->first < 10) file_name += "0";
            file_name += std::to_string(feb_it->first) + "_hybrid0" + std::to_string(hybrid);
            output_file->mkdir(file_name.c_str());
            output_file->cd(file_name.c_str());
   
            feb_it->second[hybrid]->Write(); 
        }
    }   
    
   output_file->Close(); 
}

std::string OccupancyAnalysis::toString() { 
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer;   
}

void OccupancyAnalysis::addOccupancyHistogram(int feb_id, int hybrid_id) { 
    
    std::string plot_title = "FEB: " + std::to_string(feb_id) + 
                             " Hybrid: " + std::to_string(hybrid_id) + 
                             " Hit Count";
    
    hit_counts_map[feb_id].push_back(new TH1S(plot_title.c_str(),
                                          plot_title.c_str(),
                                          700, 0, 700)); 
            
    hit_counts_map[feb_id][hybrid_id]->GetXaxis()->SetTitle("Hit Count");
    
    hit_counts[feb_id].push_back(0);

    std::cout << "[ BaselineAnalysis ]: Created hit count histograms for FEB: "   
              << feb_id << " Hybrid: " << hybrid_id << std::endl;
}

void OccupancyAnalysis::clearHitCounts() { 

            std::unordered_map <int, std::vector <int>>::iterator hit_count_it 
                = hit_counts.begin();
            for (hit_count_it; hit_count_it != hit_counts.end(); ++hit_count_it) { 
                for (int hybrid = 0; hybrid < hit_count_it->second.size(); ++hybrid) {
                hit_counts[hit_count_it->first][hybrid] = 0;                
            } 
    }
}


