/**
 *  @file   svt_qa.cxx
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   May 5, 2014
 */

//--- C++ ---//
//-----------//
#include <iostream>
#include <stdlib.h>
#include <list>
#include <getopt.h>
#include <time.h>

//--- SVT DAQ ---//
//---------------//
#include <TriggerEvent.h>
#include <TriggerSample.h>
#include <DataRead.h>
#include <Data.h>

//--- SVT QA ---//
//--------------//
#include <DataReadEvio.h>
#include <BaselineAnalysis.h>

using namespace std; 

void displayUsage(); 

int main(int argc, char **argv) {
    
    clock_t initial_time = clock(); 
    
    string input_file_name;
    bool run_baseline = false;
    bool evio = false; 
    int feb_id = -1; 
    int hybrid_id = -1; 
    int total_events = -1; 

    // Parse all the command line arguments. If there are no valid command line
    // arguments passed, print the usage and exit.
    static struct option long_options[] = {
        { "input",          required_argument, 0, 'i' },
        { "feb",            required_argument, 0, 'f' },
        { "hybrid",         required_argument, 0, 'h' },
        { "total_events",   required_argument, 0, 'n' },
        { "baseline",       no_argument,       0, 'b' },
        { "evio",           no_argument,       0, 'e' },
        { "help",           no_argument,       0, 'u' }, 
        { 0, 0, 0, 0 }
    };
    int option_index = 0;
    int option_char; 
    while ((option_char = getopt_long(argc, argv, "i:f:h:n:beu", long_options, &option_index)) != -1) {
        switch (option_char) {
            case 'i': 
                input_file_name = optarg; 
                break;
            case 'f': 
                feb_id = atoi(optarg);
                break;
            case 'h':
                hybrid_id = atoi(optarg);
                break;  
            case 'n':
                total_events = atoi(optarg); 
                break; 
            case 'b': 
                run_baseline = true; 
                break;
            case 'e':
                evio = true; 
                break;
            case 'u':
                displayUsage();
                return EXIT_SUCCESS;
            default:
               displayUsage();  
                return EXIT_FAILURE;
        }
    }

    // If an input file (binary or EVIO) was not specified, warn the user and 
    // exit the application
    if (input_file_name.empty()) {
        cerr << "\n[ SVT QA ]: Please specify a file to process." << endl;
        cerr << "[ SVT QA ]: Use --help for usage.\n" << endl;
        return EXIT_FAILURE; 
    }

    // 
    TriggerEvent trigger_event;
    TriggerSample* trigger_samples = new TriggerSample(); 
    DataRead *data_reader = NULL; 
    if (evio) { 
        data_reader = new DataReadEvio();  
    } else { 
        data_reader = new DataRead(); 
    }
    
    // Open the input file.  If the input file can't be opened, exit the 
    // application
    if (!data_reader->open(input_file_name)) {
        cerr << "\n[ SVT QA ]: Error! File " << input_file_name << " cannot be opened." << endl;
        return EXIT_FAILURE; 
    }

    // Container to hold all analyses 
    list<QAAnalysis*> analyses; 

    cout << "[ SVT QA ]: Processing file: " << input_file_name << endl;

    // TODO: All analyses should be loaded dynamically
    if (run_baseline) {
        if (feb_id != -1 && hybrid_id != -1) 
            analyses.push_back(new BaselineAnalysis(feb_id, hybrid_id));
        else analyses.push_back(new BaselineAnalysis());    
    }

    // Initialize all QA analyses 
    for (list<QAAnalysis*>::iterator analysis = analyses.begin(); analysis != analyses.end(); ++analysis) {
        cout << "[ SVT QA ]: Initializing analysis: " << (*analysis)->toString() << endl;
        (*analysis)->initialize(); 
    }   
    
    int event_number = 0; 
    int channel;
    while (data_reader->next(&trigger_event)) {
        if (event_number == total_events) break; 
        ++event_number;

        if(event_number%500 == 0) 
            cout << "[ SVT QA ]: Processing event " << event_number << endl;

        for (uint sample_set_n = 0; sample_set_n < trigger_event.count(); ++sample_set_n) {
            
            trigger_event.sample(sample_set_n, trigger_samples);
                        
            for (list<QAAnalysis*>::iterator analysis = analyses.begin(); analysis != analyses.end(); ++analysis) {
                (*analysis)->processEvent(trigger_samples); 
            }   
        }
    }
  
    for (list<QAAnalysis*>::iterator analysis = analyses.begin(); analysis != analyses.end(); ++analysis) {
        (*analysis)->finalize(); 
        delete *analysis; 
    }   
    analyses.clear();

    cout << "Total run time: " << ((float) (clock() - initial_time))/CLOCKS_PER_SEC
         << " s" << endl;
    
    return EXIT_SUCCESS;
}

void displayUsage()
{
    cout << "\nUsage: svt_qa [OPTIONS] -i [INPUT_FILE]" << endl;
    cout << "Either a binary or EVIO INPUT_FILE must be specified.\n" << endl;
    cout << "OPTIONS:\n"
         << "\t -i Input binary or EVIO file name \n"
         << "\t -b Run the baseline analysis \n"
         << "\t --help Display this help and exit \n"
         << endl;
}
