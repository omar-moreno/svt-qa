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
   : output_file(new TFile("calibration_results.root", "RECREATE")), 
     gaussian(new TF1("gaussian", "gaus")), 
     //writer(new CalibrationWriter()),
     writer(new ThresholdWriter()),
     class_name("BaselineAnalysis"), 
     feb_id(-1), hybrid_id(-1), readout_order(false)
{}

BaselineAnalysis::BaselineAnalysis(int feb_id)
    : output_file(new TFile("calibration_results.root", "RECREATE")),  
      gaussian(new TF1("gaussian", "gaus")), 
      //writer(new CalibrationWriter()),
      writer(new ThresholdWriter()),
      class_name("BaselineAnalysis"), 
      feb_id(feb_id), hybrid_id(-1), readout_order(false) 
{}

BaselineAnalysis::BaselineAnalysis(int feb_id, int hybrid_id)
    : output_file(new TFile("calibration_results.root", "RECREATE")),  
      gaussian(new TF1("gaussian", "gaus")), 
      //writer(new CalibrationWriter()),
      writer(new ThresholdWriter()),
      class_name("BaselineAnalysis"), 
      feb_id(feb_id), hybrid_id(hybrid_id), readout_order(false) 
{}

BaselineAnalysis::~BaselineAnalysis() {
    delete output_file;  
    delete writer;  
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

void BaselineAnalysis::processEvent(TriggerSample* samples) {

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

    // Create a DAQ pair
    std::pair <int, int> daq_pair = std::make_pair(samples->febAddress(), samples->hybrid()); 
    
    // Check if the baseline map already contains the DAQ pair key. If it 
    // doesn't, create a histogram to contain the baseline for that
    // FEB and hybrid.
    if (baseline_map.find(daq_pair) == baseline_map.end()) {
        
        std::string plot_title = "FEB: " + std::to_string(samples->febAddress()) + 
                                 " Hybrid: " + std::to_string(samples->hybrid()) + 
                                 " Baseline"; 
        baseline_map[daq_pair] = new SamplesPlot(6, plot_title);
        baseline_map.at(daq_pair)->setXAxisTitles("Channel");
        baseline_map.at(daq_pair)->setYAxisTitles("Baseline [ADC Counts]");
    
        std::cout << "[ BaselineAnalysis ]: Created baseline histogram for FEB: "   
                  << samples->febAddress() << " Hybrid: " << samples->hybrid()
                  << std::endl;
    }
    SamplesPlot* baseline_plot = baseline_map.at(daq_pair); 
	
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
        baseline_plot->fill(sample_n, channel, samples->value(sample_n));
    } 
}

void BaselineAnalysis::readoutOrder(bool readout_order) {
	this->readout_order = readout_order; 
}


void BaselineAnalysis::finalize() {

    std::unordered_map <std::pair <int, int>, SamplesPlot*, PairHash>::iterator plot_it = baseline_map.begin();
    
    for (plot_it; plot_it != baseline_map.end(); ++plot_it) { 
        this->processBaselinePlot(plot_it->first.first, plot_it->first.second, plot_it->second); 
    }   

    baseline_map.clear();
    
    writer->close();
    output_file->Close();
}

void BaselineAnalysis::processBaselinePlot(int feb, int hybrid, SamplesPlot* baseline_plot) {

    
    TMultiGraph* samples_mean_baseline = new TMultiGraph();
    TMultiGraph* samples_noise = new TMultiGraph();
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
    for (int sample_n = 0; sample_n < 6; ++sample_n) { 

        TGraphErrors* g_mean_baseline = new TGraphErrors(640);
        g_mean_baseline->SetMarkerColor(2 + sample_n);
        g_mean_baseline->SetLineColor(2 + sample_n);
        TGraph* g_noise = new TGraphErrors(640);    
        g_noise->SetMarkerColor(2 + sample_n);
        g_noise->SetLineColor(2 + sample_n);

        TH2F* baseline_sample_plot = baseline_plot->getPlot(sample_n);
        PlottingUtils::adjust2DPlotRange(baseline_sample_plot, 2);
        for (int channel = 0; channel < 640; ++channel) {
            //this->getSimpleCalibrations(
            //        baseline_sample_plot->ProjectionY("", channel+1, channel+1),
            //        mean_baseline, mean_baseline_error, noise);
            this->getCalibrations(
                    baseline_sample_plot->ProjectionY("", channel+1, channel+1),
                    mean_baseline, mean_baseline_error, noise);
            g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
            g_mean_baseline->SetPointError(channel, 0, mean_baseline_error); 
            g_noise->SetPoint(channel, channel, noise); 
            writer->writeThreshold(feb, hybrid, channel, mean_baseline, noise);
            //writer->writeBaseline(feb, hybrid, channel, sample_n, mean_baseline);
            //writer->writeNoise(feb, hybrid, channel, sample_n, noise);
        }

        samples_mean_baseline->Add(g_mean_baseline); 
        samples_noise->Add(g_noise); 
    }

    samples_mean_baseline->Draw("a"); 
    samples_mean_baseline->GetXaxis()->SetTitle("Channel");
    samples_mean_baseline->GetYaxis()->SetTitle("Mean Baseline [ADC Counts]");
    samples_mean_baseline->Write();
    samples_noise->Draw("a");
    samples_noise->GetXaxis()->SetTitle("Channel");
    samples_noise->GetYaxis()->SetTitle("Noise [ADC Counts]");
    samples_noise->Write();

    output_file->cd();

    delete samples_mean_baseline; 
    delete samples_noise;
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
