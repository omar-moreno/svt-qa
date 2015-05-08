/**
 *
 *  @author: Omar Moreno <omoreno1@ucsc.edu>
 *           Santa Cruz Institute for Particle Physics
 *           University of California, Santa Cruz
 *  @date:   May 5, 2014
 *
 */

#include <BaselineAnalysis.h>

BaselineAnalysis::BaselineAnalysis()
   : trigger_samples(new TriggerSample()),
     output_file(new TFile("calibration_results.root", "RECREATE")), 
     gaussian(new TF1("gaussian", "gaus")), 
     writer(new CalibrationWriter()),
     class_name("BaselineAnalysis"), 
     feb_id(-1), hybrid_id(-1), readout_order(false),
     current_event(0) 
{

    for (int feb = 0; feb < 10; ++feb) {
        for (int hybrid = 0; hybrid < 4; ++hybrid) {
            this->addBaselineHistogram(feb, hybrid); 
        }
    }
}

BaselineAnalysis::BaselineAnalysis(int feb_id)
    : trigger_samples(new TriggerSample()),
      output_file(new TFile("calibration_results.root", "RECREATE")),  
      gaussian(new TF1("gaussian", "gaus")), 
      writer(new CalibrationWriter()),
      class_name("BaselineAnalysis"), 
      feb_id(feb_id), hybrid_id(-1), readout_order(false), 
     current_event(0) 
{
    for (int hybrid = 0; hybrid < 4; ++hybrid) { 
        this->addBaselineHistogram(feb_id, hybrid); 
    }
}

BaselineAnalysis::BaselineAnalysis(int feb_id, int hybrid_id)
    : trigger_samples(new TriggerSample()),
      output_file(new TFile("calibration_results.root", "RECREATE")),  
      gaussian(new TF1("gaussian", "gaus")), 
      writer(new CalibrationWriter()),
      class_name("BaselineAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id), readout_order(false), 
      current_event(0) 
{
    this->addBaselineHistogram(feb_id, hybrid_id);
}

BaselineAnalysis::~BaselineAnalysis() {
    delete output_file;  
    delete writer;  
}

void BaselineAnalysis::addBaselineHistogram(int feb_id, int hybrid_id) { 
        
    std::string plot_title = "FEB: " + std::to_string(feb_id) + 
                             " Hybrid: " + std::to_string(hybrid_id) + 
                             " Baseline";

    baseline_map[feb_id].push_back(new SamplesPlot(6, plot_title));
    baseline_map[feb_id][hybrid_id]->setXAxisTitles("Channel");
    baseline_map[feb_id][hybrid_id]->setYAxisTitles("Baseline [ADC Counts]");
    
    std::cout << "[ BaselineAnalysis ]: Created baseline histogram for FEB: "   
              << feb_id << " Hybrid: " << hybrid_id << std::endl;
}

void BaselineAnalysis::initialize() {
    
    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle(); 
    
    writer->open("test.xml");

	for(int n = 0; n < 128; n++){
		int channel = (32*(n%4)) + (8*(n/4)) - (31*(n/16));
		channel_map[channel] = n; 
    }

}

void BaselineAnalysis::processEvent(TriggerEvent* event) {

    if (current_event != event->getEventNumber()) { 
        current_event = event->getEventNumber();
        if (current_event != 1) { 
        
            if(current_event%500 == 0) 
            cout << "[ BaselineAnalysis ]: Processing event " << current_event << endl;
        }
    }


    for (uint sample_set_n = 0; sample_set_n < event->count(); ++sample_set_n) {
        event->sample(sample_set_n, trigger_samples); 
        this->processSamples(trigger_samples);        
    }        
}

void BaselineAnalysis::processSamples(TriggerSample* samples) {

    // If a FEB ID has been specified and it doesn't match the FEB ID 
    // associated with the sample, skip the event
    if ((feb_id != -1) && (samples->febAddress() != feb_id)) return;

    // If a hybrid ID has been specified and it doesn't match the hybrid ID
    // associated with the sample, skip the event 
    if ((hybrid_id != -1) && (samples->hybrid() != hybrid_id)) return;

    // If the sample is a header or a tail event, skip the event
    if(samples->head() || samples->tail()) return;

	// If the readout order flag has been set, get the readout order number
	// corresponding to the APV25 channel number.  Else, get the physical 
	// channel number.
	int channel = 0; 
	if (readout_order) {
		channel = channel_map[samples->channel()] + samples->apv()*128; 
	} else { 
		channel = QAUtils::getPhysicalChannel(samples->apv(), samples->channel()); 
	}

    for (int sample_n = 0; sample_n < 6; ++sample_n) {
        baseline_map[samples->febAddress()][samples->hybrid()]->fill(sample_n, channel, samples->value(sample_n));
    } 
}

void BaselineAnalysis::readoutOrder(bool readout_order) {
	this->readout_order = readout_order; 
}


void BaselineAnalysis::finalize() {

    std::unordered_map <int, std::vector <SamplesPlot*>>::iterator feb_it = baseline_map.begin();

    for (feb_it; feb_it != baseline_map.end(); ++feb_it) {
        for (int hybrid = 0; hybrid < feb_it->second.size(); ++hybrid) { 
            this->processBaselinePlot(feb_it->first, hybrid, feb_it->second[hybrid]); 
        }
    }
    
    writer->close();
    output_file->Close();
}

void BaselineAnalysis::processBaselinePlot(int feb, int hybrid, SamplesPlot* baseline_plot) {

    TH1D* projection = NULL;

    std::string file_name = "feb";
    if (feb < 10) file_name += "0";
    file_name += std::to_string(feb) + "_hybrid0" + std::to_string(hybrid);
	if (readout_order) file_name += "_readout_order"; 
    output_file->mkdir(file_name.c_str());
    output_file->cd(file_name.c_str());

    baseline_plot->getSumOfPlots()->Write(); 
   
    double mean_baseline =0;
    double mean_baseline_error = 0; 
    double noise = 0;
    TGraph* g_mean_baseline = NULL;
    TGraph* g_noise = NULL;  
    std::string plot_title = "";
    for (int sample_n = 0; sample_n < 6; ++sample_n) { 
    
        plot_title = "FEB: " + std::to_string(feb) 
                    + " Hybrid: " + std::to_string(hybrid) 
                    + " Mean Baseline - Sample " + std::to_string(sample_n);
        g_mean_baseline = new TGraphErrors(640);
        g_mean_baseline->SetNameTitle(plot_title.c_str(), plot_title.c_str());

        
        plot_title = "FEB: " + std::to_string(feb) 
                + " Hybrid: " + std::to_string(hybrid) 
                + " Noise - Sample " + std::to_string(sample_n);
        g_noise = new TGraphErrors(640);    
        g_noise->SetNameTitle(plot_title.c_str(), plot_title.c_str());
       
        
        TH2S* baseline_sample_plot = baseline_plot->getPlot(sample_n);
        baseline_sample_plot->Write();
        for (int channel = 0; channel < 640; ++channel) {

            if (baseline_sample_plot->ProjectionY("", channel+1, channel+1)->GetEntries() == 0) continue; 
            this->getCalibrations(
                    baseline_sample_plot->ProjectionY("", channel+1, channel+1),
                    mean_baseline, mean_baseline_error, noise);
            g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
            g_noise->SetPoint(channel, channel, noise); 
            writer->writeBaseline(feb, hybrid, channel, sample_n, mean_baseline);
            writer->writeNoise(feb, hybrid, channel, sample_n, noise);
        }
         
        g_mean_baseline->Write();
        g_noise->Write();
        delete g_mean_baseline; 
        delete g_noise; 
    }

    output_file->cd();
}

void BaselineAnalysis::getCalibrations(TH1D* baseline_histogram, 
                                       double &mean_baseline, 
                                       double &mean_baseline_error,
                                       double &noise) {

    this->getSimpleCalibrations(baseline_histogram, 
                                mean_baseline, mean_baseline_error, noise);
    gaussian->SetRange(mean_baseline - 3*noise, mean_baseline + 3*noise); 
    baseline_histogram->Fit("gaussian", "RQ");
    mean_baseline = gaussian->GetParameter(1); 
    noise = gaussian->GetParameter(2);
}

void BaselineAnalysis::getSimpleCalibrations(TH1D* baseline_histogram, 
                                             double &mean_baseline, 
                                             double &mean_baseline_error,
                                             double &noise) { 
    
    mean_baseline = baseline_histogram->GetMean();
    mean_baseline_error = baseline_histogram->GetMeanError();
    noise = baseline_histogram->GetRMS();
}

std::string BaselineAnalysis::toString() {
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer;   
}
