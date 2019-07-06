/**
 *  @file BaselineProcessor.h
 *  @brief Processor that create a tuple out of an SVT baseline run. 
 *  @author Omar Moreno, SLAC National Accelerator Laboratory
 *
 */

#include <BaselineProcessor.h>

//---------//
//   C++   //
//---------//
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
    : Processor(name, process), 
      baseline_map(10, std::vector< std::vector< TH2S* >>(4, std::vector< TH2S* >(6)))      
{    
}

BaselineProcessor::~BaselineProcessor() {
}

void BaselineProcessor::addBaselineHistogram(int feb_id, int hybrid_id) { 

    std::cout << "[ BaselineAnalysis ]: Created baseline histogram for FEB: " 
              << feb_id << " Hybrid: " << hybrid_id << std::endl;

    std::string prefix = "FEB: " + std::to_string(feb_id) + " Hybrid: " 
                                 + std::to_string(hybrid_id);

    for (int isample{0}; isample < 6; ++isample) {
        baseline_map[feb_id][hybrid_id][isample] =
            new TH2S((prefix + " Sample " + std::to_string(isample)).c_str(),
                     (prefix + " Sample " + std::to_string(isample)).c_str(),
                     640, 0, 640, 16384, 0, 16384); 
        baseline_map[feb_id][hybrid_id][isample]->GetXaxis()->SetTitle("Physical Channel");
        baseline_map[feb_id][hybrid_id][isample]->GetYaxis()->SetTitle("Baseline [ADC Counts]");
    }
}

void BaselineProcessor::initialize(TTree* tree) {

    for (int feb{0}; feb < 10; ++feb) {
        for (int hybrid{0}; hybrid < 4; ++hybrid) {
            this->addBaselineHistogram(feb, hybrid); 
        }
    }

    tfile.open("baseline.dat"); 

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
        if (feb == 0 || feb == 1) { 
            if (apv == 0) apv = 1; 
            else if (apv == 1) apv = 0; 
        } 

        // Get the physical channel number 
        int channel = samples->channel(); 
        int pchannel = QAUtils::getPhysicalChannel(apv, channel);
        if (feb == 0 || feb == 1) pchannel = apv*128 + channel; 

        //tuple_->set<int>("apv",      apv); 
        //tuple_->set<int>("pchannel", pchannel); 

        for (int isample{0}; isample < 6; ++isample) {
            //tuple_->set<int>("sample" + std::to_string(isample), samples->value(isample)); 
            baseline_map[feb][hybrid][isample]->Fill(pchannel, samples->value(isample)); 
        }
        
        //tuple_->tree()->Fill(); 
    }
}

void BaselineProcessor::finalize() { 

    int baseline[24576][6] = {0}; 
    int noise[24576][6] = {0};
    

    int channel_index = 0; 
    for (int feb{0}; feb < 10; ++feb) {
        int max_channel = 640; 
        if (feb == 0 || feb == 1) max_channel = 512; 
        for (int hybrid{0}; hybrid < 4; ++hybrid) {
            for (int sample{0}; sample < 6; ++sample) {

                int base[640]; 
                int n[640]; 
                std::cout << "[ BaselineProcessor ][ finalize ]: Extracting calibrations for " 
                          << "FEB: " << feb << " " 
                          << "Hybrid: " << hybrid << " " 
                          << "Sample: " << sample << std::endl; 
                TH2S* baseline_plot = baseline_map[feb][hybrid][sample];
                if (baseline_plot->GetEntries() == 0) {
                    continue;  
                }
                processBaselinePlot(feb, hybrid, base, n, 
                            baseline_map[feb][hybrid][sample]); 
                
                for (int channel{0}; channel < max_channel; ++channel) { 
                    baseline[channel_index + channel][sample] = base[channel]; 
                    noise[channel_index + channel][sample] = n[channel]; 
                }
            }
            channel_index += max_channel; 
        }
    }

    for (int id{0}; id < 24576; ++id) { 
        tfile << id 
              << " " << baseline[id][0] 
              << " " << baseline[id][1] 
              << " " << baseline[id][2]
              << " " << baseline[id][3] 
              << " " << baseline[id][4] 
              << " " << baseline[id][5]
              << " " << noise[id][0]
              << " " << noise[id][1] 
              << " " << noise[id][2]
              << " " << noise[id][3] 
              << " " << noise[id][4] 
              << " " << noise[id][5]
              << "\n"; 
    }

    tfile.close(); 
}


void BaselineProcessor::processBaselinePlot(int feb, int hybrid, int (&baseline)[640],
       int (&noise)[640], TH2S* baseline_plot) {

    TH1D* projection{nullptr}; 

    std::string file_name = "FEB " + std::to_string(feb) + " Hybrid " 
                                   + std::to_string(hybrid);
    //output_file->mkdir(file_name.c_str());
    //output_file->cd(file_name.c_str());

    double mean_baseline = 0;
    double mean_baseline_error = 0; 
    double noise_value = 0;
    TGraph* g_noise = nullptr;  
    
    int max_channel = 640; 
    if (feb == 0 || feb == 1) max_channel = 512; 

    std::string name = baseline_plot->GetName(); 
    TGraph* g_mean_baseline = new TGraphErrors(max_channel);
    g_mean_baseline->SetNameTitle((name + " - Mean Baseline").c_str(),
                                  (name + " - Mean Baseline").c_str());

    g_noise = new TGraphErrors(max_channel);    
    g_noise->SetNameTitle((name + " - Noise").c_str(),
                          (name + " - Noise").c_str());

    int max_apv = 5;
    if (feb == 0 || feb == 1) max_apv = 4; 
    //int calibration [max_apv][128] = { 0 };   
    for (int channel{0}; channel < max_channel; ++channel) {

        if (baseline_plot->ProjectionY("", channel+1, channel+1)->GetEntries() == 0) { 
            std::cout << baseline_plot->GetName() << " Channel: " << channel 
                      << " doesn't have any entries." << std::endl;
            continue; 
        }
        
        TH1D* baseline_projection = baseline_plot->ProjectionY("", channel+1, channel+1);
        this->getCalibrations(baseline_projection, mean_baseline, mean_baseline_error, noise_value);
        
        baseline[channel] = mean_baseline; 
        noise[channel] = noise_value; 
        //baseline_projection->SetName(
        //    ("FEB: " + std::to_string(feb) + " Hybrid: " + std::to_string(hybrid)).c_str()); 
        //" Sample: " + std::to_string(sample_n) + " Channel: " + std::to_string(channel)).c_str()); 

        //baseline_projection->Write();
        g_mean_baseline->SetPoint(channel,channel, mean_baseline); 
        g_noise->SetPoint(channel, channel, noise_value); 
        
        delete baseline_projection; 
            
        // Get the APV25 number from the physical channel number
        //int apv = QAUtils::getApv25FromPhysicalChannel(channel);

        // Get the APV25 channel number from the physical channel number
        //int apv_channel = QAUtils::getApv25ChannelFromPhysicalChannel(channel);
            
        //if (feb == 0 || feb == 1) {
        //   apv =  (channel - apv_channel)/128; 
            //std::cout << feb << " " <<  hybrid << " " << apv << " " << " " << channel 
            //          << " " << apv_channel << std::endl;  
        //}
        //calibration[apv][apv_channel] = mean_baseline + 2*noise;  
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
