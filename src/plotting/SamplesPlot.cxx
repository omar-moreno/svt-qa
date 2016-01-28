/**
 *  @file   SamplesPlot.cxx
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   September 30, 2014
 *
 */

#include <SamplesPlot.h>

SamplesPlot::SamplesPlot(int n_plots, std::string name) 
    : sum_plot(NULL), 
      name(name),
      x_axis_title(""),
      y_axis_title(""),
     n_plots(n_plots) 
{ 

    for (int sample_n = 0; sample_n < this->getNumberOfPlots(); ++sample_n) {   
        std::string histo_name = this->getName() + " - Sample " + std::to_string(sample_n);
        plots.push_back(new TH2S(histo_name.c_str(), histo_name.c_str(), 640, 0, 640, 16384, 0, 16384));    
    }
}

SamplesPlot::~SamplesPlot() {

    // Destroy all plots stored in the map
    std::vector <TH2S*>::iterator plot_it = plots.begin(); 
    for (plot_it; plot_it != plots.end(); ++plot_it) { 
        delete *plot_it; 
    }
    plots.clear();

    if (sum_plot != NULL) delete sum_plot; 
}

void SamplesPlot::fill(int sample_n, int channel, double pedestal) { 
    plots[sample_n]->Fill(channel, pedestal);
}

void SamplesPlot::setXAxisTitles(std::string title) { 
    for (int plot_n; plot_n < plots.size(); ++plot_n) { 
        plots[plot_n]->GetXaxis()->SetTitle(title.c_str());
    }
    this->x_axis_title = title;
}

void SamplesPlot::setYAxisTitles(std::string title) { 
    for (int plot_n; plot_n < plots.size(); ++plot_n) { 
        plots[plot_n]->GetYaxis()->SetTitle(title.c_str()); 
    }
    this->y_axis_title = title;
}

TH2S* SamplesPlot::getPlot(int sample_n) { 
    return plots[sample_n]; 
}

TH2S* SamplesPlot::getSumOfPlots() { 

    if(sum_plot != NULL) return sum_plot; 

    std::string histo_name = this->getName(); 
    TH2S* sum_plot = new TH2S(histo_name.c_str(), 
            histo_name.c_str(), 640, 0, 640, 16384, 0, 16384); 
    for (int sample_n = 0; sample_n < plots.size(); ++sample_n) { 
        sum_plot->Add(plots[sample_n]); 
    }
    sum_plot->GetXaxis()->SetTitle(x_axis_title.c_str());
    sum_plot->GetYaxis()->SetTitle(y_axis_title.c_str());
    PlottingUtils::adjust2DPlotRange(sum_plot, 1);  
    return sum_plot; 
}

