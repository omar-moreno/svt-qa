/**
 *  @file BaselineProcessor.h
 *  @brief Processor that create a tuple out of an SVT baseline run. 
 *  @author Omar Moreno, SLAC National Accelerator Laboratory
 *
 */

#include <BaselineProcessor.h>

#include <cmath>
#include <stdio.h>

//---------//
//   DAQ   //
//---------//
#include "TrackerEvent.h"
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

BaselineProcessor::BaselineProcessor(const std::string& name, Process& process)
    : Processor(name, process) {    
}

BaselineProcessor::~BaselineProcessor() {
}

void BaselineProcessor::addBaselineHistogram(int feb_id, int hybrid_id) { 

    std::string plot_title = "FEB: " + std::to_string(feb_id) + " Hybrid: " + std::to_string(hybrid_id) + " Baseline";

    baseline_map[feb_id].push_back(
            new TH2S(plot_title.c_str(), plot_title.c_str(), 640, 0, 640, 16384, 0, 16384));
    baseline_map[feb_id][hybrid_id]->GetXaxis()->SetTitle("Channel");
    baseline_map[feb_id][hybrid_id]->GetYaxis()->SetTitle("Baseline [ADC Counts]");

    std::cout << "[ BaselineAnalysis ]: Created baseline histogram for FEB: " << feb_id 
              << " Hybrid: " << hybrid_id << std::endl;
}

void BaselineProcessor::initialize(TTree* tree) {

    for (int feb{0}; feb < 10; ++feb) {
        for (int hybrid{0}; hybrid < 4; ++hybrid) {
            this->addBaselineHistogram(feb, hybrid); 
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

//void BaselineProcessor::process(TrackerEvent* event) {
void BaselineProcessor::process(Phys2019SvtEvent* event) {
    
    ++event_count_;
    //tuple_->set<int>("event", event_count_);  
   
    //std::cout << "[ BaselineProcessor ]: Total samples: "  << event->count()
    //          << std::endl;
    
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
        //if (feb != 0 || feb != 1) apv = int(samples->apv() + 8)%5;
        //std::cout << "apv old: " << samples->apv() << std::endl;
        //std::cout << "apv: " << apv << std::endl;

        // Get the physical channel number 
        int channel = samples->channel(); 
        int pchannel = QAUtils::getPhysicalChannel(apv, channel);
        if (feb == 0 || feb == 1) pchannel = apv*128 + channel; 

        //tuple_->set<int>("apv",      apv); 
        //tuple_->set<int>("pchannel", pchannel); 

        for (int isample{0}; isample < 6; ++isample) {
            //tuple_->set<int>("sample" + std::to_string(isample), samples->value(isample)); 
            baseline_map[feb][hybrid]->Fill(pchannel, samples->value(isample)); 
        }
        
        //tuple_->tree()->Fill(); 
    }
}

void BaselineProcessor::finalize() { 


    for (int feb{0}; feb < 10; ++feb) {
        for (int hybrid{0}; hybrid < 4; ++hybrid) {
            std::cout << "[ BaselineProcessor ][ finalize ]: FEB " << feb 
                      << " Hybrid: " << hybrid << std::endl; 
            TH2S* baseline_plot = baseline_map[feb][hybrid];
            if (baseline_plot->GetEntries() == 0) { 
                std::cout << "[ BaselineProcessor ][ finalize ]: FEB " << feb 
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
            processBaselinePlot(feb, hybrid, baseline_map[feb][hybrid]); 
        }
    }
    
    tfile.close(); 
}


void BaselineProcessor::processBaselinePlot(int feb, int hybrid, TH2S* baseline_plot) {

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
    //for (int sample_n = 0; sample_n < 6; ++sample_n) { 

        plot_title = "FEB: " + std::to_string(feb) + " Hybrid: " + std::to_string(hybrid);
        //             " Mean Baseline - Sample " + std::to_string(sample_n);
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
                 //" Sample: " + std::to_string(sample_n) + " Channel: " + std::to_string(channel)).c_str()); 

            //baseline_projection->Write();
            g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
            g_noise->SetPoint(channel, channel, noise); 
            //writer->writeBaseline(feb, hybrid, channel, sample_n, mean_baseline);
            //writer->writeNoise(feb, hybrid, channel, sample_n, noise);
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
            calibration[apv][apv_channel] = mean_baseline + 3*noise;  
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
        g_mean_baseline->GetYaxis()->SetTitle("Mean Baseline (ADC Counts)");
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

void BaselineProcessor::getCalibrations(TH1D* baseline_histogram, double &mean_baseline, double &mean_baseline_error,
                                       double &noise) {

    this->getSimpleCalibrations(baseline_histogram, mean_baseline, mean_baseline_error, noise);
    gaussian->SetRange(mean_baseline - 3*noise, mean_baseline + 3*noise); 
    baseline_histogram->Fit("gaussian", "RQ");
    mean_baseline = gaussian->GetParameter(1); 
    noise = gaussian->GetParameter(2);
}


void BaselineProcessor::printSamples(TrackerSample* samples) { 

    std::cout << "[ BaselineProcessor ]: \n"  
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

void BaselineProcessor::getSimpleCalibrations(TH1D* baseline_histogram, double &mean_baseline, 
                                             double &mean_baseline_error, double &noise) { 

    mean_baseline = baseline_histogram->GetMean();
    mean_baseline_error = baseline_histogram->GetMeanError();
    noise = baseline_histogram->GetRMS();
}

DECLARE_PROCESSOR(BaselineProcessor);
