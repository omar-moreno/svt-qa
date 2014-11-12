/**
 *
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date:   October 16, 2014
 *
 */

#include <DataReadEvio.h>

const int DataReadEvio::PHYSICS_EVENT_TAG = 1;
const int DataReadEvio::SVT_BANK_TAG = 2;

DataReadEvio::DataReadEvio()
	: file_channel(NULL), event(NULL), data_banks(new evioDOMNodeList), event_n(0)
{
}

DataReadEvio::~DataReadEvio()
{
	delete file_channel;
	delete event;
	data_list.clear();

}

bool DataReadEvio::next(Data *data)
{

	if(!data_banks->empty()) { 
		std::cout << "[ DataReadEvio ]:\tProcessing data bank " << (*data_iterator)->tag << std::endl;
		// Use the data enclosed by the data bank to create a Data object 
		this->processDataBank(data, (*data_iterator)->tag, (*data_iterator)->getVector<uint32_t>());
		// Remove the data bank from the list of data banks
		data_iterator = data_banks->erase(data_iterator); 
		return true;
	}

	try { 

		// Get a physics event.  If the physics event is NULL, the end of the EVIO file was reached.
		if((event = this->getPhysicsEvent()) == NULL) return false;
		std::cout << "[ DataReadEvio ]: --------------------------------------" << std::endl;
		std::cout << "[ DataReadEvio ]: --------------------------------------" << std::endl;
		std::cout << "[ DataReadEvio ]:\tProcessing event " << ++event_n << std::endl;

		// Get the physics bank (tag == 1) from the physics event.  There
		// should only be a single physics bank.
		evioDOMNodeP physics_bank = event->getFirstNode(tagEquals(PHYSICS_EVENT_TAG));
		std::cout << "[ DataReadEvio ]:\tFound physics event " << std::endl;

		// Get the SVT bank (tag == 2) encapsulated by the physics event.  
		// There should only be a single SVT bank.
		evioDOMNodeListP svt_banks = physics_bank->getChildren(tagEquals(SVT_BANK_TAG));
		std::cout << "[ DataReadEvio ]: \tTotal number of SVT banks " << svt_banks->size() << std::endl;

		// Get the data banks (either FPGA or FEB) encapsulated by the SVT bank.
		data_banks = svt_banks->front()->getChildren();
		std::cout << "[ DataReadEvio ]: \tTotal number of data banks " << data_banks->size() << std::endl;

		// Get an iterator to the list of data banks
		data_iterator = data_banks->begin();

		// Clear the list of all previously created Data objects
		data_list.clear();

	} catch(evioException e) { 
		std::cerr << e.toString() << std::endl;
	}

	// Once the list of data banks from the physics event has been created, 
	// process each bank and create Data objects
	return this->next(data); 
}

bool DataReadEvio::open(string evio_file_name, bool compressed = false)
{
	try {

		// Create the EVIO file channel that will be used to read
		file_channel = new evioFileChannel(evio_file_name, "r");
		// Open the EVIO file channel 
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
		// Close the EVIO file channel
		file_channel->close(); 
	} catch(evioException e){ 
		std::cerr << "[ DataReadEvio ]: Couldn't close file." << std::endl; 
		std::cerr << e.toString() << std::endl;
	}
}

void DataReadEvio::processDataBank(Data* data_obj, uint16_t tag, std::vector<uint32_t>* data)
{
	// Create the FPGA tag and insert it into the front of the data list
	unsigned short fpga; 
	fpga = tag & 0xFFFF;
	data->insert(data->begin(), fpga);



	// Copy the data to the data object
	data_obj->copy(&(*data)[0], data->size());
}

evioDOMTree* DataReadEvio::getPhysicsEvent()
{
	// TODO: Check that the file channel is open before beginning to process 
	// events.  If it's not open, an exception should be thrown.
	while(true) {

		// Read an event from the EVIO channel.  If the end of the EVIO file 
		// has been reached, return NULL
		if(!file_channel->read()) return NULL;

		// Create the event tree from the contents of the channel
		evioDOMTree* event = new evioDOMTree(file_channel);

		// If the event is a physics event, return a pointer to the event tree.
		// Otherwise, destroy the object.
		if((event->getFirstNode(tagEquals(PHYSICS_EVENT_TAG))) != NULL) return event;
		delete event;	
	}
}
