/**
 *	@file	PlottingUtils.h
 *	@brief	A compilation of plotting utilities.
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *			Santa Cruz Institute for Paritcle Physics
 *			University of California, Santa Cruz
 *	@date	2013
 *
 */

#ifndef __PLOTTING_UTILS_H__
#define __PLOTTING_UTILS_H__

#include <iostream>
#include <sstream>
#include <typeinfo>

//--- ROOT ---//
#include <TColor.h>
#include <TStyle.h>


namespace PlottingUtils { 


	/**
	 *	Setup the color palette used by ROOT
	 */
    void setPalette(); 
   
	/**
	 *
	 */
	TStyle* getStyle(); 

	/**
	 *	Convert a numerical value to a string.  This method should only be 
	 *	used when the C++11 to_string function is not available.
	 *
	 *	@parameter value Numerical value.
	 *	@return A string object representation of value
	 */
    template <typename T>
        std::string toString(const T value) {
            std::stringstream ss; 
            ss << value; 
            return ss.str(); 
        } 

	/**
	 *
	 */
	template <typename T>
		void initMulti1DPlots(int n_plots, std::vector<T*> &plots, std::string hist_name, int bins_x, int x_min, int x_max)
		{
            std::string h_name; 
            for(int plot_n = 0; plot_n < n_plots; ++plot_n){
                h_name = hist_name + "_" + toString<int>(plot_n); 
                plots.push_back(new T(h_name.c_str(), h_name.c_str(), bins_x, x_min, x_max));          
            }
		
		}	

	/**
	 *
	 */
    template <typename T> 
        void initMulti1DPlots(int n_plots, std::vector<T*> &plots, std::string hist_name, int x_min, int x_max)
        {        
            int bins_x = x_max - x_min;
			initMulti1DPlots(n_plots, plots, hist_name, bins_x, x_min, x_max); 
	   	}

	/**
	 *
	 */
    template <typename T>
    	void initMulti2DPlots(int n_plots, std::vector<T*> &plots, std::string hist_name,
    							 int bins_x, int x_min, int x_max, int bins_y, int y_min, int y_max)
    	{
    		std::string h_name;
    		for(int plot_n = 0; plot_n < n_plots; ++plot_n){
    			h_name = hist_name + "_" + toString<int>(plot_n);
    			plots.push_back(new T(h_name.c_str(), h_name.c_str(), bins_x, x_min, x_max, bins_y, y_min, y_max));
    		}
    	}

	/**
	 *
	 */
    template <typename T>
        void initMulti2DPlots(int n_plots, std::vector<T*> &plots, std::string hist_name, int x_min, int x_max, int y_min, int y_max)
        {
            int bins_x = x_max - x_min;
            int bins_y = y_max - y_min;
            initMulti2DPlots(n_plots, plots, hist_name, bins_x, x_min, x_max, bins_y, y_min, y_max);
        }

	/**
	 *
	 */
    template <typename T>
        void adjust1DPlotRange(T* &plot, int bin_threshold)
        {

            int histo_min = plot->FindFirstBinAbove(bin_threshold, 1);
            int histo_max = plot->FindLastBinAbove(bin_threshold, 1);
            plot->GetXaxis()->SetRange(histo_min, histo_max);
        }

	/**
	 *
	 */
    template <typename T>
        void adjust2DPlotRange(T* &plot, int bin_threshold)
        {


            int histo_x_min = plot->FindFirstBinAbove(bin_threshold, 1);
            int histo_x_max = plot->FindLastBinAbove(bin_threshold, 1);
            int histo_y_min = plot->FindFirstBinAbove(bin_threshold, 2);
            int histo_y_max = plot->FindLastBinAbove(bin_threshold, 2);
            plot->GetXaxis()->SetRange(histo_x_min, histo_x_max);
            plot->GetYaxis()->SetRange(histo_y_min, histo_y_max);

        }

	/**
	 *
	 */
    template <typename T>
        void set1DPlotStyle(T* &plot, std::string x_axis_title)
        {
            plot->GetXaxis()->SetLabelSize(0.03);
            plot->GetXaxis()->SetTitleSize(0.04);
            plot->GetXaxis()->SetTitleOffset(1.0);
            plot->GetYaxis()->SetLabelSize(0.03);
            plot->GetYaxis()->SetTitleSize(0.04);
            plot->GetYaxis()->SetTitleOffset(1.29);
            plot->GetXaxis()->SetTitle(x_axis_title.c_str());
            plot->SetTitle("");
        }

	/**
	 *
	 */
    template <typename T>
        void set2DPlotStyle(T* &plot, std::string x_axis_title, std::string y_axis_title)
        {
            set1DPlotStyle(plot, x_axis_title); 
            plot->GetYaxis()->SetTitle(y_axis_title.c_str());

        }


}; // PlottingUtils

#endif // __PLOTTING_UTILS_H__
