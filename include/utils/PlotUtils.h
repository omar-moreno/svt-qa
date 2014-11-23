/*
 *
 *
 *
 */

#ifndef _PLOT_UTILS_
#define _PLOT_UTILS_

//--- C++ ---//
#include <iostream>
#include <sstream>
#include <typeinfo>

//--- ROOT ---//
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TF1.h>
#include <TColor.h>
#include <TStyle.h>


namespace PlotUtils { 

    template <typename T>
        std::string toString(const T value)
        {
            std::stringstream ss; 
            ss << value; 
            return ss.str(); 
        } 

	template <typename T>
		void initMulti1DPlots(int n_plots, std::vector<T*> &plots, std::string hist_name, int bins_x, int x_min, int x_max)
		{
            std::string h_name; 
            for(int plot_n = 0; plot_n < n_plots; ++plot_n){
                h_name = hist_name + "_" + toString<int>(plot_n); 
                plots.push_back(new T(h_name.c_str(), h_name.c_str(), bins_x, x_min, x_max));          
            }
		
		}	

    template <typename T> 
        void initMulti1DPlots(int n_plots, std::vector<T*> &plots, std::string hist_name, int x_min, int x_max)
        {        
            int bins_x = x_max - x_min;
			initMulti1DPlots(n_plots, plots, hist_name, bins_x, x_min, x_max); 
	   	}

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

    template <typename T>
        void initMulti2DPlots(int n_plots, std::vector<T*> &plots, std::string hist_name, int x_min, int x_max, int y_min, int y_max)
        {
            int bins_x = x_max - x_min;
            int bins_y = y_max - y_min;
            initMulti2DPlots(n_plots, plots, hist_name, bins_x, x_min, x_max, bins_y, y_min, y_max);
        }

    template <typename T>
        void adjust1DPlotRange(T* &plot, int bin_threshold)
        {

            int histo_min = plot->FindFirstBinAbove(bin_threshold, 1);
            int histo_max = plot->FindLastBinAbove(bin_threshold, 1);
            plot->GetXaxis()->SetRange(histo_min, histo_max);
        }

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

    template <typename T>
        void set2DPlotStyle(T* &plot, std::string x_axis_title, std::string y_axis_title)
        {
            set1DPlotStyle(plot, x_axis_title); 
            plot->GetYaxis()->SetTitle(y_axis_title.c_str());

        }

    void setContours(); 
    void setupCanvas(TCanvas* &); 

};

#endif // _PLOT_UTILS_

/* 
   template <typename T> 
   static void initialize2DPlot(T* &, string, int, int, string, int, int, string);
   template <typename T> 
   static void initialize2DPlots(int, vector<T*> &, string, int, int, string, int, int, string);
   template<typename T>
   void drawHistogram(T*);         
   template<typename T>
   void draw2DPlot(T*);
   template<typename T>
   void drawMulti2DPlot(vector<T*> &);

//--- Setters ---//
//---------------//
void setPlotOptions(string);
void setBinThreshold(int, int, int); 
static void setPlotSavePath(string);
//        void setFileType(string);

static void savePlots(bool);

private:

static string plot_path;
string options;
//       string file_type;
int bin_threshold[3];

static bool save_plots; 
static TCanvas *canvas;
*/
