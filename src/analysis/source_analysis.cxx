/**
 *	source_analysis.cpp
 * 		author: Omar Moreno <omoreno1@ucsc.edu>
 * 		created: September 27, 2013
 *
 */

//--- C++ ---//
//-----------//
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

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
#include <TGraph.h>
#include <TGraphErrors.h>


#include <RooGlobalFunc.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooExponential.h>
#include <RooPlot.h>
#include <RooAddPdf.h>
#include <RooCBShape.h>


//--- Analysis ---//
//----------------//
#include <PlotUtils.h>
#include <Apv25Utils.h>
#include <SvtCalibrations.h>
//#include "utils/NearestNeighbor.h"
//#include "utils/Cluster.h"

using namespace std;
using namespace RooFit;  

void displayUsage();

int main(int argc, char **argv)
{
	string input_file_name = ""; 
	string input_file_list_name = ""; 
	string output_file_name = "";
	string output_path = ".";
	string baseline_path = "";
	int number_events = 0;
	int run = 0;
	int option_char;
	int threshold = 2; // Noise RMS 	

	// Parse any command line arguments. If there are no valid command line
	// arguments given, print the usage.
	while((option_char = getopt(argc, argv, "h:f:un:p:b:r:i:t:")) != -1){
		switch(option_char){
			case 'i':
				// Name of input file
				input_file_name = optarg;
				break;
			case 'l': 
				// Name of the list that contains the files to process
				input_file_list_name = optarg; 
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
			case 't':
				// Threshold in noise RMS
				threshold = atoi(optarg);
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
		// Check if the file was opened successfully. If not, exit
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

	// Setup the canvas
	TCanvas *canvas = new TCanvas("canvas", "canvas", 500, 500);
	PlotUtils::setupCanvas(canvas);
	PlotUtils::setContours();

	// Fitted signal amplitude for all channels combined
	TH1F* h_sig_amp = new TH1F("sig_amp", "Fitted Signal Amplitude", 34, 0, 1190);
	h_sig_amp->SetLineColor(kBlue-4);
	h_sig_amp->SetLineWidth(2);

	// Fitted signal amplitude by channel
	vector<TH1F*> sig_amps; 
	PlotUtils::initMulti1DPlots(640, sig_amps, "Fitted Signal Amplitudes", 34, 0, 1190);
	
	// Fitted signal amplitude of single hit clusters for all channels combined
	TH1F* h_sh_cluster_amp = new TH1F("sh_cluster_amp", "Single Hit Cluster Amplitude", 34, 0, 1190);
	h_sh_cluster_amp->SetLineColor(kGreen+3);
	h_sh_cluster_amp->SetLineWidth(2);

	// 2D histogram of the fitted signal amplitude vs channel 
	TH2F* sig_amp_vs_channel = new TH2F("sig_amp_vs_channel", "Fitted Signal Amplitude vs Channel", 640, 0, 640, 34, 0, 1190);

	TGraphErrors* g_mean_sig_amp = new TGraphErrors(); 

	// The samples for those events which pass the signal cuts
	vector<TH2F*> signal_samples;
	PlotUtils::initMulti2DPlots(640, signal_samples, "Signal_Samples", 6, 0, 150, 400, -100, 700);

	// The number of events per channel
	TGraph* g_n_events = new TGraph();

	//
	TH1F* h_n_hits = new TH1F("n_hits", "Number of Hits Above Threshold", 10, 0, 10);
	h_n_hits->SetLineColor(kBlue-4); 
	h_n_hits->SetLineWidth(2); 	

	//
	TH1F* h_n_clusters = new TH1F("n_clusters", "Number of Clusters", 10, 0, 10);
	h_n_clusters->SetLineColor(kBlue-4); 
	h_n_clusters->SetLineWidth(2); 	

	//
	TH1F* h_n_hits_clusters = new TH1F("n_hit_clusters", "Number of Hits per Clusters", 10, 0, 10);
	h_n_clusters->SetLineColor(kBlue-4); 
	h_n_clusters->SetLineWidth(2); 	

	// Plots of all samples when the given number of samples are above threshold
	TH1F* h_one_above_thresh = new TH1F("h_one_above_thresh", "Single Sample Above Threshold", 100, -200, 1190);
	TH1F* h_two_above_thresh = new TH1F("h_two_above_thresh", "Two Samples Above Threshold", 100, -200, 1190);
	TH1F* h_three_above_thresh = new TH1F("h_tree_above_thresh", "Three Samples Above Threshold", 100, -200, 1190);
	TH1F* h_four_above_thresh = new TH1F("h_four_above_thresh", "Four Samples Above Threshold", 100, -200, 1190);

	// Load calibrations
	bool calibrations_loaded = SvtCalibrations::getInstance()->loadCalibrations(baseline_path); 

	//NearestNeighbor* clusterer = new NearestNeighbor();

	// Output stream used to dump the configuration file
	ofstream output_config; 
	string file_path, file_name, input_line;
	double sig_samples[6] = {0};
	int channel;
	double max_sample;
	int event_number = 0;
	double baseline, noise;
	int pass_threshold[6] = {0};
	int threshold_sum = 0; 
	int fpga = 0; 
	int hybrid = 1;
	uint pedestal = 0; 	
	double fit_params[4] = {0};
	map<int, double> hit_map;
	bool read_config = false; 
	for(list<string>::iterator files_it = files.begin(); files_it != files.end(); files_it++){

		cout << "Processing file " << *files_it << endl;

		if(!dataRead->open(*files_it)){
			cerr << "Files not found!" << endl;
			return EXIT_FAILURE;
		}

		// flag to indicate if configuration should be dumped and values should
		// be read.  this will be done at the beginning of each calibration run.
		// note: the use of a flag is required because the configuration is
		//       not parsed until after an event has been loaded.  the use of a
		//       flag is to avoid reading and writing the configuration during
		//       every event.
		read_config = true; 
		while(dataRead->next(event)){


			if(read_config){ 

				cout << "Reading configuration" << endl;

				// All configuration files will be dumped into a "config"
				// directory created within the output path specified by the
				// user.  If the config directory doesn't exist, create it.
				struct stat sb = {0};
				file_path = output_path + "/config";
				if(stat(file_path.c_str(), &sb) == -1){
					mkdir(file_path.c_str(), 0777);
				}

				file_name = *files_it; 
				file_name = file_name.substr(input_line.find_last_of("/") + 1);
				file_name.erase(file_name.find("."));
				file_name += ".config";
				output_config.open((file_path + "/" + file_name).c_str());
				if(output_config.is_open()){ 
					//dataRead->dumpConfig(output_config); 
					cout << "DAQ configuration has been dumped to " + file_path + "/" + file_name << endl;
				}
				output_config.close(); 

				read_config = false;
			}


			// Once the number of desired events has been reached, skip the
			// rest of the events
			++event_number;
			if(event_number == number_events) break;
			if(event_number%1000 == 0){
				cout << "Event: " << event_number << endl;
			}

			//hit_map.clear(); 
			for(uint x = 0; x < event->count(); x++){

				// Get the samples
				samples = event->sample(x);

				// Get the channel number
				channel = (samples->apv()*128) + samples->channel();

				// Get the physical channel number
				channel = Apv25Utils::getPhysicalChannel(samples->apv(), samples->channel());
				
				// Fill the sample array and subtract the pedestal.
				pedestal = sig_samples[0]; 
				threshold_sum = 0;
				for(int sample_n = 0; sample_n < 6; ++sample_n){
					pass_threshold[sample_n] = 0; 
					sig_samples[sample_n] = samples->value(sample_n);
						sig_samples[sample_n] -= SvtCalibrations::getInstance()->getChannelBaseline(fpga, hybrid, channel);
						noise = SvtCalibrations::getInstance()->getChannelNoise(fpga, hybrid, channel);

					if(sig_samples[sample_n] >= threshold*noise) pass_threshold[sample_n]++;
				}
		
				// Check if any sample is above threshold
				for(int sample_n = 0; sample_n < 6; ++sample_n){
					if(pass_threshold[sample_n] > 0) threshold_sum = 1;
					break; 
				}

				// Check if two consecutive samples are above threshold
				for(int sample_n = 0; sample_n < 5; ++sample_n){
					if(pass_threshold[sample_n] > 0 && pass_threshold[sample_n + 1] > 0) threshold_sum = 2; 
				}

				// Check if three consecutive samples are above threshold
				for(int sample_n = 0; sample_n < 4; ++sample_n){ 
					if(pass_threshold[sample_n] > 0 && pass_threshold[sample_n + 1] > 0 && pass_threshold[sample_n + 2] > 0) threshold_sum = 3; 
				}

				for(int sample_n = 0; sample_n < 4; ++sample_n){ 
					if(pass_threshold[sample_n] > 0 && pass_threshold[sample_n + 1] > 0 && pass_threshold[sample_n + 2] > 0 && pass_threshold[sample_n + 3] > 0) threshold_sum = 4; 
				}


				if(threshold_sum >= 1 && channel > 1 && channel < 637){
					for(int sample_n =0; sample_n <6;sample_n++){
					h_one_above_thresh->Fill(sig_samples[sample_n]);
					}

				} 
			   
				if(threshold_sum >= 2 && channel > 1 && channel < 637){
					for(int sample_n =0; sample_n <6;sample_n++){
					h_two_above_thresh->Fill(sig_samples[sample_n]);
					}
				} 
				
				if(threshold_sum >= 3 && channel > 1 && channel < 637){
					
					for(int sample_n =0; sample_n <6;sample_n++){
					h_three_above_thresh->Fill(sig_samples[sample_n]);
					}

					Apv25Utils::fitShaperSignal("Root", sig_samples, fit_params);
					h_sig_amp->Fill(fit_params[0]);				
					sig_amp_vs_channel->Fill(channel, fit_params[0]);
					sig_amps[channel]->Fill(fit_params[0]);

					//hit_map.insert(pair<int, double>(channel, fit_params[0]));

					max_sample = 0;
					for(int sample_n = 0; sample_n < 6; ++sample_n){
						signal_samples[channel]->Fill(sample_n*25, sig_samples[sample_n]);
						if(sig_samples[sample_n] > max_sample) max_sample = sig_samples[sample_n];
					}
				}
				
				if(threshold_sum >= 4 && channel > 1 && channel < 637){
					for(int sample_n =0; sample_n <6;sample_n++){
					h_four_above_thresh->Fill(sig_samples[sample_n]);
					}
				} 
			}

			/*if(hit_map.size() > 0){ 
				h_n_hits->Fill(hit_map.size()); 
				list<Cluster> clusters = clusterer->findClusters(hit_map);
				h_n_clusters->Fill(clusters.size());
				cout << "Number of Clusters: " << clusters.size() << endl;
				for(list<Cluster>::iterator cluster_iterator = clusters.begin(); cluster_iterator != clusters.end(); cluster_iterator++){ 
					cout << "Number of hits in cluster: " << cluster_iterator->getNumberOfHits() << endl; 
					h_n_hits_clusters->Fill(cluster_iterator->getNumberOfHits());
				   cout << "Amplitude: " << cluster_iterator->getAmplitude() << endl;	
					if(cluster_iterator->getNumberOfHits() == 1 && cluster_iterator->getAmplitude() > 0){
						cout << "Filling plot" << endl;
						h_sh_cluster_amp->Fill(cluster_iterator->getAmplitude()); 
					}
				
				}

			}*/
		}

		dataRead->close(); 
	}

	// Setup the file names to output the plots to
	string output_file_ext = ".pdf"; 
	output_file_name = "source_run" + PlotUtils::convertToString(run); 

	cout << output_file_name << endl;

	// If a "plots" folder does not exist, create it. This folder will be used
	// to store any ROOT files generated along with all plots.
	struct stat sb = {0};
	file_path = output_path + "/plots";
	if(stat(file_path.c_str(), &sb) == -1){
		mkdir(file_path.c_str(), 0777);
	}

	canvas->Print((file_path + "/" + output_file_name + "_ch_signal_samples" + output_file_ext + "[").c_str());
	for(channel = 0; channel < 640; ++channel){
		signal_samples[channel]->Draw("colz");
		PlotUtils::set2DPlotStyle(signal_samples[channel], "Sample Number", "Amplitude [ADC Counts]");
		canvas->Print((file_path + "/" + output_file_name + "_ch_signal_samples" + output_file_ext + "(").c_str());

	}
	canvas->Print((file_path + "/" + output_file_name + "_ch_signal_samples" + output_file_ext + "]").c_str());


	canvas->Print((file_path + "/" + output_file_name + "_ch_signal_amplitude" + output_file_ext + "[").c_str());
	double entries = 0; 
	double error = 0; 
	for(channel = 0; channel < 640; ++channel){
		entries = sig_amps[channel]->GetEntries(); 
		if(entries == 0) continue;
		sig_amps[channel]->Fit("gaus", "q");
		g_n_events->SetPoint(channel, channel, entries); 
		g_mean_sig_amp->SetPoint(channel, channel, sig_amps[channel]->GetFunction("gaus")->GetParameter(1));
		error = sig_amps[channel]->GetFunction("gaus")->GetParameter(2)/sqrt(entries); 
		g_mean_sig_amp->SetPointError(channel, 0, error); 	
		sig_amps[channel]->Draw();
		//		PlotUtils::set2DPlotStyle(signal_samples[channel], "Sample Number", "Amplitude [ADC Counts]");
		canvas->Print((file_path + "/" + output_file_name + "_ch_signal_amplitude" + output_file_ext + "(").c_str());

	}
	canvas->Print((file_path + "/" + output_file_name + "_ch_signal_amplitude" + output_file_ext + "]").c_str());

	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "[").c_str());

	//--- Fit the signal peak ---//
	//---------------------------//
/*
	// Observable
	RooRealVar amplitude("amplitude", "Fitted Amplitude [ADC Counts]", 0., 1190., "");
	RooRealVar c_amplitude("c_amplitude", "Single Hit Cluster Fitted Amplitude [ADC Counts]", 0., 1190., "");

	// Create a CrystalBall function object
	RooRealVar signal_mean("signal_mean", "Signal Mean Value", 220., 150., 300.);
	RooRealVar signal_sigma("signal_sigma", "Signal #sigma Value", 30., 0., 100.);
	RooRealVar alpha("alpha", "#alpha", -10, -1000, -1);
	RooRealVar n("n", "n", 100, 0, 10000); 
	RooCBShape sig_cb("signal", "signal", amplitude, signal_mean, signal_sigma, alpha, n);

	RooRealVar c_signal_mean("c_signal_mean", "Single Hit Cluster Signal Mean", 220., 150., 300.);
	RooRealVar c_signal_sigma("c_signal_sigma", "Single Hit Cluster Signal #sigma Value", 30., 0., 100.);
	RooRealVar c_alpha("c_alpha", "Single Hit Cluster #alpha", -10, -1000, -1);
	RooRealVar c_n("c_n", "Single Hit Cluster n", 100, 0, 10000); 
	RooCBShape c_sig_cb("c_signal", "Single Hit Cluster Signal", c_amplitude, c_signal_mean, c_signal_sigma, c_alpha, c_n);



	// Get the data
	RooDataHist amplitude_hist("amplitude_hist", "Fitted Amplitude", amplitude, h_sig_amp);
	RooDataHist sh_cluster_hist("sh_cluster_hist", "Single Hit Cluster Amplitude", c_amplitude, h_sh_cluster_amp); 

	// Fit the CrystalBall function to the fitted amplitude data
	sig_cb.fitTo(amplitude_hist);
	c_sig_cb.fitTo(sh_cluster_hist); 

	// Create a RooPlot and plot the data
	RooPlot* frame = amplitude.frame(Title(""));
	RooPlot* c_frame = c_amplitude.frame(Title(""));
	amplitude_hist.plotOn(frame, MarkerSize(.5), MarkerColor(kGreen+3), LineColor(kGreen+3), LineWidth(1.5));
	sh_cluster_hist.plotOn(c_frame, MarkerSize(.5), MarkerColor(kRed), LineColor(kRed), LineWidth(1.5)); 

	sig_cb.plotOn(frame, LineColor(kBlue-4));
	c_sig_cb.plotOn(c_frame, LineColor(kRed)); 

	frame->GetXaxis()->SetLabelSize(0.025);
	frame->GetXaxis()->SetTitleSize(0.025);
	frame->GetXaxis()->SetTitleOffset(1.2);
	frame->GetYaxis()->SetLabelSize(0.025);
	frame->GetYaxis()->SetTitleSize(0.025);
	frame->GetYaxis()->SetTitleOffset(1.4);
	frame->SetTitle(""); 
	frame->Draw(); 
	sig_cb.Print("t"); 
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	canvas->SetLogy(1);
	frame->Draw(); 
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	canvas->SetLogy(0);

	c_frame->GetXaxis()->SetLabelSize(0.025);
	c_frame->GetXaxis()->SetTitleSize(0.025);
	c_frame->GetXaxis()->SetTitleOffset(1.2);
	c_frame->GetYaxis()->SetLabelSize(0.025);
	c_frame->GetYaxis()->SetTitleSize(0.025);
	c_frame->GetYaxis()->SetTitleOffset(1.4);
	c_frame->SetTitle(""); 
	c_frame->Draw(); 
	c_sig_cb.Print("t"); 
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());


	canvas->SetLogy(1);
	c_frame->Draw(); 
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());
*/
	canvas->SetLogy(0);
	
	h_sig_amp->Draw(); 
	h_sh_cluster_amp->Draw("same"); 
	PlotUtils::set1DPlotStyle(h_sh_cluster_amp, "Amplitude [ADC Counts]"); 
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	sig_amp_vs_channel->Draw("colz");	
	PlotUtils::set2DPlotStyle(sig_amp_vs_channel,  "Physical Channel #", "Fitted Amplitude [ADC Counts]");
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());


	g_mean_sig_amp->Draw("A*");
	PlotUtils::set2DPlotStyle(g_mean_sig_amp, "Physical Channel #", "Mean Fitted Amplitude [ADC Counts]");
	g_mean_sig_amp->GetYaxis()->SetRangeUser(160, 400); 	
	g_mean_sig_amp->GetXaxis()->SetRangeUser(0, 640); 	
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	g_n_events->Draw("A*"); 	
	g_n_events->GetXaxis()->SetRangeUser(0, 640); 	
	PlotUtils::set2DPlotStyle(g_n_events, "Physical Channel #", "# of Events");
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	h_n_hits->Draw();
	PlotUtils::set1DPlotStyle(h_n_hits, "Number of Hits/Event"); 
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	h_n_clusters->Draw();
	PlotUtils::set1DPlotStyle(h_n_clusters, "Number of Clusters"); 
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());

	h_n_hits_clusters->Draw();
	PlotUtils::set1DPlotStyle(h_n_hits_clusters, "Number of Clusters");	
	canvas->Print((file_path + "/" + output_file_name + output_file_ext + "(").c_str());
	
	canvas->SetLogy(1);

	h_one_above_thresh->Draw();
	h_one_above_thresh->SetLineColor(kRed); 

	h_two_above_thresh->Draw("same");
	h_two_above_thresh->SetLineColor(kBlue); 
	
	h_three_above_thresh->Draw("same");
	h_three_above_thresh->SetLineColor(kGreen); 

	h_four_above_thresh->Draw("same"); 

	canvas->Print((file_path + "/" + output_file_name + output_file_ext + ")").c_str());

	return EXIT_SUCCESS;
}


void displayUsage()
{
	cout << "Usage: source_analysis [OPTIONS] ..."                 << endl;
	cout << "Example: apv25_qa -i input_file.bin"           << endl;
	cout << "\n\t -i    Input file to be processed"         << endl;
	cout << "\t -o    Optional output file name. By default,"
		<< " all output is written to default.pdf"         << endl;
	cout << "\t -n    The number of events to process"		<< endl;
	cout << "\t -p    The output path"						<< endl;
	cout << "\t -b    The path to the file containing the calibration"
		<<	" constants"									<< endl;
	cout << "\t -r 	  The run number " 						<< endl;
	cout << "\t -u    Show this usage \n"                   << endl;
	exit(EXIT_FAILURE);
}

