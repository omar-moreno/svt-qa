/**
 *	@file	PlottingUtils.h
 *	@brief	A compilation of plotting utilities.
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Paritcle Physics
 *			University of California, Santa Cruz
 *	@date	2013
 *
 */

#include <PlottingUtils.h>

void PlottingUtils::setPalette() {
        
		// Setup the color palette
        const int n_rgb  = 5; 
        const int n_cont = 255;

        double stops[n_rgb] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
        double red[n_rgb]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
        double green[n_rgb] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
        double blue[n_rgb]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
        TColor::CreateGradientColorTable(n_rgb, stops, red, green, blue, n_cont);
        gStyle->SetNumberContours(n_cont); 
}

TStyle* PlottingUtils::getStyle() { 
	
	TStyle* style = new TStyle("Clean", "Clean Style");

	// Set frame attributes
	style->SetFrameBorderMode(0);
	style->SetFrameBorderSize(0);
	style->SetFrameFillColor(0);

	// Set canvas attributes
	style->SetCanvasBorderMode(0);
	style->SetCanvasBorderSize(0);
	style->SetCanvasColor(0);	
}	
