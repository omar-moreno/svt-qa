
#include <DataReadEvio.h>
#include <getopt.h>
#include <stdlib.h>

using namespace std; 

int main(int argc, char **argv)	
{
	string evio_file_name;
	
	// Parse all the command line arguments. If there are no valid command line
	// arguments passed, print the usage and exit.
	static struct option long_options[] = 
	{
		{"input",    required_argument, 0, 'i'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	int option_char; 
	while((option_char = getopt_long(argc, argv, "i:f:h:bu", long_options, &option_index)) != -1){
		switch(option_char){
			case 'i': 
				evio_file_name = optarg; 
				break;
			default:
				return EXIT_FAILURE;
		}
	}

	// If an EVIO input file was not specified, warn the user and 
	// exit the program
	if(evio_file_name.empty()){
		cerr << "\n[ SVT QA ]: Please specify a file to process." << endl;
		cerr << "[ SVT QA ]: Use --help for usage.\n" << endl;
		return EXIT_FAILURE; 
	}
	
	DataReadEvio* data_reader = new DataReadEvio();
	data_reader->open(evio_file_name, false);
	data_reader->close();
}
