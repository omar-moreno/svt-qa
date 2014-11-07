/**
 *
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date:   October 16, 2014
 *
 */

#include <DataReadEvio.h>

DataReadEvio::DataReadEvio()
	: file_channel(NULL)
{}

DataReadEvio::~DataReadEvio()
{
	delete file_channel;
}

bool DataReadEvio::next(Data *data)
{

}

bool DataReadEvio::open(string evio_file_name, bool compressed = false)
{
	try {

		// Create the EVIO file channel that will be used to read
		file_channel = new evioFileChannel(evio_file_name, "r");
		// Open the file 
		file_channel->open(); 

	} catch(evioException e){ 
	
		std::cerr << "[ DataReadEvio ]: Couldn't open file " 
				  << evio_file_name << "." << std::endl;
		std::cerr << e.toString() << std::endl;
		return(EXIT_FAILURE);
	}

}

void DataReadEvio::close()
{
	try { 
		file_channel->close(); 
	} catch(evioException e){ 
		std::cerr << "[ DataReadEvio ]: Couldn't close file." << std::endl; 
		std::cerr << e.toString() << std::endl;
	}
}
