/**
 *  @file   svt_qa.cxx
 *  @brief  App used to process SVT EVIO events and Data events
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   May 5, 2014
 */

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <list>
#include <getopt.h>
#include <ctime>

//---------------//
//--- SVT DAQ ---//
//---------------//
#include <TriggerEvent.h>
#include <TriggerSample.h>
#include <DataRead.h>
#include <Data.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <DataReadEvio.h>
#include <BaselineAnalysis.h>
#include <SimpleBaselineAnalysis.h>
#include <OccupancyAnalysis.h>
#include <ThresholdAnalysis.h>
#include <CalibrationAnalysis.h>

using namespace std; 

void displayUsage(); 

int main(int argc, char **argv) {
    
    clock_t initial_time = clock(); 
    
    string input_file_name;
    string input_file_list_name;
    string run_threshold;
    bool run_baseline = false;
    bool run_occupancy = false; 
    bool evio = false; 
    bool readout_order = false;
    bool fix_apv_order = false; 
    int feb_id = -1; 
    int hybrid_id = -1; 
    int total_events = -1; 
    int calibration = -1;
    double threshold = -1;

    // Parse all the command line arguments. If there are no valid command line
    // arguments passed, print the usage and exit.
    static struct option long_options[] = {
        { "input",           required_argument, 0, 'i' },
        { "input_list",      required_argument, 0, 'l' },
        { "feb",             required_argument, 0, 'f' },
        { "hybrid",          required_argument, 0, 'h' },
        { "readout_order",   no_argument,       0, 'r' },
        { "fix_apv_order",   no_argument,       0, 'o' },     
        { "total_events",    required_argument, 0, 'n' },
        { "baseline",        no_argument,       0, 'b' },
        { "calibration",     required_argument, 0, 'c' },
        { "threshold",       required_argument, 0, 't' },
        { "occupancy",       no_argument,       0, 'p' },
        { "thres_analysis",  required_argument, 0, 'a' },
        { "evio",            no_argument,       0, 'e' },
        { "help",            no_argument,       0, 'u' }, 
        { 0, 0, 0, 0 }
    };

    int option_index = 0;
    int option_char; 
    while ((option_char = getopt_long(argc, argv, "i:l:f:h:ron:bct:peua:", long_options, &option_index)) != -1) {
        switch (option_char) {
            case 'i': 
                input_file_name = optarg; 
                break;
            case 'l': 
                input_file_list_name = optarg;
                break;
            case 'f': 
                feb_id = atoi(optarg);
                break;
            case 'h':
                hybrid_id = atoi(optarg);
                break;  
            case 'r':
                readout_order = true;
                break;
            case 'o':
                fix_apv_order = true; 
                break;
            case 'n':
                total_events = atoi(optarg); 
                break; 
            case 'b': 
                run_baseline = true; 
                break;
            case 'c': 
                calibration = atoi(optarg); 
                break;
            case 't':
                threshold = atof(optarg);
                break;
            case 'p':
                run_occupancy = true;
                break;
            case 'a': 
                run_threshold = optarg;
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
    if (input_file_name.empty() && input_file_list_name.empty()) {
        cerr << "\n[ SVT QA ]: Please specify a file or a list of files to process." << endl;
        cerr << "[ SVT QA ]: Use --help for usage.\n" << endl;
        return EXIT_FAILURE; 
    } else if (!input_file_name.empty() && !input_file_list_name.empty()) { 
        cerr << "\n[ SVT QA ]: Cannot specify both a input file name and a list of files." << endl;
        cerr << "[ SVT QA ]: Use --help for usage.\n" << endl;
        return EXIT_FAILURE; 
    }

    // Create the list of files to process
    list<string> files; 
    string file; 
    if(!input_file_name.empty()) { 
        files.push_back(input_file_name); 
    } else if (!input_file_list_name.empty()) { 
        ifstream input_file_list(input_file_list_name.c_str(), ifstream::in);
        if(!input_file_list.is_open()) { 
            cerr << "[ SVT QA ]: Failed to open file " << input_file_list << endl;
            return EXIT_FAILURE;
        }
        while (input_file_list >> file) { 
            files.push_back(file); 
        }
        input_file_list.close(); 
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
    
    // Container to hold all analyses 
    list<QAAnalysis*> analyses; 
    
    // TODO: All analyses should be loaded dynamically
    BaselineAnalysis* baseline_analysis = NULL;
    SimpleBaselineAnalysis* simple_baseline_analysis = NULL;
    OccupancyAnalysis* occupancy_analysis = NULL; 
    ThresholdAnalysis* threshold_analysis = NULL;
    CalibrationAnalysis* calibration_analysis = NULL; 
    if (run_baseline) {

        if (feb_id != -1 && hybrid_id != -1)
            baseline_analysis = new BaselineAnalysis(feb_id, hybrid_id); 
        else if (feb_id != -1) 
            baseline_analysis = new BaselineAnalysis(feb_id);
        else baseline_analysis = new BaselineAnalysis();    
        
        baseline_analysis->readoutOrder(readout_order); 
        analyses.push_back(baseline_analysis);

    } else if (threshold != -1) { 
    
        if (feb_id != -1 && hybrid_id != -1)
            simple_baseline_analysis = new SimpleBaselineAnalysis(feb_id, hybrid_id); 
        else if (feb_id != -1) 
            simple_baseline_analysis = new SimpleBaselineAnalysis(feb_id);
        else simple_baseline_analysis = new SimpleBaselineAnalysis();    
        
        simple_baseline_analysis->readoutOrder(readout_order);
        simple_baseline_analysis->setThreshold(threshold);  
        analyses.push_back(simple_baseline_analysis); 
    } else if (run_occupancy) { 
        if (feb_id != -1 && hybrid_id != -1)
            occupancy_analysis = new OccupancyAnalysis(feb_id, hybrid_id); 
        else if (feb_id != -1) 
            occupancy_analysis = new OccupancyAnalysis(feb_id);
        else occupancy_analysis = new OccupancyAnalysis();    
        analyses.push_back(occupancy_analysis); 
    } else if (!run_threshold.empty()) { 
        if (feb_id != -1 && hybrid_id != -1)
            threshold_analysis = new ThresholdAnalysis(feb_id, hybrid_id); 
        else if (feb_id != -1) 
            threshold_analysis = new ThresholdAnalysis(feb_id);
        else threshold_analysis = new ThresholdAnalysis();    
        threshold_analysis->setThresholdFile(run_threshold);
        analyses.push_back(threshold_analysis); 
    } else if (calibration != -1) { 
        if (feb_id != -1 && hybrid_id != -1)
            calibration_analysis = new CalibrationAnalysis(feb_id, hybrid_id);
        else if (feb_id != -1) 
            calibration_analysis = new CalibrationAnalysis(feb_id);
        else calibration_analysis = new CalibrationAnalysis(); 
        calibration_analysis->setCalibrationGroup(calibration);    
        analyses.push_back(threshold_analysis); 
    }

    // Initialize all QA analyses 
    for (list<QAAnalysis*>::iterator analysis = analyses.begin(); analysis != analyses.end(); ++analysis) {
        cout << "[ SVT QA ]: Initializing analysis: " << (*analysis)->toString() << endl;
        (*analysis)->initialize(); 
    }   
    
    int event_number = 0;
    // Loop over all input files and process them
    for (list<string>::iterator files_it = files.begin(); files_it != files.end(); ++files_it) { 
   
        // Open the input file.  If the input file can't be opened, exit the 
        // application
        cout << "Data reader state " << data_reader->open(*files_it, false) << endl;
        if (!data_reader->open(*files_it, false)) {
            cerr << "\n[ SVT QA ]: Error! File " << *files_it << " cannot be opened." << endl;
            return EXIT_FAILURE; 
        }
        
        cout << "[ SVT QA ]: Processing file: " << *files_it << endl;
        event_number = 0;  
        while (data_reader->next(&trigger_event)) {
            if (event_number == total_events) break; 
            ++event_number;

            for (list<QAAnalysis*>::iterator analysis = analyses.begin(); analysis != analyses.end(); ++analysis) {
                (*analysis)->processEvent(&trigger_event); 
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

void displayUsage() {
    cout << "\nUsage: svt_qa [OPTIONS] -i [INPUT_FILE]" << endl;
    cout << "Either a binary or EVIO INPUT_FILE must be specified.\n" << endl;
    cout << "OPTIONS:\n"
         << "\t -i Input binary or EVIO file name \n"
         << "\t -l List of binary or EVIO files to process \n"
         << "\t -f Front end board ID to process data from \n"
         << "\t -b Run the baseline analysis \n"
         << "\t --help Display this help and exit \n"
         << endl;
}
