/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 * @date: July 23, 2014
 *
 */

#include <RootPlotter.h>

RootPlotter::RootPlotter() : canvas(new TCanvas())
{
	// The default plot style should be loaded here
}


RootPlotter::~RootPlotter()
{
   delete canvas;	
}


TH1F* RootPlotter::get1DHistogram(std::string name, std::string x_label,
										int bins_x, int min_x, int max_x)
{
	if(histogram1D_map.find(name) != histogram1D_map.end()){
		return  (TH1F*) histogram1D_map[name];
	}	

	TH1* histogram =  RootHistogramFactory::create1DHistogram(name, bins_x, min_x, max_x);
	histogram->GetXaxis()->SetTitle(x_label.c_str());

	histogram1D_map.insert(std::pair<std::string, TH1*>(name, histogram)); 

	return (TH1F*) histogram1D_map[name];
}

TH2F* RootPlotter::get2DHistogram(std::string name, 
										std::string x_label, std::string y_label,
									   	int bins_x, int min_x, int max_x,
									   	int bins_y, int min_y, int max_y)
{
	if(histogram2D_map.find(name) != histogram2D_map.end()){
		return (TH2F*) histogram2D_map[name];
	}	

	TH2* histogram =  RootHistogramFactory::create2DHistogram(name, bins_x, min_x, max_x, bins_y, min_y, max_y);
	histogram->GetXaxis()->SetTitle(x_label.c_str());
	histogram->GetYaxis()->SetTitle(y_label.c_str());
	histogram2D_map.insert(std::pair<std::string, TH2*>(name, histogram));
	
	return (TH2F*) histogram2D_map[name];
}

TGraph* RootPlotter::getGraph(std::string name, std::string x_label, std::string y_label) 
{
	if(graph_map.find(name) != graph_map.end()){
		return graph_map[name]; 
	}

	TGraph* graph = RootGraphFactory::createGraph();
	graph->GetXaxis()->SetTitle(x_label.c_str());
	graph->GetYaxis()->SetTitle(y_label.c_str());
	graph_map.insert(std::pair<std::string, TGraph*>(name, graph)); 

	return graph; 
}

TH1F* RootPlotter::get1DHistogram(std::string name)
{
	if(histogram1D_map.find(name) == histogram1D_map.end()){
		// TODO: Throw an exception if a histogram isn't found
		return NULL;
	}	

	return (TH1F*) histogram1D_map[name];
}

TH2F* RootPlotter::get2DHistogram(std::string name)
{
	if(histogram2D_map.find(name) == histogram2D_map.end()){
		// TODO: Throw an exception if a histogram isn't found
		return NULL;
	}	

	return (TH2F*) histogram2D_map[name];
}

TGraph* RootPlotter::getGraph(std::string name)
{
	if(graph_map.find(name) == graph_map.end()){
		// TODO: Throw an exception if a graph isn't found
		return NULL; 
	}

	return graph_map[name]; 
}

void RootPlotter::SavePlots(std::string file_name)
{
	std::string suffix = file_name.substr(file_name.find("."));
	
	histogram1D_iter = histogram1D_map.begin();
	for(histogram1D_iter; histogram1D_iter != histogram1D_map.end(); ++histogram1D_iter){
		histogram1D_iter->second->Draw();
		canvas->SaveAs(file_name.c_str());
	}

	histogram2D_iter = histogram2D_map.begin();
	for(histogram2D_iter; histogram2D_iter != histogram2D_map.end(); ++histogram2D_iter){
		histogram2D_iter->second->Draw("colz");
		canvas->SaveAs(file_name.c_str());
	}

	graph_iter = graph_map.begin();
	for(graph_iter; graph_iter != graph_map.end(); graph_iter++){
		graph_iter->second->Draw("Ap");
		canvas->SaveAs(file_name.c_str());
	}
}
