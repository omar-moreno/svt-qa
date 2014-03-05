/**
 *	@section purpose
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *  @date:   January 15, 2013
 *  @version: 1.0
 */

//--- C++ ---//
//-----------//
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>
#include <cmath>

//--- ROOT ---//
//------------//
#include <TCanvas.h>
#include <TError.h>
#include <TH2F.h>
#include <TChain.h>
#include <TGraphErrors.h>
#include <TH1D.h>

//--- Analysis ---//
//----------------//
#include <PlotUtils.h>

using namespace std; 

void displayUsage(); 

int main(int argc, char **argv)
{
	string input_file_list_name = "";
	string output_path = ".";

	int number_events = 0;
	int option_char; 

	// Parse any command line arguments. If there are no valid command line
	// arguments given, print the usage.
	while((option_char = getopt(argc, argv, "l:un:p:")) != -1){
		switch(option_char){
			case 'l': 
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
			case 'u':
				displayUsage(); 
			default:
				displayUsage(); 
		}
	}

	// If a valid input file list has not beed specified, exit the program
	if(input_file_list_name.length() < 0){
		cerr << "\nCannot specify both input file name and a list of files." << endl;
		return EXIT_FAILURE; 
	}

	// Chain the input files together
	TChain *chain = new TChain("FPGA_Data"); 
	string file; 
	ifstream input_file_list(input_file_list_name.c_str(), ifstream::in);
	while(input_file_list >> file){
		chain->Add(file.c_str()); 
	}

	// Setup histgrams
	gErrorIgnoreLevel = kWarning; 
	TCanvas *canvas = new TCanvas("canvas", "canvas", 1000, 500); 
	TCanvas *canvas_response = new TCanvas("canvas_response", "canvas_response", 1500, 1000); 
	PlotUtils::setupCanvas(canvas); 
	PlotUtils::setupCanvas(canvas_response); 
	canvas->Divide(2,1);
	canvas->cd();
	canvas->cd(1); 
	vector<TH2F*> h_response;
	PlotUtils::initMulti2DPlots(640, h_response, "Response", 55, 0, 55000, 300, 0, 3000); 
	PlotUtils::setContours();
	vector<TGraphErrors*> mean_response; 

	// 
	double response; 
	int hybrid, channel, charge;
	chain->SetBranchAddress("response", &response); 
	chain->SetBranchAddress("hybrid", &hybrid); 
	chain->SetBranchAddress("channel", &channel); 
	chain->SetBranchAddress("charge", &charge); 

	cout << "Entries: " << chain->GetEntries() << endl;
	for(int entry = 0; entry < (int) chain->GetEntries(); ++entry){
		chain->GetEntry(entry); 
		h_response[channel]->Fill(charge, response); 	
	}

	string output_file_name = "hybrid_" + PlotUtils::convertToString(hybrid)
		+ "_response_"; 
	string output_file_ext = ".pdf"; 

	// If a "plots" folder does not exist, create it. This folder will be used
	// to store any ROOT files generated along with all plots.
	struct stat sb = {0};
	output_path += "/plots";
	if(stat(output_path.c_str(), &sb) == -1){
		mkdir(output_path.c_str(), 0777);
	}

	cout << "Files will be saved to " << output_path << endl;

	canvas->Print((output_path + "/" + output_file_name + "ch" + output_file_ext + "[").c_str()); 
	vector<TH1D*> projections;
	vector<TGraphErrors*> g_response;
	double means[10] = {0}; 
	double errors[10] = {0}; 
	double charges[10] = { 5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000};
	TGraphErrors* g_gain = new TGraphErrors(640);
	TGraphErrors* g_inv_gain = new TGraphErrors(640); 
	TGraphErrors* g_offset = new TGraphErrors(640);
	TGraphErrors* g_chisqr = new TGraphErrors(640); 
	for(int channel_n = 0; channel_n < 640; ++channel_n){
		canvas->cd(1); 
		projections.clear(); 
		h_response[channel_n]->Draw("colz");
		PlotUtils::set2DPlotStyle(h_response[channel_n], "Charge [e^{-}]", "Response [ADC]"); 
		h_response[channel_n]->SetTitle(("Hybrid: " + PlotUtils::convertToString(hybrid) + " Channel: " + PlotUtils::convertToString(channel_n)).c_str()); 
		h_response[channel_n]->SetStats(0); 

		canvas->cd(2); 
		for(int index = 0; index < 10; ++index){
			cout << "Getting means of distribution for channel number " << channel_n <<  endl;
			projections.push_back(h_response[channel_n]->ProjectionY(("ch" + PlotUtils::convertToString(channel_n) + "_proj" + PlotUtils::convertToString((index+1)*5000)).c_str(), (index+1)*5 + 1, (index+1)*5 + 1));
			projections[index]->Fit("gaus");
			means[index] = projections[index]->GetFunction("gaus")->GetParameter("Mean"); 
			errors[index] = sqrt(projections[index]->GetFunction("gaus")->GetParameter("Sigma"))/projections[index]->GetEntries(); 
		
		}

		g_response.push_back(new TGraphErrors(10, charges, means, errors, 0)); 

		g_response[channel_n]->SetTitle("");
		g_response[channel_n]->SetMarkerStyle(21); 
		g_response[channel_n]->SetMarkerColor(kBlue-3); 
		g_response[channel_n]->SetMarkerSize(1.0); 
		g_response[channel_n]->SetLineWidth(2);  
		g_response[channel_n]->GetXaxis()->SetTitle("Charge [e^{-}]");
		g_response[channel_n]->GetXaxis()->SetTitleSize(0.03); 
		g_response[channel_n]->GetXaxis()->SetTitleOffset(1.4); 
		g_response[channel_n]->GetXaxis()->SetLabelSize(0.03); 
		g_response[channel_n]->GetYaxis()->SetTitle("Response [ADC]");
		g_response[channel_n]->GetYaxis()->SetTitleSize(0.03); 
		g_response[channel_n]->GetYaxis()->SetTitleOffset(1.8); 
		g_response[channel_n]->GetYaxis()->SetLabelSize(0.03); 

		g_response[channel_n]->Draw("ape"); 
		g_response[channel_n]->Fit("pol1", "q"); 

		TF1 *fit = g_response[channel_n]->GetFunction("pol1"); 
		canvas->cd(1); 
		fit->Draw("same");


		canvas->Print((output_path + "/" + output_file_name + "ch" + output_file_ext + "(").c_str()); 

		g_gain->SetPoint(channel_n, channel_n, fit->GetParameter("p1")); 
		g_inv_gain->SetPoint(channel_n, channel_n, 1/fit->GetParameter("p1")); 
		g_offset->SetPoint(channel_n, channel_n, fit->GetParameter("p0"));
		g_chisqr->SetPoint(channel_n, channel_n, g_response[channel_n]->Chisquare(fit)); 
	
	}

	canvas->Print((output_path + "/" + output_file_name + "ch" + output_file_ext + "]").c_str()); 
	
	canvas->Clear();
	canvas->SetCanvasSize(700, 700); 
	g_gain->Draw("ape");
	g_gain->GetXaxis()->SetRangeUser(0,640); 
	PlotUtils::set2DPlotStyle(g_gain, "Physical Channel #", "ADC Count/e^{-}");
	g_gain->SetMarkerStyle(3); 
	g_gain->SetMarkerColor(kBlue-3); 
	canvas->Print((output_path + "/" + output_file_name + "summary" + output_file_ext + "(").c_str());
	
	g_inv_gain->Draw("ape");
	g_inv_gain->GetXaxis()->SetRangeUser(0,640); 
	PlotUtils::set2DPlotStyle(g_inv_gain, "Physical Channel #", "e^{-}/ADC Count");
	g_inv_gain->SetMarkerStyle(3); 
	g_inv_gain->SetMarkerColor(kBlue-3); 
	canvas->Print((output_path + "/" + output_file_name + "summary" + output_file_ext + "(").c_str());
	
	g_offset->Draw("ape");
	g_offset->GetXaxis()->SetRangeUser(0,640); 
	PlotUtils::set2DPlotStyle(g_offset, "Physical Channel #", "Offset [ADC Counts]");
	g_offset->SetMarkerStyle(3); 
	g_offset->SetMarkerColor(kBlue-3); 
	canvas->Print((output_path + "/" + output_file_name + "summary" + output_file_ext + "(").c_str());
	
	g_chisqr->Draw("ape"); 
	g_chisqr->GetXaxis()->SetRangeUser(0, 640); 
	PlotUtils::set2DPlotStyle(g_chisqr, "Physical Channel #", "#chi^{2}"); 
	g_chisqr->SetMarkerStyle(3); 
	g_chisqr->SetMarkerColor(kBlue-3); 
	canvas->Print((output_path + "/" + output_file_name + "summary" + output_file_ext + ")").c_str());


	return EXIT_SUCCESS; 
}

void displayUsage(){
}
