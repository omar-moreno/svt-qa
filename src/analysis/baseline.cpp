/**
 *  baseline.cpp
 *     author: Omar Moreno <omoreno1@ucsc.edu>
 *     created: July 2, 2013
 */

//--- C++ ---//
//-----------//
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>

//--- DAQ ---//
//-----------//
#include <TrackerEvent.h>
#include <TrackerSample.h>
#include <DataRead.h>
#include <Data.h>

//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TF1.h>
#include <TH2.h>
#include <TError.h>
#include <TTree.h>
#include <TFile.h>
#include <TLegend.h>

//--- Utils ---//
//-------------//
#include <PlotUtils.h>
#include <Apv25Utils.h>

using namespace std; 

void displayUsage(); 

int main(int argc, char **argv)
{

	string input_file_name; 
	//string output_file_name = "baseline.pdf";
	string output_path = ".";
	uint fpga = 0;
	uint hybrid = -1; 
	int option_char; 
	int number_events = 0; 
	int run = -1;
	bool short_hybrid = false;
	// Parse any command line arguments. If there are no valid command line
	// arguments given, print the usage.
	while((option_char = getopt(argc, argv, "i:h:un:r:p:s")) != -1){
		switch(option_char){
			case 'i':
				input_file_name = optarg;
				break;
				// case 'o':
				//     output_file_name = optarg;
				//     break;
				// case 'f':
				//     fpga = atoi(optarg); 
				//     break; 
			case 'h': 
				hybrid = atoi(optarg); 
				break; 
			case 'n': 
				number_events = atoi(optarg); 
				break;
			case 'r':
				run = atoi(optarg);
				break;
			case 'p':
				output_path = optarg;
				break;
			case 's':
				short_hybrid = true; 
				break;
			case 'u':
				displayUsage(); 
			default:
				displayUsage(); 
		}
	}

	// If an input file (binary or EVIO) was not specified, exit the program
	if(input_file_name.length() == 0){
		cout << "\n Please specify an input file to process." 
			<< "\n Use '-h' flag for usage.\n" << endl;
		return EXIT_FAILURE; 
	}

	// If a valid FPGA address was not specified, exit the program
	/*if(fpga == uint(-1) || fpga < uint(0) || fpga > uint(6)){ 
	  cout << "\nPlease specify a valid FPGA address."
	  << "\nUse '-h' flag for usage.\n" << endl;
	  return EXIT_FAILURE;  
	  }*/

	// If a valid hybrid address was not specified, exit the program
	if(hybrid <= uint(0)){
		cout << "\nPlease specify a valid hybrid address."
			<< "\n Use '-h' flag for usage.\n" << endl;
		return EXIT_FAILURE;
	}

	if(run <= 0){ 
		cerr << "\nPlease specify a valid run number."
			<< "\nUse '-h' flag for usage.\n" << endl;
		return EXIT_FAILURE; 
	}

	// 
	TrackerEvent *event = new TrackerEvent(); 
	TrackerSample *samples = NULL;
	DataRead *dataRead = new DataRead(); 

	// Open the input file.  If the input file cannot be opened exit.
	if(!dataRead->open(input_file_name)){
		cerr << "\n Error! File " << input_file_name << " cannot be opened."
			<< endl;
		return EXIT_FAILURE;
	}

	cout << "Processing file: " << input_file_name << endl;

	//--- Setup Histograms ---//
	//------------------------//
	gErrorIgnoreLevel = kWarning; 
	TCanvas *canvas = new TCanvas("canvas", "canvas", 500, 500);
	PlotUtils::setupCanvas(canvas);
	PlotUtils::setContours();  

	TH2F* h_pedestal = new TH2F("baseline", "Baseline", 640, 0, 640, 8000, 2000, 10000);
	h_pedestal->SetStats(0);
	vector<TH2F*> h_pedestal_sample;
	PlotUtils::initMulti2DPlots(6, h_pedestal_sample, "Baseline_Sample", 640, 0, 640, 8000, 2000, 10000);
	TGraph* g_mean  = new TGraph();
	g_mean->SetMarkerSize(0.4);
	g_mean->SetMarkerColor(kBlue-4);
	PlotUtils::set1DPlotStyle(g_mean, "Mean Baseline [ADC Counts]");  
	TGraph* g_noise = new TGraph();
	g_noise->SetMarkerSize(0.4);
	g_noise->SetMarkerColor(kBlue-4);
	PlotUtils::set1DPlotStyle(g_noise, "Noise [ADC Counts]");

	TGraph* graph = NULL;
	TMultiGraph* mg_mean_baseline = new TMultiGraph();
	TMultiGraph* mg_noise = new TMultiGraph();
	TMultiGraph* mg_mean_baseline_diff = new TMultiGraph();
	TMultiGraph* mg_noise_diff = new TMultiGraph();
	int color_index;
	TLegend* legend = new TLegend(.8, .99, .99, .85);
	legend->SetFillColor(0);
	for(int sample = 0; sample < 6; ++sample){

		color_index = sample + 2;
		graph = new TGraph();
		graph->SetMarkerColor(color_index);
		graph->SetMarkerStyle(3);
		graph->SetMarkerSize(0.4);
		mg_mean_baseline->Add(graph, "p");
		legend->AddEntry(graph, ("Sample " + PlotUtils::convertToString(sample+1)).c_str(), "p");

		graph = new TGraph();
		graph->SetMarkerColor(color_index);
		graph->SetMarkerStyle(3);
		graph->SetMarkerSize(0.4);
		mg_noise->Add(graph, "p");

		graph = new TGraph();
		graph->SetMarkerColor(color_index);
		graph->SetMarkerStyle(3);
		graph->SetMarkerSize(0.4);
		mg_mean_baseline_diff->Add(graph, "p");

		graph = new TGraph();
		graph->SetMarkerColor(color_index);
		graph->SetMarkerStyle(3);
		graph->SetMarkerSize(0.4);
		mg_noise_diff->Add(graph, "p");
	}


	//--- Analysis ---//
	//----------------//
	int channel;
	int event_number = 0; 
	double noise, baseline;

	// Loop through all of the events until the end of file is reached 
	while(dataRead->next(event)){

		// Skip the event if the event doesn't contain data from the FPGA 
		// of interest
		//if(fpga != event->fpgaAddress()) continue;  

		// Once the number of desired events has been reached, skip the
		// rest of the events
		++event_number; 
		if(event_number == number_events) break; 
		if(event_number%500 == 0){
			cout << "Event: " << event_number << endl; 
		}

		for(uint x = 0; x < event->count(); x++){

			// Get the samples
			samples = event->sample(x); 

			// Skip the sample if it doesn't come from the hybrid of interest
			//if(hybrid != samples->hybrid()) continue; 

			// Get the physical channel 
			channel = Apv25Utils::getPhysicalChannel(samples->apv(), samples->channel());

			// For source test, comment out otherwise
			/*if((samples->value(1) > samples->value(0) && samples->value(2) > samples->value(1)) ||
			  (samples->value(2) > samples->value(1) && samples->value(3) > samples->value(2)) ||
			  (samples->value(3) > samples->value(2) && samples->value(4) > samples->value(3)) ||
			  (samples->value(4) > samples->value(3) && samples->value(5) > samples->value(4))) continue; 
			 */
			for(int sample = 0; sample < 6; ++sample){
				h_pedestal->Fill(channel, double(samples->value(sample)));
				h_pedestal_sample[sample]->Fill(channel, double(samples->value(sample)));
			}
		}
	}

	cout << "Finish processing " << event_number << " events. Generating plots ... " << endl;

	// Setup the output file names
	string file_path, output_file_name;
	string output_file_ext = ".pdf";
	if(short_hybrid){
		output_file_name = "hybrid_s"; 
	} else { 
		output_file_name = "hybrid"; 
	}
	if(hybrid < 10) output_file_name += "0" + PlotUtils::convertToString(hybrid);
	else output_file_name += PlotUtils::convertToString(hybrid); 
	output_file_name += "_baseline_dtrig_run";
	if(run < 10) output_file_name += "0" + PlotUtils::convertToString(run); 
	else output_file_name += PlotUtils::convertToString(run); 

	// If a "baseline" folder does not exist, create it.  The channels
	// baselines in text form will be stored here.
	struct stat sb = {0};
	file_path = output_path + "/baseline";
	if(stat(file_path.c_str(), &sb) == -1){
		cout << "Creating a baseline directory ... " << endl;
		mkdir(file_path.c_str(), 0777);
	}

	// Open a file to save the pedestal and noise to.
	ofstream baseline_file_name;
	baseline_file_name.open((file_path + "/" + output_file_name + ".baseline").c_str());
	cout << "Writing baseline file to " + file_path + "/" + output_file_name + ".baseline" << endl;
	if(baseline_file_name.is_open()){
		baseline_file_name << "#" << " Pedestal and Noise for FPGA " << fpga
			<< ", Hybrid " << hybrid << "\n#"
			<< setw(15) << " FPGA "
			<< setw(15) << " Hybrid "
			<< setw(15) << " Channel " 
			<< setw(15) << " Baseline " 
			<< setw(15) << " Noise\n#"
			<< setfill('#') << setw(75) << "\n" 
			<< setfill(' '); 
	}

	// If a "plots" folder does not exist in the path, create it.  This folder
	// will be used to store any ROOT files generated along with all plots.
	file_path = output_path + "/plots";
	if(stat(file_path.c_str(), &sb) == -1){
		mkdir(file_path.c_str(), 0777);
	}

	cout << "Output files will be saved to " + file_path << endl;
	// Open a ROOT file to save some simple calibration variables
	TFile *r_file = NULL;
	TTree *tree = NULL;
	r_file = new TFile((file_path + "/" + output_file_name + ".root").c_str(), "recreate");
	tree = new TTree("FPGA_Data", "A tree containing data from an FPGA");
	tree->Branch("fpga", &fpga, "fpga/I");
	tree->Branch("hybrid", &hybrid, "hybrid/I");
	tree->Branch("channel", &channel, "channel/I");
	tree->Branch("noise", &noise, "noise/D");
	tree->Branch("baseline", &baseline, "baseline/D");
	tree->Branch("run", &run, "run/I");

	//--- Extract baseline and noise for all samples combined ---//
	//-----------------------------------------------------------//

	canvas->cd();
	// Histogram pointer that the projected histogram will be assigned to
	TH1D* h_pedestal_proj = NULL;
	// Open the file to which the plots will be output to
	canvas->Print((file_path + "/" + output_file_name + "_ch_plots" + output_file_ext + "[").c_str());
	// Set the number of points that the baseline and noise graphs will contain
	g_mean->Set(639);
	g_noise->Set(639);

	for(channel = 0; channel < 640; channel++){

		h_pedestal_proj = h_pedestal->ProjectionY(("Channel: " + PlotUtils::convertToString(channel)).c_str(), channel+1, channel+1, "e");
		if(h_pedestal_proj->GetEntries() == 0) continue;
		PlotUtils::adjust1DPlotRange(h_pedestal_proj, 1); 
		PlotUtils::set1DPlotStyle(h_pedestal_proj, "Baseline [ADC Counts]"); 
		h_pedestal_proj->Fit("gaus", "q"); 
		baseline = h_pedestal_proj->GetFunction("gaus")->GetParameter(1); 
		noise = h_pedestal_proj->GetFunction("gaus")->GetParameter(2); 
		g_mean->SetPoint(channel, channel, baseline);
		g_noise->SetPoint(channel, channel, noise);
		h_pedestal_proj->Draw("");  
		canvas->Print((file_path + "/" + output_file_name + "_ch_plots" + output_file_ext + "(").c_str());

		if(baseline_file_name.is_open()){
			baseline_file_name << setw(15) << fpga
				<< setw(15) << hybrid
				<< setw(15) << channel 
				<< fixed
				<< setw(15) << setprecision(5) << baseline 
				<< setw(15) << setprecision(5) << noise << "\n"; 
		}

		tree->Fill();
	}
	canvas->Print((file_path + "/" + output_file_name + "_ch_plots" + output_file_ext + "]").c_str());

	TList* g_baseline_list = mg_mean_baseline->GetListOfGraphs();
	TList* g_noise_list = mg_noise->GetListOfGraphs();
	TList* g_baseline_diff_list = mg_mean_baseline_diff->GetListOfGraphs();
	TList* g_noise_diff_list = mg_noise_diff->GetListOfGraphs();
	string title;
	double point_x, point_y, point_x_base, point_y_base;
	for(int sample = 0; sample < 6; ++sample){
		// Open the file to which the plots will be output to
		canvas->Print((file_path + "/" + output_file_name + "_ch_plots_sample" + PlotUtils::convertToString(sample + 1) + output_file_ext + "[").c_str());
		for(channel = 0; channel < 640; ++channel){
			title = "Channel: " + PlotUtils::convertToString(channel) + " Sample: " + PlotUtils::convertToString(sample);
			h_pedestal_proj = h_pedestal_sample[sample]->ProjectionY(title.c_str(), channel+1, channel+1, "e");
			if(h_pedestal_proj->GetEntries() == 0) continue;
			PlotUtils::adjust1DPlotRange(h_pedestal_proj, 2);
			PlotUtils::set1DPlotStyle(h_pedestal_proj, "Baseline [ADC Counts]");
			h_pedestal_proj->Fit("gaus", "q");
			baseline = h_pedestal_proj->GetFunction("gaus")->GetParameter(1);
			noise = h_pedestal_proj->GetFunction("gaus")->GetParameter(2);
			((TGraph*) g_baseline_list->At(sample))->SetPoint(channel, channel, baseline);
			((TGraph*) g_noise_list->At(sample))->SetPoint(channel, channel, noise);
			h_pedestal_proj->Draw("");
			canvas->Print((file_path + "/" + output_file_name + "_ch_plots_sample" + PlotUtils::convertToString(sample + 1) + output_file_ext + "(").c_str());

			g_mean->GetPoint(channel, point_x_base, point_y_base);
			((TGraph*) g_baseline_diff_list->At(sample))->SetPoint(channel, channel, point_y_base - baseline);

			g_noise->GetPoint(channel, point_x_base, point_y_base);
			((TGraph*) g_noise_diff_list->At(sample))->SetPoint(channel, channel, point_y_base - noise);
		}
		canvas->Print((file_path + "/" + output_file_name + "_ch_plots_sample" + PlotUtils::convertToString(sample + 1) + output_file_ext + "]").c_str());
	}


	//--- Save Summary Histograms ---//
	//-------------------------------//

	string title_prefix = "FPGA: " + PlotUtils::convertToString(fpga) + " Hybrid: " + PlotUtils::convertToString(hybrid);
	output_file_name += "_summary";

	PlotUtils::adjust2DPlotRange(h_pedestal, 0);  
	canvas->Clear();
	h_pedestal->Draw("colz");
	PlotUtils::set2DPlotStyle(h_pedestal, "Physical Channel #", "Baseline [ADC Counts]");
	h_pedestal->SetTitle(title_prefix.c_str());
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	canvas->Clear();
	g_mean->Draw("A*");
	g_mean->GetXaxis()->SetRangeUser(0, 639);
	PlotUtils::set2DPlotStyle(g_mean, "Physical Channel #", "Mean Baseline [ADC Counts]"); 
	//g_mean->SetTitle(title_prefix.c_str());
	//g_mean->GetYaxis()->SetRangeUser(5000, 7000);
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	canvas->Clear();
	mg_mean_baseline->Draw("A*");
	mg_mean_baseline->GetXaxis()->SetRangeUser(0, 639);
	PlotUtils::set2DPlotStyle(mg_mean_baseline, "Physical Channel #", "Mean Baseline [ADC Counts]");
	mg_mean_baseline->SetTitle(title_prefix.c_str());
	//mg_mean_baseline->GetYaxis()->SetRangeUser(5000, 7000);
	legend->Draw();
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	canvas->Clear();
	mg_mean_baseline_diff->Draw("A*");
	mg_mean_baseline_diff->GetXaxis()->SetRangeUser(0, 639);
	PlotUtils::set2DPlotStyle(mg_mean_baseline_diff, "Physical Channel #", "Difference [ADC Counts]");
	mg_mean_baseline_diff->SetTitle(title_prefix.c_str());
	legend->Draw();
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());


	canvas->Clear();
	g_noise->Draw("A*");
	g_noise->GetXaxis()->SetRangeUser(0, 639);
	PlotUtils::set2DPlotStyle(g_noise, "Physical Channel #", "Noise [ADC Counts]");
	g_noise->SetTitle(title_prefix.c_str());
	//g_noise->GetYaxis()->SetRangeUser(0, 100);
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	canvas->Clear();
	mg_noise->Draw("A*");
	mg_noise->GetXaxis()->SetRangeUser(0, 639);
	PlotUtils::set2DPlotStyle(mg_noise, "Physical Channel #", "Noise [ADC Counts]");
	mg_noise->SetTitle(title_prefix.c_str());
	//mg_noise->GetYaxis()->SetRangeUser(0, 100);
	legend->Draw();
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	canvas->Clear();
	mg_noise_diff->Draw("A*");
	mg_noise_diff->GetXaxis()->SetRangeUser(0, 639);
	PlotUtils::set2DPlotStyle(mg_noise_diff, "Physical Channel #", "Difference [ADC Counts]");
	mg_noise_diff->SetTitle(title_prefix.c_str());
	legend->Draw();
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + ")").c_str());

	baseline_file_name.close();

	// Write the ROOT tree and close the ROOT file
	tree->Write();
	r_file->Close();

	return EXIT_SUCCESS; 
}

void displayUsage()
{
	cout << "Usage: baseline [OPTIONS] ..."                 << endl;
	cout << "Example: baseline -i input_file.bin"           << endl;
	cout << "\n\t -i    Input file to be processed"         << endl;
	//cout << "\t -o    Optional output file name. By default," 
	//     << " all output is written to default.pdf"         << endl;
	//cout << "\t -f    FPGA address in the range of 0-6"     << endl;
	cout << "\t -h    Hybrid address in the range of 0-2"   << endl;
	cout << "\t -r    Run number. Used if ROOT output is enabled" << endl;
	cout << "\t -p    Path to save all output files to"     << endl;
	cout << "\t -u    Show this usage \n"                   << endl;
	exit(EXIT_FAILURE);
}
