/**
 *
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *  @date:   May 5, 2014
 *
 */

#include <BaselineAnalysis.h>

BaselineAnalysis::BaselineAnalysis()
	: canvas(NULL), 
      writer(new CalibrationWriter()),
	  class_name("BaselineAnalysis"), 
	  feb_id(-1), hybrid_id(-1)
{}

BaselineAnalysis::BaselineAnalysis(int feb_id, int hybrid_id)
	: canvas(NULL),
      writer(new CalibrationWriter()),
	  class_name("BaselineAnalysis"), 
	  feb_id(feb_id), hybrid_id(feb_id) 
{}

BaselineAnalysis::~BaselineAnalysis() {
    delete canvas; 
    delete writer;  
}

void BaselineAnalysis::initialize() {	
	PlottingUtils::setPalette(); 
	PlottingUtils::setStyle(); 
	
	canvas = new TCanvas("canvas", "canvas", 300, 300);
	
	writer->openDocument("test.xml");
    // TODO: Add method to writer that checks whether a specific FEB and Hybrid 
    //       node have been created.
    //writer->addFeb(feb_id);
    //writer->addHybrid(feb_id, hybrid_id); 
    
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
		baseline_map[daq_pair] = new SamplesPlot(plot_title);
		baseline_map.at(daq_pair)->setXAxisTitles("Physical Channel");
		baseline_map.at(daq_pair)->setYAxisTitles("Baseline [ADC Counts]");
	
		std::cout << "[ BaselineAnalysis ]: Created baseline histogram for FEB: "	
				  << samples->febAddress() << " Hybrid: " << samples->hybrid()
				  << std::endl;
	}
	SamplesPlot* baseline_plot = baseline_map.at(daq_pair); 

	// Get the physical channel number corresponding to the APV25 channel
	// number.
	int physical_channel =  QAUtils::getPhysicalChannel(samples->apv(), samples->channel());
	
	for (int sample_n = 0; sample_n < 6; ++sample_n) {
		baseline_plot->fill(sample_n, physical_channel, samples->value(sample_n));
	} 
}

void BaselineAnalysis::finalize() {

	std::unordered_map <std::pair <int, int>, SamplesPlot*, pairHash>::iterator plot_it = baseline_map.begin();	
	
	for (plot_it; plot_it != baseline_map.end(); ++plot_it) { 
		this->processBaselinePlot(plot_it->first.first, plot_it->first.second, plot_it->second); 
	}	

	baseline_map.clear();

    //writer->writeBaseline(feb_id, hybrid_id, 0, channel, 0, baseline);  
    //writer->writeNoise(feb_id, hybrid_id, 0, channel, 0, noise);

    writer->closeDocument();
}

void BaselineAnalysis::processBaselinePlot(int feb, int hybrid, SamplesPlot* baseline_plot) {

	TMultiGraph* samples_mean_baseline = new TMultiGraph();
	TMultiGraph* samples_noise = new TMultiGraph();

    TH1D* projection = NULL;

	std::string file_name = "feb" + std::to_string(feb) + 
							"_hybrid0" + std::to_string(hybrid) +
							"_baseline_run_summary.pdf";

	canvas->Print((file_name + "[").c_str());
	for (int sample_n = 0; sample_n < 6; ++sample_n) { 

		//baseline_plot->getPlot(sample_n)->Draw("colz"); 
		//canvas->Print((file_name + "(").c_str());
		
		TGraphErrors* mean_baseline = new TGraphErrors(640);
		TGraph* noise = new TGraphErrors(640); 	

		for (int channel = 0; channel < 640; ++channel) { 
			projection = baseline_plot->getPlot(sample_n)->ProjectionY("", channel+1, channel+1);
			mean_baseline->SetPoint(channel,channel, projection->GetMean()); 
			mean_baseline->SetPointError(channel, 0, projection->GetMeanError()); 
			noise->SetPoint(channel, channel, projection->GetRMS()); 
		}

		samples_mean_baseline->Add(mean_baseline); 
		samples_noise->Add(noise); 
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
