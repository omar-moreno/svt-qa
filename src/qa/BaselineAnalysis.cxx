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
	: canvas(NULL), baseline_h(NULL),
	  class_name("BaselineAnalysis"), 
	  feb_id(0), hybrid_id(0)
{}

BaselineAnalysis::BaselineAnalysis(int feb_id, int hybrid_id)
{
	this->feb_id = feb_id; 
	this->hybrid_id = hybrid_id; 
}

BaselineAnalysis::~BaselineAnalysis()
{
    delete canvas; 
    delete baseline_h;
}

void BaselineAnalysis::initialize()
{
    // TODO: Use a histogram factory instead of booking histograms directly
	canvas = new TCanvas("canvas", "canvas", 300, 300);
	baseline_h = new TH2F("baseline", "Baseline", 640, 0, 640, 16384, 0, 16384);
    
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

    canvas->Print("baseline_run_summary.pdf[");
    baseline_h->Draw("colz");
	canvas->Print("baseline_run_summary.pdf(");
    canvas->Print("baseline_run_summary.pdf]");
}

std::string BaselineAnalysis::toString()
{
	std::string string_buffer = "Class Name: " + class_name; 
	return string_buffer; 	
}




