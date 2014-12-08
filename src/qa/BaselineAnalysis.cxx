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
    : canvas(NULL), 
      writer(new CalibrationWriter()),
      class_name("BaselineAnalysis"), 
      feb_id(-1), hybrid_id(-1), readout_order(false)
{}

BaselineAnalysis::BaselineAnalysis(int feb_id, int hybrid_id)
    : canvas(NULL),
      writer(new CalibrationWriter()),
      class_name("BaselineAnalysis"), 
      feb_id(feb_id), hybrid_id(feb_id), readout_order(false) 
{}

BaselineAnalysis::~BaselineAnalysis() {
    delete canvas; 
    delete writer;  
}

void BaselineAnalysis::initialize() {

    
    PlottingUtils::setPalette(); 
    PlottingUtils::setStyle(); 
    
    canvas = new TCanvas("canvas", "canvas", 300, 300);
    
    writer->open("test.xml");

	for(int n = 0; n < 128; n++){
		int channel = (32*(n%4)) + (8*(n/4)) - (31*(n/16));
		channel_map[channel] = n; 
		//channel_map.push_back(n); 
    }

}

void BaselineAnalysis::processEvent(TriggerSample* samples) {

    //if(samples->febAddress() != feb_id) return; 
    //if(samples->hybrid()  != hybrid_id) return; 

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
        baseline_map.at(daq_pair)->setXAxisTitles("Physical Channel");
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

    // Get the physical channel number corresponding to the APV25 channel
    // number.
    //int physical_channel =  QAUtils::getPhysicalChannel(samples->apv(), samples->channel());
    
    for (int sample_n = 0; sample_n < 6; ++sample_n) {
        baseline_plot->fill(sample_n, channel, samples->value(sample_n));
    } 
}

void BaselineAnalysis::readoutOrder(bool readout_order) {
	this->readout_order = readout_order; 
}


void BaselineAnalysis::finalize() {

    std::unordered_map <std::pair <int, int>, SamplesPlot*, pairHash>::iterator plot_it = baseline_map.begin(); 
    
    for (plot_it; plot_it != baseline_map.end(); ++plot_it) { 
        this->processBaselinePlot(plot_it->first.first, plot_it->first.second, plot_it->second); 
    }   

    baseline_map.clear();
    
    writer->close();
}

void BaselineAnalysis::processBaselinePlot(int feb, int hybrid, SamplesPlot* baseline_plot) {

    
    TMultiGraph* samples_mean_baseline = new TMultiGraph();
    TMultiGraph* samples_noise = new TMultiGraph();
    TH1D* projection = NULL;

    std::string file_name = "feb";
    if (feb < 10) file_name += "0";
    file_name += std::to_string(feb) + "_hybrid0" + std::to_string(hybrid);
	if (readout_order) file_name += "_readout_order"; 
    file_name += "_baseline_run_summary.pdf";

    canvas->Print((file_name + "[").c_str());

    baseline_plot->getSumOfPlots()->Draw("colz"); 
    canvas->Print((file_name + "(").c_str());
    
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
            projection = baseline_sample_plot->ProjectionY("", channel+1, channel+1);
            double mean_baseline = projection->GetMean();
            double mean_baseline_error = projection->GetMeanError();
            double noise = projection->GetRMS();
            g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
            g_mean_baseline->SetPointError(channel, 0, mean_baseline_error); 
            g_noise->SetPoint(channel, channel, noise); 
            writer->writeBaseline(feb, hybrid, channel, sample_n, mean_baseline);
            writer->writeNoise(feb, hybrid, channel, sample_n, noise);
        }

        samples_mean_baseline->Add(g_mean_baseline); 
        samples_noise->Add(g_noise); 
    }

    samples_mean_baseline->Draw("Ap");
    canvas->Print((file_name + "(").c_str());
    samples_noise->Draw("Ap");
    canvas->Print((file_name + "(").c_str());
    canvas->Print((file_name + "]").c_str());
   

    delete samples_mean_baseline; 
    delete samples_noise;
}

std::string BaselineAnalysis::toString() {
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer;   
}
