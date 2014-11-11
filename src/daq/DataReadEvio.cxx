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
	: file_channel(NULL), event(NULL), fpga_banks(new evioDOMNodeList), event_n(0)
{
}

DataReadEvio::~DataReadEvio()
{
	delete file_channel;
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
	}
}

bool DataReadEvio::next(Data *data)
{

	if(!fpga_banks->empty()) { 
		std::cout << "[ DataReadEvio ]: Processing FPGA Bank " << (*fpga_iterator)->tag << std::endl;
		std::cout << "[ DataReadEvio ]: type " << hex << (*fpga_iterator)->getContentType() << std::endl;
		std::cout << "[ DataReadEvio ]: " << (*fpga_iterator)->toString() << std::endl;
		this->processDataBank(data, (*fpga_iterator)->tag, (*fpga_iterator)->getVector<uint32_t>());
		fpga_iterator = fpga_banks->erase(fpga_iterator); 
		return true;
	}

	try { 


		if((event = this->getPhysicsEvent()) == NULL) return false;
		std::cout << "[ DataReadEvio ]: --------------------------------------" << std::endl;
		std::cout << "[ DataReadEvio ]:\tProcessing event " << ++event_n << std::endl;

		evioDOMNodeP physics_bank = event->getFirstNode(tagEquals(PHYSICS_EVENT_TAG));
		std::cout << "[ DataReadEvio ]:\tFound physics event " << std::endl;

		evioDOMNodeListP crate_banks = physics_bank->getChildren();
		std::cout << "[ DataReadEvio ]: \tTotal number of crate banks " << crate_banks->size() << std::endl;

		evioDOMNodeListP svt_banks = physics_bank->getChildren(tagEquals(SVT_BANK_TAG));
		std::cout << "[ DataReadEvio ]: \tTotal number of SVT banks " << svt_banks->size() << std::endl;
		fpga_banks = svt_banks->front()->getChildren();
		std::cout << "[ DataReadEvio ]: \tTotal number of FPGA banks " << fpga_banks->size() << std::endl;
		fpga_iterator = fpga_banks->begin();
		data_list.clear();
		std::cout << "[ DataReadEvio ]: --------------------------------------" << std::endl;
	
	
	} catch(evioException e) { 
		std::cerr << e.toString() << std::endl;
	}
	
	return this->next(data); 
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

//TestRunSvtEvent* DataReadEvio::processDataBank(uint16_t tag, std::vector<uint32_t>* data)
void DataReadEvio::processDataBank(Data* data_obj, uint16_t tag, std::vector<uint32_t>* data)
{
	unsigned short fpga; 
	fpga = tag&0xFFFF;
	data->insert(data->begin(), fpga);
	std::cout << "[ DataReadEvio ]: tag " << tag << std::endl; 
	std::cout << "[ DataReadEvio ]: fpga " << fpga << std::endl; 
	
	data_obj->copy(&(*data)[0], data->size());
	//data_list.push_back(tracker_event);
	
	//return tracker_event; 
}
