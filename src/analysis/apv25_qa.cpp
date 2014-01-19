/**
 *	@section purpose
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *  @date:   July 2, 2013
 *  @version: 1.0
 */

//--- C++ ---//
//-----------//
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <list>

//--- DAQ ---//
//---------------//
#include <TrackerEvent.h>
#include <TrackerSample.h>
#include <DataRead.h>
#include <Data.h>

//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TError.h>
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>

//--- Analysis ---//
//----------------//
#include <PlotUtils.h>
#include <Apv25Utils.h>
#include <SvtCalibrations.h>

using namespace std; 

void displayUsage(); 


int main(int argc, char **argv)
{

	string input_file_name = "";
	string input_file_list_name = ""; 
	string output_path = ".";
	string baseline_path = "";
	uint hybrid = 0;
	int module = 0; 
	int number_events = 0;
	int run = 0;
	bool short_hybrid = false;
	int option_char;

	// Parse any command line arguments. If there are no valid command line
	// arguments given, print the usage.
	while((option_char = getopt(argc, argv, "i:l:h:m:un:p:b:r:s")) != -1){
		switch(option_char){
			case 'i':
				input_file_name = optarg; 
				break;
			case 'l': 
				input_file_list_name = optarg; 
				break; 
			case 'h': 
				// Hybrid ID to process
				hybrid = atoi(optarg); 
				break;
			case 'm': 
				// Module ID to process
				module = atoi(optarg);
				break; 
			case 'n': 
				// The number of events to process
				number_events = atoi(optarg); 
				break;
			case 'p':
				// The path to output all files to
				output_path = optarg;
				break;
			case 'b':
				// The path to baseline file
				baseline_path = optarg;
				break;
			case 'r':
				// The run number
				run = atoi(optarg);
				break;
			case 's':
				// SVT data is being processed
				short_hybrid = true;
				break;
			case 'u':
				displayUsage(); 
			default:
				displayUsage(); 
		}
	}

	// If a valid input file name or a valid input file list has not been specified, 
	// exit the program
	if(input_file_name.length() == 0 && input_file_list_name.length() == 0){
		cerr << "\nPlease specify a file name or a list of files."
			<< "\nUse '-h' flag for usage.\n" << endl;
		return EXIT_FAILURE;
	} else if(input_file_name.length() > 0 && input_file_list_name.length() > 0){
		cerr << "\nCannot specify both input file name and a list of files." << endl;
		return EXIT_FAILURE; 
	}

	// If a valid hybrid number was not specified, exit the program
	if(hybrid <= uint(0)){
		cerr << "\nPlease specify a valid hybrid address."
			<< "\n Use '-h' flag for usage.\n" << endl;
		return EXIT_FAILURE;
	}

	// If a valid run number has not been specified, exit the program
	if(run <= 0){ 
		cerr << "\nPlease specify a valid run number."
			<< "\nUse '-h' flag for usage.\n" << endl;
		return EXIT_FAILURE; 
	}


	// Create a list of files to process
	list<string> files;
	string file; 
	if(input_file_name.length() > 0){
		files.push_back(input_file_name); 
	} else if(input_file_list_name.length() > 0){
		ifstream input_file_list(input_file_list_name.c_str(), ifstream::in);
		if(!input_file_list.is_open()){
			cerr << "Failed to open file " << input_file_list << "!" << endl;
			return EXIT_FAILURE; 
		}
		while(input_file_list >> file){
			files.push_back(file); 
		}
	}

	// Instantiate the objects needed to read the tracker data
	TrackerEvent *event = new TrackerEvent(); 
	TrackerSample *samples = NULL;
	DataRead *dataRead = new DataRead(); 

	//--- Setup Histograms ---//
	//------------------------//
	gErrorIgnoreLevel = kWarning; 
	TCanvas *canvas = new TCanvas("canvas", "canvas", 500, 500); 
	PlotUtils::setupCanvas(canvas); 
	PlotUtils::setContours(); 

	//  Shaper signal amplitude vs channel
	TH2F* h_amplitude = new TH2F("amplitude", "Signal Amplitude", 640, 0, 640, 5000, 0, 10000); 
	h_amplitude->SetStats(0);
	h_amplitude->SetMarkerColor(kBlue-4);

	//  Mean shaper signal amplitude vs channel
	TGraph* g_mean_amplitude = new TGraph(); 
	//    PlotUtils::set1DPlotStyle(g_mean_amplitude, "Mean Amplitude [ADC Counts]");
	g_mean_amplitude->SetMarkerColor(kBlue-4);
	g_mean_amplitude->SetMarkerSize(0.4);

	//
	TH2F* h_tp = new TH2F("tp", "T_{p}", 640, 0, 640, 130, 0, 130);
	h_tp->SetStats(0);  
	TGraph* g_mean_tp = new TGraph(); 
	g_mean_tp->SetMarkerColor(kBlue-4);
	g_mean_tp->SetMarkerSize(0.4);

	//    PlotUtils::set1DPlotStyle(g_mean_tp, "Mean T_{p} [ns]");

	TH2F* h_t0 = new TH2F("t0", "T_{0}", 640, 0, 640, 100, 0, 100);
	h_t0->SetStats(0);  
	TGraph* g_mean_t0 = new TGraph(); 
	g_mean_t0->SetMarkerColor(kBlue-4);
	g_mean_t0->SetMarkerSize(0.4);
	//    PlotUtils::set1DPlotStyle(g_mean_t0, "Mean T_{0} [ns]");

	vector<TH2F*> signal_samples; 
	PlotUtils::initMulti2DPlots(640, signal_samples, "Signal_Samples", 6, 0, 150, 1200, -400, 2000);
	/*vector<TH2F*> h_amp_vs_chi2;
	  PlotUtils::initMulti2DPlots(640, h_amp_vs_chi2, "Amplitude_vs_Chi2", 1000, 0, 4000, 1000, 0, 1000);
	  vector<TH2F*> h_tp_vs_chi2;
	  PlotUtils::initMulti2DPlots(640, h_tp_vs_chi2, "tp_vs_Chi2", 65, 0, 130, 1000, 0, 1000);
	  vector<TH2F*> h_t0_vs_chi2;
	  PlotUtils::initMulti2DPlots(640, h_t0_vs_chi2, "t0_vs_Chi2", 50, 0, 100, 1000, 0, 1000);*/

	//-------------------------//
	//-------------------------//



	// Output stream used to dump the configuration file
	ofstream output_config; 

	// Load calibrations
	bool calibrations_loaded = SvtCalibrations::getInstance()->loadCalibrations(baseline_path);

	// Process all of the files specified in the input file list
	string file_path, file_name;
	int fpga = 0; 
	bool read_config = false; 
	double sig_samples[6] = {0};
	double fit_params[4] = {0};
	int event_number;  
	int channel, calibration_group;
	int injected_charge = 18125;
	double response, tp, t0;

	TFile* r_file = NULL;
	TTree* tree = NULL;
	string output_file_name = "";
	string output_file_ext = ".pdf";

	for(list<string>::iterator files_it = files.begin(); files_it != files.end(); files_it++){

		// Attempt to open the data file
		if(!dataRead->open(*files_it)){
			cerr << "File not found" << endl;
			return EXIT_FAILURE;
		}

		cout << "Processing file " << *files_it << endl;

		//calibration_group = 0; 

		//--- Analysis ---//
		//----------------//

		// flag to indicate if configuration should be dumped and values should
		// be read.  this will be done at the beginning of each calibration run.
		// note: the use of a flag is required because the configuration is
		//       not parsed until after an event has been loaded.  the use of a
		//       flag is to avoid reading and writing the configuration during
		//       every event.
		read_config = true; 
		event_number = 0;
		uint pedestal = 0; 
		// Loop through all of the events until the end of the file is reached
		while(dataRead->next(event)){

			if(read_config){ 

				// All configuration files will be dumped into a "config"
				// directory created within the output path specified by the
				// user.  If the config directory doesn't exist, create it.
				struct stat sb = {0};
				file_path = output_path + "/config";
				if(stat(file_path.c_str(), &sb) == -1){
					mkdir(file_path.c_str(), 0777);
				}

				file_name = file.substr(file.find_last_of("/") + 1);
				file_name.erase(file_name.find("."));
				file_name += ".config";
				output_config.open((file_path + "/" + file_name).c_str());
				if(output_config.is_open()){ 
					//dataRead->dumpConfig(output_config); 
					cout << "DAQ configuration has been dumped to " + file_path + "/" + file_name << endl;
				}
				output_config.close(); 

				// Get the calibration group number and injected charge
				calibration_group = dataRead->getConfigInt("cntrlFpga:hybrid:apv25:CalGroup");
				injected_charge = dataRead->getConfigInt("cntrlFpga:hybrid:apv25:Ical"); 
				injected_charge *= 625;

				cout << "Processing calibration group " << calibration_group << endl;
				cout << "Amount of charge injected:  " << injected_charge << " e-" << endl; 

				read_config = false;

				file_path = output_path + "/plots";

				if(r_file == NULL){ 
					// Setup the file names to output the plots to
					if(short_hybrid){
						output_file_name = "hybrid_s"; 
					} else if(hybrid > 0){ 
						output_file_name = "hybrid"; 
					} else if(module > 0){
						output_file_name = "hm"; 
					}

					if(hybrid < 10 || module < 10) output_file_name += "0" + PlotUtils::convertToString(hybrid);
					else output_file_name += PlotUtils::convertToString(hybrid); 

					output_file_name += "_charge" + PlotUtils::convertToString(injected_charge) + "_run";

					if(run < 10) output_file_name += "0" + PlotUtils::convertToString(run); 
					else output_file_name += PlotUtils::convertToString(run); 

					// If a "plots" folder does not exist, create it. This folder will be used
					// to store any ROOT files generated along with all plots.
					if(stat(file_path.c_str(), &sb) == -1){
						mkdir(file_path.c_str(), 0777);
					}

					cout << "Files will be saved to " << file_path << endl;
					cout << "Saving to file " << output_file_name << endl;

					// Open a ROOT file to save some simple calibration variables
					r_file = new TFile((file_path + "/" + output_file_name + ".root").c_str(), "recreate");
					tree = new TTree("FPGA_Data", "A tree containing data from an FPGA" );
					tree->Branch("fpga", &fpga, "fpga/I");
					tree->Branch("hybrid", &hybrid, "hybrid/I");
					tree->Branch("channel", &channel, "channel/I");
					tree->Branch("response", &response, "response/D");
					tree->Branch("t0", &t0, "t0/D");
					tree->Branch("tp", &tp, "tp/D");
					tree->Branch("run", &run, "run/I");
					tree->Branch("charge", &injected_charge, "injected_charge/I");
				}

			}

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

				// Get the channel number
				channel = (samples->apv()*128) + samples->channel(); 

				// Only process channels which are part of the current calibration group
				if((channel - calibration_group)%8 == 0){

					// Get the physical channel number
					channel = Apv25Utils::getPhysicalChannel(samples->apv(), samples->channel());

					// Fill the sample array 
					for(int sample_n = 0; sample_n < 6; ++sample_n){
						sig_samples[sample_n] = samples->value(sample_n); 
					}

					// Only look at events which contain a positive signal
					// if(sig_samples[3] > sig_samples[2])  // <== Use when Latency is set to 134
					if(sig_samples[1] > sig_samples[0]){  // <== Use when Latency is set to 132

						// If a pedestal files exist, load the pedestals from there. Otherwise, 
						// use the first sample as the pedestal.
						pedestal = sig_samples[0]; 
						for(int sample_n = 0; sample_n < Apv25Utils::TOTAL_SAMPLES; ++sample_n){
							if(calibrations_loaded){
								sig_samples[sample_n] -= SvtCalibrations::getInstance()->getChannelBaseline(fpga, hybrid, channel);
								signal_samples[channel]->Fill(sample_n*25, sig_samples[sample_n]);

							} else {
								sig_samples[sample_n] -= pedestal;
								signal_samples[channel]->Fill(sample_n*25, sig_samples[sample_n]);
							}
						}

						Apv25Utils::fitShaperSignal("Root", sig_samples, fit_params);
						response = fit_params[0]; 
						tp = fit_params[1];
						t0 = fit_params[2]; 
						h_amplitude->Fill(channel, fit_params[0]); 
						h_tp->Fill(channel, fit_params[1]); 
						h_t0->Fill(channel, fit_params[2]); 
						/*h_amp_vs_chi2[channel]->Fill(fit_params[0], fit_params[3]);
						  h_tp_vs_chi2[channel]->Fill(fit_params[1], fit_params[3]);
						  h_t0_vs_chi2[channel]->Fill(fit_params[2], fit_params[3]); */
						tree->Fill(); 
					}
				}
			}    
		}
	}

	TH1D* h_proj = NULL;
	g_mean_amplitude->Set(639);
	g_mean_tp->Set(639);
	g_mean_t0->Set(639);
	for(channel = 0; channel < 640; ++channel){

		h_proj = h_amplitude->ProjectionY(("Amplitude - Channel: " + PlotUtils::convertToString(channel)).c_str(), channel+1, channel+1, "e");
		if(h_proj->GetEntries() == 0) continue;
		PlotUtils::adjust1DPlotRange(h_proj,2);
		h_proj->Fit("gaus", "q");
		response = h_proj->GetFunction("gaus")->GetParameter(1);
		g_mean_amplitude->SetPoint(channel, channel, response);

		h_proj = h_tp->ProjectionY(("Tp - Channel: " + PlotUtils::convertToString(channel)).c_str(), channel+1, channel+1, "e");
		if(h_proj->GetEntries() == 0) continue;
		PlotUtils::adjust1DPlotRange(h_proj,2);
		h_proj->Fit("gaus", "q");
		tp = h_proj->GetFunction("gaus")->GetParameter(1);
		g_mean_tp->SetPoint(channel, channel, tp);

		h_proj = h_t0->ProjectionY(("T0 - Channel: " + PlotUtils::convertToString(channel)).c_str(), channel+1, channel+1, "e");
		if(h_proj->GetEntries() == 0) continue;
		PlotUtils::adjust1DPlotRange(h_proj,2);
		h_proj->Fit("gaus", "q");
		t0 = h_proj->GetFunction("gaus")->GetParameter(1);
		g_mean_t0->SetPoint(channel, channel, t0);

	}

	PlotUtils::adjust2DPlotRange(h_amplitude, 1);
	h_amplitude->Draw("colz");
	PlotUtils::set2DPlotStyle(h_amplitude, "Physical Channel #", "Pedestal Subtracted Amplitude [ADC Counts]");
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());
	g_mean_amplitude->Draw("A*"); 
	PlotUtils::set2DPlotStyle(g_mean_amplitude, "Physical Channel #", "Mean Response [ADC Counts]");
	g_mean_amplitude->GetXaxis()->SetRangeUser(0, 640);  
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	PlotUtils::adjust2DPlotRange(h_tp, 1); 
	h_tp->Draw("colz"); 
	PlotUtils::set2DPlotStyle(h_tp, "Physical Channel #", "t_{p} [ns]");
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());
	g_mean_tp->Draw("A*");
	PlotUtils::set2DPlotStyle(g_mean_tp, "Physical Channel #", "Mean t_{p} [ns]");
	g_mean_tp->GetXaxis()->SetRangeUser(0, 640);
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	PlotUtils::adjust2DPlotRange(h_t0, 1); 
	h_t0->Draw("colz"); 
	PlotUtils::set2DPlotStyle(h_t0, "Physical Channel #", "t_{0} [ns]");
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());
	g_mean_t0->Draw("A*"); 
	PlotUtils::set2DPlotStyle(g_mean_t0, "Physical Channel #", "Mean t_{0} [ns]");
	g_mean_t0->GetXaxis()->SetRangeUser(0, 640);  
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + ")").c_str());

	/*
	   canvas->Print((file_path + "/" + output_file_name + "_ch_amp_vs_chi2" + output_file_ext + "[").c_str());
	   canvas->Print((file_path + "/" + output_file_name + "_ch_tp_vs_chi2" + output_file_ext + "[").c_str());
	   canvas->Print((file_path + "/" + output_file_name + "_ch_t0_vs_chi2" + output_file_ext + "[").c_str());
	 */
	canvas->Print((file_path + "/" + output_file_name + "_ch_samples" + output_file_ext + "[").c_str());
	TF1 shaper_fit;
	for(channel = 0; channel < 640; ++channel){


		// Code used to fit the mean of the samples ...
		for(int sample_n = 1; sample_n <= 6; ++sample_n){
			h_proj = signal_samples[channel]->ProjectionY(("Channel: " + PlotUtils::convertToString(channel) + " - Sample "
						+ PlotUtils::convertToString(sample_n)).c_str(), sample_n, sample_n, "e");
			if(h_proj->GetEntries() == 0) continue;
			h_proj->Fit("gaus", "q");
			sig_samples[sample_n -1] = h_proj->GetFunction("gaus")->GetParameter(1);
		}


		Apv25Utils::fitShaperSignal("Root", sig_samples, fit_params);

		shaper_fit = Apv25Utils::getShaperSignalFit();
		signal_samples[channel]->SetStats(0);
		signal_samples[channel]->SetTitle(("Channel: " + PlotUtils::convertToString(channel) + " - Charge: "
					+ PlotUtils::convertToString(injected_charge)).c_str());
		signal_samples[channel]->Draw("colz");
		shaper_fit.Draw("same");
		PlotUtils::set2DPlotStyle(signal_samples[channel], "Sample Number", "Amplitude [ADC Counts]");
		canvas->Print((file_path + "/" + output_file_name + "_ch_samples" + output_file_ext + "(").c_str());

		/*
		//        PlotUtils::adjust2DPlotRange(h_amp_vs_chi2[channel], 0);
		h_amp_vs_chi2[channel]->Draw("colz");
		PlotUtils::set2DPlotStyle(h_amp_vs_chi2[channel], "#Chi^{2}", "Amplitude [ADC Counts]");
		canvas->Print((file_path + "/" + output_file_name + "_ch_amp_vs_chi2" + output_file_ext + "(").c_str());

		//        PlotUtils::adjust2DPlotRange(h_tp_vs_chi2[channel], 0);
		h_tp_vs_chi2[channel]->Draw("colz");
		PlotUtils::set2DPlotStyle(h_tp_vs_chi2[channel], "#Chi^{2}", "t_{p} [ns]");
		canvas->Print((file_path + "/" + output_file_name + "_ch_tp_vs_chi2" + output_file_ext + "(").c_str());

		//        PlotUtils::adjust2DPlotRange(h_t0_vs_chi2[channel], 0);
		h_t0_vs_chi2[channel]->Draw("colz");
		PlotUtils::set2DPlotStyle(h_t0_vs_chi2[channel], "#Chi^{2}", "t_{0} [ns]");
		canvas->Print((file_path + "/" + output_file_name + "_ch_t0_vs_chi2" + output_file_ext + "(").c_str());
		 */

	}
	canvas->Print((file_path + "/" + output_file_name + "_ch_samples" + output_file_ext + "]").c_str());
	/*
	   canvas->Print((file_path + "/" + output_file_name + "_ch_amp_vs_chi2" + output_file_ext + "]").c_str());
	   canvas->Print((file_path + "/" + output_file_name + "_ch_tp_vs_chi2" + output_file_ext + "]").c_str());
	   canvas->Print((file_path + "/" + output_file_name + "_ch_t0_vs_chi2" + output_file_ext + "]").c_str());
	 */
	tree->Write();
	r_file->Close();

	return EXIT_SUCCESS; 
}

void displayUsage()
{
	cout << "Usage: apv25_qa [OPTIONS]"                     			<< endl;
	cout << "An input file or a list of inputs files must be specified" << endl;
	cout << "Example: apv25_qa -i input_file.bin [OPTIONS]"             << endl;
	cout << "Example: apv25_qa -l input_file_list.txt [OPTIONS]\n"      << endl;
	cout << "\t -i    Input file to be processed"         			    << endl;
	cout << "\t -l 	  Text file containing a list of input files" 		<< endl;
	cout << "\t -h    Hybrid number of the device under test"	        << endl;
	cout << "\t -m    Half-module number of the device under test"      << endl;
	cout << "\t -n    The number of events to process"		            << endl;
	cout << "\t -p    The output path"								    << endl;
	cout << "\t -b    The path to the file containing the calibration"
		<< " constants"												<< endl;
	cout << "\t -r 	  The run number " 								    << endl;
	cout << "\t -u    Show this usage \n"                   			<< endl;
	exit(EXIT_FAILURE);
}
