/**
 *
 * @file    TestRunDataReadEvioTest.cxx 
 * @brief   Test to check the conversion from EVIO to Data objects.
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date    November 13, 2014
 *
 */

#include <getopt.h>
#include <stdlib.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <TestRunDataReadEvio.h>
#include <TestRunSvtEvent.h>
#include <TestRunSvtSample.h>

//---------------//
//--- SVT DAQ ---//
//---------------//
#include <Data.h>

using namespace std; 

int main(int argc, char **argv)	
{
	string evio_file_name;

	// Parse all the command line arguments. If there are no valid command line
	// arguments passed, print the usage and exit.
	static struct option long_options[] =  {
		{"input",    required_argument, 0, 'i'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	int option_char; 
	while ((option_char = getopt_long(argc, argv, "i:f:h:bu", long_options, &option_index)) != -1) {
		switch (option_char) {
			case 'i': 
				evio_file_name = optarg; 
				break;
			default:
				return EXIT_FAILURE;
		}
	}

	// If an EVIO input file was not specified, warn the user and 
	// exit the program
	if (evio_file_name.empty()) {
		cerr << "\n[ SVT QA ]: Please specify a file to process." << endl;
		cerr << "[ SVT QA ]: Use --help for usage.\n" << endl;
		return EXIT_FAILURE; 
	}

	DataRead* data_reader = new TestRunDataReadEvio();
	data_reader->open(evio_file_name, false);
	TestRunSvtSample* data_sample = new TestRunSvtSample();
	TestRunSvtEvent* data = new TestRunSvtEvent();

	while (data_reader->next(data)) {
		cout << "[ DataReadEvioTest ]:\t\tFpga: " << data->fpgaAddress() << endl;
	    	
		for(int temp_index = 0; temp_index < 12; ++temp_index){
			cout << "[ DataReadEvioTest ]:\t\tTemperature " << temp_index << ": " 
				 << data->temperature(temp_index) << endl; 
		}
		cout << "[ DataReadEvioTest ]:\t\tCount: " << data->count() << endl;
		
        for(uint sample_set_n = 0; sample_set_n < data->count(); ++sample_set_n){ 
			data->sample(sample_set_n, data_sample);
			cout << "[ DataReadEvioTest ]:\t\tFPGA:    " << data_sample->fpgaAddress() 
                 << "\tHybrid:  " << data_sample->hybrid() 
                 << "\tapv:	    " << data_sample->apv()
			     << "\tchannel: " << data_sample->channel() << endl;
			for(int sample_n = 0; sample_n < 6; ++sample_n){
				cout << "[ DataReadEvioTest ]:\t\t\tsample " << sample_n 
					 << ": " << data_sample->value(sample_n) << endl;
			}
		}
	}
	data_reader->close();
}
