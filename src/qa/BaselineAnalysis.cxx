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

BaselineAnalysis::~BaselineAnalysis()
{
    delete canvas; 
    delete writer;  
}

void BaselineAnalysis::initialize()
{

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
		
		//
		//std::string plot_name = "baseline_feb" + PlotUtils::toString(samples->febAddress()) +
		//   	"_hybrid" + PlotUtils::toString(samples->hybrid());	
		//baseline_map[daq_pair] = new TH2F(plot_name.c_str(), "Baseline", 640, 0, 640, 16384, 0, 16384);
		std::string plot_title = "FEB: " + PlotUtils::toString(samples->febAddress()) + 
			" Hybrid: " + PlotUtils::toString(samples->hybrid()) + " Baseline"; 
		baseline_map[daq_pair] = new SamplesPlot(plot_title);
		//baseline_map.at(daq_pair)->SetTitle(plot_title.c_str()); 
		std::cout << "[ BaselineAnalysis ]: Created baseline histogram for FEB: "	
			<< samples->febAddress() << " Hybrid: " << samples->hybrid() << std::endl;
	}
	//TH2F* baseline_plot = baseline_map.at(daq_pair);
	SamplesPlot* baseline_plot = baseline_map.at(daq_pair); 

	// Get the physical channel number corresponding to the APV25 channel
	// number.
	int physical_channel =  QAUtils::getPhysicalChannel(samples->apv(), samples->channel());
	
	for (int sample_n = 0; sample_n < 6; ++sample_n) {
		//baseline_plot->Fill(physical_channel, double(samples->value(sample_n)));
		baseline_plot->fill(sample_n, physical_channel, samples->value(sample_n));
	} 
}

void BaselineAnalysis::finalize()
{

	
	//std::unordered_map <std::pair <int, int>, TH2F*, pairHash>::iterator plot_it = baseline_map.begin(); 
	std::unordered_map <std::pair <int, int>, SamplesPlot*, pairHash>::iterator plot_it = baseline_map.begin(); 
	canvas->Print("baseline_run_summary.pdf[");
    TH1D* projection = NULL;
	//TH2F* baseline_plot = NULL;
	SamplesPlot* baseline_plot = NULL;
	TGraphErrors* mean = NULL;
	TGraphErrors* noise = NULL;
	for (plot_it; plot_it != baseline_map.end(); ++plot_it) { 
		
		baseline_plot = plot_it->second;

		for(int sample_n = 0; sample_n < 6; ++sample_n) { 

			//baseline_plot->getPlot(sample_n)->Draw("colz"); 
			//canvas->Print("baseline_run_summary.pdf(");
			mean = new TGraphErrors(640);
			noise = new TGraphErrors(640); 	
			for (int channel = 0; channel < 640; ++channel) { 
				projection = baseline_plot->getPlot(sample_n)->ProjectionY("", channel+1, channel+1);
				mean->SetPoint(channel,channel, projection->GetMean()); 
				mean->SetPointError(channel, 0, projection->GetMeanError()); 
				noise->SetPoint(channel, channel, projection->GetRMS()); 
			}
			mean->Draw("A*e");
			canvas->Print("baseline_run_summary.pdf(");
			noise->Draw("A*"); 
			canvas->Print("baseline_run_summary.pdf(");
			delete mean;
			delete noise; 	
		}
		//baseline_plot->Draw("colz"); 		
	}	
	canvas->Print("baseline_run_summary.pdf]");

	baseline_map.clear();

    //writer->writeBaseline(feb_id, hybrid_id, 0, channel, 0, baseline);  
    //writer->writeNoise(feb_id, hybrid_id, 0, channel, 0, noise);

    writer->closeDocument();
}

std::string BaselineAnalysis::toString()
{
	std::string string_buffer = "Class Name: " + class_name; 
	return string_buffer; 	
}

/*void BaselineAnalysis::findCalibrations(TH1* pedestal_plot, double &baseline, double &noise)
{
   gaussian->SetRange(pedestal_plot->GetMean() - 3*pedestal_plot->GetRMS(), 
                      pedestal_plot->GetMean() + 3*pedestal_plot->GetRMS());
    pedestal_plot->Fit("gaussian", "RQ"); 
    baseline = gaussian->GetParameter(1); 
    noise = gaussian->GetParameter(2); 
}*/



