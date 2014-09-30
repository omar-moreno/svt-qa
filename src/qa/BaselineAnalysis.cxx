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
	: canvas(NULL), baseline_h(NULL), gaussian(NULL),
      writer(new CalibrationWriter()),
	  class_name("BaselineAnalysis"), 
	  feb_id(0), hybrid_id(0)
{}

BaselineAnalysis::BaselineAnalysis(int feb_id, int hybrid_id)
	: canvas(NULL), baseline_h(NULL), gaussian(NULL),
      writer(new CalibrationWriter()),
	  class_name("BaselineAnalysis"), 
	  feb_id(0), hybrid_id(0)
{
    this->feb_id = feb_id; 
	this->hybrid_id = hybrid_id; 
}

BaselineAnalysis::~BaselineAnalysis()
{
    delete canvas; 
    delete baseline_h;
    delete gaussian;

    delete writer;  
}

void BaselineAnalysis::initialize()
{
    // TODO: Use a histogram factory instead of booking histograms directly
	canvas = new TCanvas("canvas", "canvas", 300, 300);
	baseline_h = new TH2F("baseline", "Baseline", 640, 0, 640, 16384, 0, 16384);
    gaussian = new TF1("gaussian", "gaus");

    writer->openDocument("test.xml");
    // TODO: Add method to writer that checks whether a specific FEB and Hybrid 
    //       node have been created.
    writer->addFeb(feb_id);
    writer->addHybrid(feb_id, hybrid_id); 
    
}

void BaselineAnalysis::processEvent(TriggerSample* samples)
{	
	if(samples->febAddress() != feb_id) return; 
	if(samples->hybrid()  != hybrid_id) return; 

	// Get the physical channel number corresponding to the APV25 channel
	// number.
	int physical_channel = QAUtils::getPhysicalChannel(samples->apv(), samples->channel()); 
	
	for(int sample_n = 0; sample_n < 6; ++sample_n){
            baseline_h->Fill(physical_channel, double(samples->value(sample_n)));
	}
}

void BaselineAnalysis::finalize()
{

    TH1D* projection = NULL;
    double baseline, noise; 
    for(int channel = 0; channel < 640; ++channel){
        projection = baseline_h->ProjectionY("projection", channel+1, channel+1, "e");
        if(projection->GetEntries() == 0){
            std::cout << "[ BaselineAnalysis ]: Histogram has not entries." << std::endl;
            continue; 
        }
        findCalibrations(projection, baseline, noise);
        writer->writeBaseline(feb_id, hybrid_id, 0, channel, 0, baseline);  
        writer->writeNoise(feb_id, hybrid_id, 0, channel, 0, noise);
        //cout << "[ BaselineAnalysis ]: Baseline: " << baseline << " Noise: " << noise << endl; 
    
    } 

    canvas->Print("baseline_run_summary.pdf[");
    baseline_h->Draw("colz");
	canvas->Print("baseline_run_summary.pdf(");
    canvas->Print("baseline_run_summary.pdf]");

    writer->closeDocument();
}

std::string BaselineAnalysis::toString()
{
	std::string string_buffer = "Class Name: " + class_name; 
	return string_buffer; 	
}

void BaselineAnalysis::findCalibrations(TH1* pedestal_plot, double &baseline, double &noise)
{
   gaussian->SetRange(pedestal_plot->GetMean() - 3*pedestal_plot->GetRMS(), 
                      pedestal_plot->GetMean() + 3*pedestal_plot->GetRMS());
    pedestal_plot->Fit("gaussian", "RQ"); 
    baseline = gaussian->GetParameter(1); 
    noise = gaussian->GetParameter(2); 
}



