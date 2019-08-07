/**
 *  @file ThresholdProcessor.h
 *  @brief Processor that generates a pedestal and noise plots for all samples
 *         and generates a baseline file to load into the conditions DB.
 *  @author Omar Moreno, SLAC National Accelerator Laboratory
 *
 */

#include <ThresholdProcessor.h>

#include <cmath>
#include <stdio.h>

//---------//
//   DAQ   //
//---------//
//#include "TrackerEvent.h"
#include "TrackerSample.h"
#include "Phys2019SvtEvent.h" 

//------------//
//   svt-qa   //
//------------//
#include "QAUtils.h"
#include "TupleBuilder.h"


#include "TTree.h"
#include "TH2S.h"
#include "TH1D.h"
#include "TGraphErrors.h"

ThresholdProcessor::ThresholdProcessor(const std::string& name, Process& process)
    : Processor(name, process) {    
}

ThresholdProcessor::~ThresholdProcessor() {
}

void ThresholdProcessor::addThresholdHistogram(int feb_id, int hybrid_id) { 

    std::cout << "[ ThresholdAnalysis ]: Created baseline histogram for FEB: " 
              << feb_id << " Hybrid: " << hybrid_id << std::endl;

    std::string plot_title = "FEB: " + std::to_string(feb_id) + " Hybrid: " 
                                     + std::to_string(hybrid_id) + " Threshold";

    baseline_map[feb_id].push_back(new TH2S(plot_title.c_str(), plot_title.c_str(), 640, 0, 640, 16384, 0, 16384));
    baseline_map[feb_id][hybrid_id]->GetXaxis()->SetTitle("Physical Channel");
    baseline_map[feb_id][hybrid_id]->GetYaxis()->SetTitle("Threshold [ADC Counts]");
}

void ThresholdProcessor::initialize(TTree* tree) {

    for (int feb{0}; feb < 10; ++feb) {
        for (int hybrid{0}; hybrid < 4; ++hybrid) {
            this->addThresholdHistogram(feb, hybrid); 
        }
    }

    tfile.open("thresholds.dat"); 

    // Create a TupleBuilder instance.  The builder class is used to construct
    // an ntuple.
    //tuple_ = std::make_unique<TupleBuilder>(tree);

    // Add variables to the tuple
    /*tuple_->add<int>("event",    TupleBuilder::Int);
    tuple_->add<int>("feb",      TupleBuilder::Int);  
    tuple_->add<int>("hybrid",   TupleBuilder::Int); 
    tuple_->add<int>("apv",      TupleBuilder::Int); 
    tuple_->add<int>("pchannel", TupleBuilder::Int); 
    for (int isample{0}; isample < 6; ++isample) { 
        tuple_->add<int>("sample" + std::to_string(isample), TupleBuilder::Int); 
    } */
}

void ThresholdProcessor::process(Phys2019SvtEvent* event) {
    
    // Loop through the block of samples where a sample set consist of 4 32 bit
    // integers.  Here, event count returns the number of sample sets in an
    // event. Without baseline suppression, this will always be equal to 
    // 5x128 = 640 channels. 
    for (int isamples{0}; isamples < event->count(); ++isamples) {
        
        // Get a set of samples
        TrackerSample* samples = event->sample(isamples);
   
        int feb = samples->fpgaAddress(); 
        //std::cout << "FEB: " << feb << std::endl;
        //tuple_->set<int>("feb", feb);

        int hybrid = samples->hybrid();
        //std::cout << "Hybrid: " << hybrid << std::endl; 
        //tuple_->set<int>("hybrid",   hybrid); 

        //printSamples(samples); 
        int apv = samples->apv(); 
        /*if (feb == 0 || feb == 1) { 
            if (apv == 0) apv = 1; 
            else if (apv == 1) apv = 0; 
        }*/ 

        // Get the physical channel number 
        int channel = samples->channel(); 
        int pchannel = QAUtils::getPhysicalChannel(apv, channel);
        if (feb == 0 || feb == 1) pchannel = apv*128 + channel; 

        //tuple_->set<int>("apv",      apv); 
        //tuple_->set<int>("pchannel", pchannel); 

        for (int isample{1}; isample < 6; ++isample) {
            //tuple_->set<int>("sample" + std::to_string(isample), samples->value(isample)); 
            baseline_map[feb][hybrid]->Fill(pchannel, samples->value(isample)); 
        }
        
        //tuple_->tree()->Fill(); 
    }
}

void ThresholdProcessor::finalize() { 


    for (int feb{0}; feb < 10; ++feb) {
        for (int hybrid{0}; hybrid < 4; ++hybrid) {
            std::cout << "[ ThresholdProcessor ][ finalize ]: FEB " << feb 
                      << " Hybrid: " << hybrid << std::endl; 
            TH2S* baseline_plot = baseline_map[feb][hybrid];
            if (baseline_plot->GetEntries() == 0) { 
                std::cout << "[ ThresholdProcessor ][ finalize ]: FEB " << feb 
                          << " Hybrid: " << hybrid 
                          << " has no calibrations. Skipping ..." << std::endl;

                int max_apv = 5;
                if (feb == 0 || feb == 1) max_apv = 4;
                for (int iapv{0}; iapv < max_apv; ++iapv) {
                    std::cout << feb << " " <<  hybrid << " " << iapv << std::endl; 
                    tfile << feb << " " << hybrid << " " << iapv << " ";
                    for (int channel{0}; channel < 128; ++channel) {
                        tfile << std::hex << 17000 << setw(2) << " ";
                    }
                    tfile << "\n"; 
                }
                continue;  
            }
            processThresholdPlot(feb, hybrid, baseline_map[feb][hybrid]); 
        }
    }
    
    tfile.close(); 
}


void ThresholdProcessor::processThresholdPlot(int feb, int hybrid, TH2S* baseline_plot) {

    TH1D* projection = NULL;

    std::string file_name = "feb";
    if (feb < 10) file_name += "0";
    file_name += std::to_string(feb) + "_hybrid0" + std::to_string(hybrid);
    //output_file->mkdir(file_name.c_str());
    //output_file->cd(file_name.c_str());

    double mean_baseline = 0;
    double mean_baseline_error = 0; 
    double noise = 0;
    TGraph* g_mean_baseline = nullptr;
    TGraph* g_noise = nullptr;  
    std::string plot_title = "";
        int max_channel = 640; 
        if (feb == 0 || feb == 1) max_channel = 512; 

        plot_title = "FEB: " + std::to_string(feb) + " Hybrid: " + std::to_string(hybrid);
        //             " Mean Threshold - Sample " + std::to_string(sample_n);
        g_mean_baseline = new TGraphErrors(max_channel);
        g_mean_baseline->SetNameTitle(plot_title.c_str(), plot_title.c_str());


        plot_title = "FEB: " + std::to_string(feb) + " Hybrid: " + std::to_string(hybrid);
                     //" Noise - Sample " + std::to_string(sample_n);
        g_noise = new TGraphErrors(max_channel);    
        g_noise->SetNameTitle(plot_title.c_str(), plot_title.c_str());

        TH2S* baseline_sample_plot = baseline_plot; //->getPlot(sample_n);
        //baseline_sample_plot->Write();
        int max_apv = 5;
        if (feb == 0 || feb == 1) max_apv = 4; 
        int calibration [max_apv][128] = { 0 };   
        for (int channel = 0; channel < max_channel; ++channel) {

            if (baseline_plot->ProjectionY("", channel+1, channel+1)->GetEntries() == 0) continue; 
            TH1D* baseline_projection = baseline_plot->ProjectionY("", channel+1, channel+1);
            this->getCalibrations(baseline_projection, mean_baseline, mean_baseline_error, noise);
            baseline_projection->SetName(
                ("FEB: " + std::to_string(feb) + " Hybrid: " + std::to_string(hybrid)).c_str()); 

            g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
            g_noise->SetPoint(channel, channel, noise); 
            delete baseline_projection; 
            
            // Get the APV25 number from the physical channel number
            int apv = QAUtils::getApv25FromPhysicalChannel(channel);

            // Get the APV25 channel number from the physical channel number
            int apv_channel = QAUtils::getApv25ChannelFromPhysicalChannel(channel);
            
            if (feb == 0 || feb == 1) {
                apv =  (channel - apv_channel)/128; 
                //std::cout << feb << " " <<  hybrid << " " << apv << " " << " " << channel 
                //          << " " << apv_channel << std::endl;  
            }
            if (feb == 0 && hybrid == 3) { 
                calibration[apv][apv_channel] = mean_baseline + 1.7*noise;  
            //} else if (feb == 8) { 
            //    calibration[apv][apv_channel] = mean_baseline + 1.7*noise;  
            } else { 
                calibration[apv][apv_channel] = mean_baseline + 2*noise;  
            }
        }
       
        for (int iapv{0}; iapv < max_apv; ++iapv) {
            std::cout << feb << " " <<  hybrid << " " << iapv << " ";  
            tfile << feb << " " << hybrid << " " << iapv << " ";
            for (int ichannel{0}; ichannel < 128; ++ichannel) {
                std::cout << std::hex << calibration[iapv][ichannel] << setw(2) << " "; 
                tfile << std::hex << calibration[iapv][ichannel] << setw(2) << " ";
            }
            tfile << "\n";
            std::cout << "\n";  
        }

        g_mean_baseline->Draw("ap"); 
        g_mean_baseline->GetXaxis()->SetTitle("Channel"); 
        g_mean_baseline->GetYaxis()->SetTitle("Mean Threshold (ADC Counts)");
        g_mean_baseline->Write();
        
        g_noise->Draw("ap"); 
        g_noise->GetXaxis()->SetTitle("Channel"); 
        g_noise->GetYaxis()->SetTitle("Noise (ADC Counts)"); 
        g_noise->Write();
        
        delete g_mean_baseline; 
        delete g_noise;
    //}
    //output_file->cd();

}

void ThresholdProcessor::getCalibrations(TH1D* baseline_histogram, double &mean_baseline, double &mean_baseline_error,
                                       double &noise) {

    this->getSimpleCalibrations(baseline_histogram, mean_baseline, mean_baseline_error, noise);
    gaussian->SetRange(mean_baseline - 3*noise, mean_baseline + 3*noise); 
    baseline_histogram->Fit("gaussian", "RQ");
    mean_baseline = gaussian->GetParameter(1); 
    noise = gaussian->GetParameter(2);
}


void ThresholdProcessor::printSamples(TrackerSample* samples) { 

    std::cout << "[ ThresholdProcessor ]: \n"  
                  << "Hybrid: " << samples->hybrid() << "\n" 
                  << "APV: " << samples->apv() << "\n" 
                  << "Channel: " << samples->channel() << "\n" 
                  << "Samples: [ " 
                  << samples->value(0) << ", " 
                  << samples->value(1) << ", " 
                  << samples->value(2) << ", " 
                  << samples->value(3) << ", " 
                  << samples->value(4) << ", " 
                  << samples->value(5) << " ] " 
                  << std::endl;
}

void ThresholdProcessor::getSimpleCalibrations(TH1D* baseline_histogram, double &mean_baseline, 
                                             double &mean_baseline_error, double &noise) { 

    mean_baseline = baseline_histogram->GetMean();
    mean_baseline_error = baseline_histogram->GetMeanError();
    noise = baseline_histogram->GetRMS();
}

DECLARE_PROCESSOR(ThresholdProcessor);
