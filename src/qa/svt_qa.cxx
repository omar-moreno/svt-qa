/**
 *
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *	@date: May 5, 2014
 *
 */

//--- C++ ---//
//-----------//
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

//--- DAQ ---//
//-----------//
#include <TrackerEvent.h>
#include <TrackerSample.h>
#include <DataRead.h>
#include <Data.h>

using namespace std; 

int main(int argc, char **argv)
{
	string input_file_name;

	bool run_baseline = false;	

	// Parse all the command line arguments. If there are no valid command line
	// arguments passed, print the usage and exit.
	int option_char; 
	while((option_char = getopt(argc, argv, "i:b")) != -1){
		switch(option_char){
			case 'i': 
				input_file_name = optarg; 
				break;
			case 'b': 
				run_baseline = true; 
				break;
			default: 
				break;
		}
	}

	// If an input file (binary or EVIO) was not specified, exit the program
	if(input_file_name.length() == 0){
		cerr << "\nPlease specify a file to process." << endl;
		return EXIT_FAILURE; 
	}

	// 
	TrackerEvent *event = new TrackerEvent(); 
	TrackerSample *samples = NULL; 
	DataRead *data_reader = new DataRead(); 

	// Open the input file.  If the input file can't be opened, exit.
	if(!data_reader->open(input_file_name)){
		cerr << "\nError! File " << input_file_name << " cannot be opened." << endl;
		return EXIT_FAILURE; 
	}

	cout << "Processing file " << input_file_name << endl;

	int event_number = 0; 
	int channel;
	// Loop through all of the events until the end of file is reached
	while(data_reader->next(event)){
	
		++event_number;
		if(event_number%500 == 0) cout << "Processing event " << event_number << endl;

		// Loop through all the channels that have samples
		for(uint samples_n = 0; samples_n < event->count(); samples_n++){
					
			// Get the samples
			samples = event->sample(samples_n); 

			if(run_baseline){
			}	
		}
	}

	return EXIT_FAILURE;
}
