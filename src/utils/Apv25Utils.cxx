/**
 *
 */

#include "Apv25Utils.h"

namespace 
{
    double sampling_time[] = {0, 25, 50, 75, 100, 125};

    TF1 shaper_fit("Shaping Function", "[0]*((x-[1])/[2])*exp(1-((x-[1])/[2]))", .5*25.0, 6*25.0);
//    TF1 shaper_fit("Shaping Function", "[0]*((x-[1])/[2])*exp(1-((x-[1])/[2]))", 1.5*25.0, 6*25.0);
    TGraph g_signal(6);
};

namespace Apv25Utils
{ 
    int getPhysicalChannel(int apv, int channel)
    {
        return (TOTAL_HYBRID_CHANNELS - 1) - (apv*TOTAL_APV25_CHANNELS + (TOTAL_APV25_CHANNELS - 1) - channel); 
    }

    void fitShaperSignal(std::string fit_type, double* adc, double* fit_params)
    {
        if(fit_type == "Root"){

            double max_sample = 0;

            // Fill the TGraph with the points to fit. Also, search for the
            // sample with the largest amplitude.
            for(int sample_n = 0; sample_n < TOTAL_SAMPLES; ++sample_n){
                if(adc[sample_n] > max_sample) max_sample = adc[sample_n]; 
                g_signal.SetPoint(sample_n, sampling_time[sample_n], adc[sample_n]); 
            }

            // Set some guesses for the fit parameters
            shaper_fit.SetParNames("Amp", "t0", "tp"); 
            shaper_fit.SetParameter("tp", 45.0);
            shaper_fit.SetParameter("t0", 10.0);
            shaper_fit.SetParameter("Amp", max_sample);

            // Fit the samples to an ideal CR-RC shape
            g_signal.Fit(&shaper_fit, "Rq");

            // Fill the fit parameter array
            fit_params[0] = shaper_fit.GetParameter("Amp");
            fit_params[1] = shaper_fit.GetParameter("tp"); 
            fit_params[2] = shaper_fit.GetParameter("t0");
            fit_params[3] = shaper_fit.GetChisquare()/shaper_fit.GetNDF();

        } else if(fit_type == "Analytic"){
             
            fit_params[0] = 0; 
            fit_params[1] = 0; 
            fit_params[2] = 0;
            fit_params[3] = 0;
        }
    }

    TF1 getShaperSignalFit()
    {
    	return shaper_fit;
    }
};

