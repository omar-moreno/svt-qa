
#include "PlotUtils.h"

namespace PlotUtils {

    void setContours()
    {
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

    void setupCanvas(TCanvas* &canvas)
    {
        canvas->SetFillColor(0);
        canvas->SetBorderMode(0);
        canvas->SetBorderSize(0);
        canvas->SetFrameFillColor(0);
        canvas->SetFrameBorderMode(0);
        canvas->SetBatch(0); 
    }
};



/*






    template<typename T> 
void PlotUtils::drawMulti2DPlot(vector<T*> &plots)
{
    // Get the number of plots in the vector
    int n_plots = plots.size(); 

    // Get the first and last bin above the user set threshold.
    // These values will be updated to accomadate the plot with the largest range
    int plot_range_min = plots[0]->FindFirstBinAbove(bin_threshold[1], 2);
    int plot_range_max = plots[0]->FindLastBinAbove(bin_threshold[1], 2);  

    // Do not save every plot when drawing. The final plot will be saved manually
    savePlots(false);

    // Loop through all the plots and draw them.  Also, adjust the plot
    // ranges so that they accomadate the plot with the largest range
    for(int plot_n = 0; plot_n < n_plots; ++plot_n){
        
        plots[plot_n]->SetMarkerStyle(31);
        plots[plot_n]->SetMarkerColor(plot_n + 2);
  
        setPlotOptions(""); 
        if(plot_n != 0) setPlotOptions("same");      
        draw2DPlot(plots[plot_n]);
    
        if(plots[plot_n]->FindFirstBinAbove(0, 2) < plot_range_min){ 
            plot_range_min = plots[plot_n]->FindFirstBinAbove(bin_threshold[1], 2); 
        }
        
        if(plots[plot_n]->FindLastBinAbove(0, 2) > plot_range_max){ 
                plot_range_max = plots[plot_n]->FindLastBinAbove(bin_threshold[1], 2);
        }
    }
    
    // Set the plot range
    plot_range_min -= 5;
    plot_range_max += 5; 
    plots[0]->GetYaxis()->SetRange(plot_range_min, plot_range_max);

    canvas->Update();

    savePlots(true); 
    string file_name = plot_path + plots[0]->GetName(); 
    //file_name += "." + file_type;
    //    string file_name = plots[0]->GetName(); 
        file_name += ".pdf";
    canvas->SaveAs(file_name.c_str()); 
}

template void PlotUtils::drawMulti2DPlot<TH2F>(vector<TH2F*> &);


template <typename T>
void PlotUtils::initialize2DPlot(T *&plot, string plot_name, int x_min, int x_max, string x_axis_title,
                                    int y_min, int y_max, string y_axis_title)
{
    int bins_x = x_max - x_min;
    int bins_y = y_max - y_min;
    plot 
        = new TH2F(plot_name.c_str(), plot_name.c_str(), bins_x, x_min, x_max, bins_y, y_min, y_max);
    plot->SetStats(0);
    plot->GetXaxis()->SetLabelSize(0.03);
    plot->GetXaxis()->SetTitleSize(0.03);
    plot->GetXaxis()->SetTitleOffset(1.2);
    plot->GetXaxis()->SetTitle(x_axis_title.c_str());
    plot->GetYaxis()->SetLabelSize(0.03);
    plot->GetYaxis()->SetTitleSize(0.03);
    plot->GetYaxis()->SetTitleOffset(1.4);
    plot->GetYaxis()->SetTitle(y_axis_title.c_str());
    plot->SetTitle("");
    

}

template void PlotUtils::initialize2DPlot<TH2F>(TH2F* &, string, int, int, string, int, int, string);

template <typename T> 
void PlotUtils::initialize2DPlots(int n_plots, vector<T*> &plots, string plot_name, 
                                    int x_min, int x_max, string x_axis_title, 
                                    int y_min, int y_max, string y_axis_title){ 

    T *plot; 
    for(int plot_n = 0; plot_n < n_plots; ++plot_n){ 
        initialize2DPlot<T>(plot, plot_name + "_" + convertToString(plot_n), 
                            x_min, x_max, x_axis_title, y_min, y_max, y_axis_title);
        plots.push_back(plot);
    }
}

template void PlotUtils::initialize2DPlots<TH2F>(int, vector<TH2F*> &, string, int, int, string, int, int, string);

void PlotUtils::setPlotOptions(string opt)
{
    options = opt;
}

void PlotUtils::setBinThreshold(int x_threshold, int y_threshold, int z_threshold)
{
    bin_threshold[0] = x_threshold; 
    bin_threshold[1] = y_threshold;
    bin_threshold[2] = z_threshold;
}

void PlotUtils::setPlotSavePath(string path)
{
   plot_path = path; 
}
*/

