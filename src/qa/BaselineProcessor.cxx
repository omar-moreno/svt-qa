/**
 *  @file BaselineProcessor.h
 *  @brief Processor that create a tuple out of an SVT baseline run. 
 *  @author Omar Moreno, SLAC National Accelerator Laboratory
 *
 */

#include <BaselineProcessor.h>

BaselineProcessor::BaselineProcessor(const std::string& name, Process& process)
    : Processor(name, process) {
}

BaselineProcessor::~BaselineProcessor() {
}

void BaselineProcessor::initialize() {

    //PlottingUtils::setPalette(); 
    //PlottingUtils::setStyle(); 
    
    std::string name{""}; 
    for (int isample{0}; isample < 6; ++isample) { 
        name = "Baseline - Hybrid 0 - Sample: " + std::to_string(isample); 
        plots.push_back(new TH2S(name.c_str(), name.c_str(), 
                        640, 0, 640, 16384, 0, 16384)); 
    }

    
    // Create the readout channel map
    for(int n = 0; n < 128; n++){
        int channel = (32*(n%4)) + (8*(n/4)) - (31*(n/16));
        //std::cout << "Channel: " << n << std::endl;
        channel_map[channel] = n; 
    }
}

void BaselineProcessor::process(TrackerEvent* event) {

    /*std::cout << "[ BaselineProcessor ]: Event: " << _event_counter
              << std::endl;
    std::cout << "[ BaselineProcessor ]: Total samples: "  << event->count()
              << std::endl; */
   
    // Loop through all of the collected sets of samples.  Here event
    // count returns the number of sample collected in an event.  In our
    // case, this is equal to the number of channels.
    for (int sample_set = 0; sample_set < event->count(); ++sample_set) {
        
        // Get the set of samples from a channel.
        samples_ = event->sample(sample_set);
    
        //this->printSamples(samples_); 

        // Get the physical channel number 
        int pchannel = QAUtils::getPhysicalChannel(samples_->apv(), samples_->channel());

        // Get the channel number in readout order
        int rchannel = channel_map[samples_->channel()] + samples_->apv()*128; 

        for (int isample{0}; isample < 6; ++isample) {
            plots[isample]->Fill(pchannel, samples_->value(isample)); 
        } 
    }
}

void BaselineProcessor::printSamples(TrackerSample* samples) { 

    std::cout << "[ BaselineProcessor ]: \n"  
                  << "Hybrid: " << samples->hybrid() << "\n" 
                  << "APV: " << samples->apv() << "\n" 
                  << "Channel: " << samples->channel() << "\n" 
                  << "Samples: [ " 
                  << samples->value(0) << ", " 
                  << samples->value(1) << ", " 
                  << samples->value(2) << ", " 
                  << samples->value(3) << ", " 
                  << samples->value(4) << ", " 
                  << samples->value(5) << " ] " 
                  << std::endl;
}


void BaselineProcessor::finalize() {

    // Create a canvas
    TCanvas* canvas{new TCanvas{"canvas", "canvas", 800, 800}}; 

    //
    TGraph* g_mean_baseline{nullptr};
    TGraph* g_noise{nullptr};  
    std::string title{""};

    for (int isample = 0; isample < 6; ++isample) {
        
        title = "Hybrid: 0 - Mean Baseline - Sample " + std::to_string(isample);
        g_mean_baseline = new TGraphErrors(640);
        g_mean_baseline->SetNameTitle(title.c_str(), title.c_str());


        title = "Hybrid: 0 - Noise - Sample " + std::to_string(isample);
        g_noise = new TGraphErrors(640);    
        g_noise->SetNameTitle(title.c_str(), title.c_str());
        

        TH2S* baseline_sample_plot = plots[isample];
        
        
        double mean_baseline, mean_baseline_error, noise; 
        for (int channel = 0; channel < 640; ++channel) {
       
             
            TH1D* proj = baseline_sample_plot->ProjectionY("", channel+1, channel+1);
            if (proj->GetEntries() == 0) continue; 
            this->getCalibrations(proj, mean_baseline, mean_baseline_error, noise);
            proj->Write();
            g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
            g_noise->SetPoint(channel, channel, noise); 
            delete proj; 
        
        }

        g_mean_baseline->Draw("ap"); 
        g_mean_baseline->GetXaxis()->SetTitle("Channel"); 
        g_mean_baseline->GetYaxis()->SetTitle("Mean Baseline (ADC Counts)");
        g_mean_baseline->Write();
        
        g_noise->Draw("ap"); 
        g_noise->GetXaxis()->SetTitle("Channel"); 
        g_noise->GetYaxis()->SetTitle("Noise (ADC Counts)"); 
        g_noise->Write();
        
        delete g_mean_baseline; 
        delete g_noise;

    } 
    delete canvas; 
}

void BaselineProcessor::getSimpleCalibrations(TH1D* baseline_histogram, double &mean_baseline, 
                                             double &mean_baseline_error, double &noise) { 

    mean_baseline = baseline_histogram->GetMean();
    mean_baseline_error = baseline_histogram->GetMeanError();
    noise = baseline_histogram->GetRMS();
}


void BaselineProcessor::getCalibrations(TH1D* baseline_histogram, double &mean_baseline, double &mean_baseline_error,
                                       double &noise) {

    this->getSimpleCalibrations(baseline_histogram, mean_baseline, mean_baseline_error, noise);
    gaussian->SetRange(mean_baseline - 3*noise, mean_baseline + 3*noise); 
    baseline_histogram->Fit("gaussian", "RQ");
    mean_baseline = gaussian->GetParameter(1); 
    noise = gaussian->GetParameter(2);
}

DECLARE_PROCESSOR(BaselineProcessor);
