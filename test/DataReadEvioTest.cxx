/**
 *
 * @file    DataReadEvioTest.cxx 
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
#include <DataReadEvio.h>
#include <TriggerEvent.h>
#include <TriggerSample.h>

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
		cerr << "\n[ DataReadEvioTest ]: Please specify a file to process." << endl;
		cerr << "[ DataReadEvioTest ]: Use --help for usage.\n" << endl;
		return EXIT_FAILURE; 
	}

	DataRead* data_reader = new DataReadEvio();
	data_reader->open(evio_file_name, false);
	TriggerSample* data_sample = new TriggerSample();
	TriggerEvent* data = new TriggerEvent();

	while (data_reader->next(data)) {
		
        cout << "[ DataReadEvioTest ]:\t\tCount: " << data->count() << endl;
		
        for(uint sample_set_n = 0; sample_set_n < data->count(); ++sample_set_n){ 
			data->sample(sample_set_n, data_sample);
		}
	}
	data_reader->close();
}
