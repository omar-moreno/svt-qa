/**
 *  @file   SimpleBaselineAnalysis.cxx
 *  @brief  Analysis class used to extract baseline and noise values for all
 *          SVT channels.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   February 19, 2015
 *
 */

#include <SimpleBaselineAnalysis.h>

SimpleBaselineAnalysis::SimpleBaselineAnalysis()
   : output_file(new TFile("calibration_results.root", "RECREATE")), 
     gaussian(new TF1("gaussian", "gaus")), 
     writer(new ThresholdWriter()),
     class_name("SimpleBaselineAnalysis"), 
     feb_id(-1), hybrid_id(-1), readout_order(false)
{

    for (int feb = 0; feb < 10; ++feb) {
        for (int hybrid = 0; hybrid < 4; ++hybrid) {
            this->addBaselineHistogram(feb, hybrid); 
        }
    }
}

SimpleBaselineAnalysis::SimpleBaselineAnalysis(int feb_id)
    : output_file(new TFile("calibration_results.root", "RECREATE")),  
      gaussian(new TF1("gaussian", "gaus")), 
      writer(new ThresholdWriter()),
      class_name("SimpleBaselineAnalysis"), 
      feb_id(feb_id), hybrid_id(-1), readout_order(false) 
{
    for (int hybrid = 0; hybrid < 4; ++hybrid) { 
        this->addBaselineHistogram(feb_id, hybrid); 
    }
}

SimpleBaselineAnalysis::SimpleBaselineAnalysis(int feb_id, int hybrid_id)
    : output_file(new TFile("calibration_results.root", "RECREATE")),  
      gaussian(new TF1("gaussian", "gaus")), 
      writer(new ThresholdWriter()),
      class_name("SimpleBaselineAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id), readout_order(false) 
{
    this->addBaselineHistogram(feb_id, hybrid_id);
}

SimpleBaselineAnalysis::~SimpleBaselineAnalysis() {
    baseline_map.clear(); 
    delete output_file;  
    delete writer;  
}

void SimpleBaselineAnalysis::initialize() {
    
    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle(); 
    
    writer->open("test.xml");

    for(int n = 0; n < 128; n++){
        int channel = (32*(n%4)) + (8*(n/4)) - (31*(n/16));
        channel_map[channel] = n; 
    }
}


void SimpleBaselineAnalysis::processEvent(TriggerSample* samples) {

    // If a FEB ID has been specified and it doesn't match the FEB ID 
    // associated with the sample, skip the event
    if ((feb_id != -1) && (samples->febAddress() != feb_id)) return;

    //std::cout << "Feb ID: " << feb_id << std::endl;

    // If a hybrid ID has been specified and it doesn't match the hybrid ID
    // associated with the sample, skip the event 
    if ((hybrid_id != -1) && (samples->hybrid() != hybrid_id)) return;
    
    //std::cout << "Hybrid ID: " << hybrid_id << std::endl;

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
        baseline_map[samples->febAddress()][samples->hybrid()]->Fill(channel, samples->value(sample_n));
    } 
}

void SimpleBaselineAnalysis::readoutOrder(bool readout_order) {
    this->readout_order = readout_order; 
}

void SimpleBaselineAnalysis::finalize() {

    std::unordered_map <int, std::vector <TH2S*>>::iterator feb_it = baseline_map.begin();

    for (feb_it; feb_it != baseline_map.end(); ++feb_it) {
        for (int hybrid = 0; hybrid < feb_it->second.size(); ++hybrid) { 
            this->processBaselinePlot(feb_it->first, hybrid, feb_it->second[hybrid]); 
        }
    }   
    
    writer->close();
    output_file->Close();
}


void SimpleBaselineAnalysis::setThreshold(const double threshold) { 
    writer->setThreshold(threshold); 
}

std::string SimpleBaselineAnalysis::toString() {
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer;   
}

void SimpleBaselineAnalysis::addBaselineHistogram(int feb_id, int hybrid_id) { 

    std::string plot_title = "FEB: " + std::to_string(feb_id) + 
                             " Hybrid: " + std::to_string(hybrid_id) + 
                             " Baseline";
    
    baseline_map[feb_id].push_back(new TH2S(plot_title.c_str(), 
                                      plot_title.c_str(), 
                                      640, 0, 640,  // bins in x 
                                      16383, 0, 16383)); // bins in y
    baseline_map[feb_id][hybrid_id]->GetXaxis()->SetTitle("Channel");
    baseline_map[feb_id][hybrid_id]->GetYaxis()->SetTitle("Baseline [ADC Counts]");
    
    std::cout << "[ BaselineAnalysis ]: Created baseline histogram for FEB: "   
              << feb_id << " Hybrid: " << hybrid_id << std::endl;
} 

void SimpleBaselineAnalysis::processBaselinePlot(int feb, int hybrid, TH2S* baseline_plot) {

    if (baseline_plot->GetEntries() == 0) return; 

    TH1D* projection = NULL;

    std::string file_name = "feb";
    if (feb < 10) file_name += "0";
    file_name += std::to_string(feb) + "_hybrid0" + std::to_string(hybrid);
    if (readout_order) file_name += "_readout_order"; 
    output_file->mkdir(file_name.c_str());
    output_file->cd(file_name.c_str());

    double mean_baseline =0;
    double mean_baseline_error = 0; 
    double noise = 0;  
    
    TGraphErrors* g_mean_baseline = new TGraphErrors(640);
    g_mean_baseline->SetMarkerColor(2);
    g_mean_baseline->SetLineColor(2);
    
    TGraph* g_noise = new TGraphErrors(640);    
    g_noise->SetMarkerColor(2);
    g_noise->SetLineColor(2);
   
    PlottingUtils::adjust2DPlotRange(baseline_plot, 2);
    
    for (int channel = 0; channel < 640; ++channel) {
    
        //this->getSimpleCalibrations(
        //        baseline_sample_plot->ProjectionY("", channel+1, channel+1),
        //        mean_baseline, mean_baseline_error, noise);
        this->getCalibrations(
                baseline_plot->ProjectionY("", channel+1, channel+1),
                mean_baseline, mean_baseline_error, noise);
        g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
        g_mean_baseline->SetPointError(channel, 0, mean_baseline_error); 
        g_noise->SetPoint(channel, channel, noise); 
        writer->writeThreshold(feb, hybrid, channel, mean_baseline, noise);
    }

    g_mean_baseline->Draw("a"); 
    g_mean_baseline->GetXaxis()->SetTitle("Channel");
    g_mean_baseline->GetYaxis()->SetTitle("Mean Baseline [ADC Counts]");
    g_mean_baseline->Write();
    g_noise->Draw("a");
    g_noise->GetXaxis()->SetTitle("Channel");
    g_noise->GetYaxis()->SetTitle("Noise [ADC Counts]");
    g_noise->Write();

    output_file->cd();

    delete g_mean_baseline; 
    delete g_noise;
}

void SimpleBaselineAnalysis::getSimpleCalibrations(TH1D* baseline_histogram, 
                                             double &mean_baseline, 
                                             double &mean_baseline_error,
                                             double &noise) { 
    
    mean_baseline = baseline_histogram->GetMean();
    mean_baseline_error = baseline_histogram->GetMeanError();
    noise = baseline_histogram->GetRMS();
}


void SimpleBaselineAnalysis::getCalibrations(TH1D* baseline_histogram, 
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


