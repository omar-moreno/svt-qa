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
	: file_channel(NULL), event(NULL), fpga_banks(new evioDOMNodeList), event_n(0)
{
}

DataReadEvio::~DataReadEvio()
{
	delete file_channel;
}

bool DataReadEvio::next(Data *data)
{

	std::cout << "[ DataReadEvio ]: Processing Data " << std::endl;

	if(!fpga_banks->empty()) { 
		std::cout << "[ DataReadEvio ]: Processing FPGA Bank " << (*fpga_iterator)->tag << std::endl;
		std::cout << "[ DataReadEvio ]: type " << hex << (*fpga_iterator)->getContentType() << std::endl;
		std::cout << "[ DataReadEvio ]: " << (*fpga_iterator)->toString() << std::endl;
		this->processDataBank(data, (*fpga_iterator)->tag, (*fpga_iterator)->getVector<uint32_t>());
		fpga_iterator = fpga_banks->erase(fpga_iterator); 
		return true;
	}

	try { 

		// Check if the event has a physics bank
		evioDOMNodeP physics_bank = NULL;
		while(physics_bank == NULL){ 
			
			// Read an event from the channel	
			if(!file_channel->read()){
				std::cout << "[ DataReadEvio ]: Reached EOF." << std::endl;
				data_list.clear();
				return false; 
			}
	
			evioDOMTree tmp_event(file_channel);	
			physics_bank = tmp_event.getFirstNode(tagEquals(1));
		}

		if(event != NULL) delete event;
		event = new evioDOMTree(file_channel);	
		//evioDOMTree event(file_channel);

		std::cout << "[ DataReadEvio ]: --------------------------------------" << std::endl;
		std::cout << "[ DataReadEvio ]: Processing event " << ++event_n << std::endl;
		std::cout << "[ DataReadEvio ]: Found physics bank " << std::endl;

		physics_bank = event->getFirstNode(tagEquals(1));

		if(physics_bank->isContainer()){ 
			std::cout << "[ DataReadEvio ]: Physics bank is a container "<< std::endl;
		}

		std::cout << physics_bank->toString() << std::endl;
		
		evioDOMNodeListP crate_banks = physics_bank->getChildren();
		std::cout << "[ DataReadEvio ]: Total number of crate banks " << crate_banks->size() << std::endl;

		evioDOMNodeListP svt_banks = physics_bank->getChildren(tagEquals(2));
		std::cout << "[ DataReadEvio ]: Total number of SVT banks " << svt_banks->size() << std::endl;
		fpga_banks = svt_banks->front()->getChildren();
		std::cout << "[ DataReadEvio ]: Total number of FPGA banks " << fpga_banks->size() << std::endl;
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
	std::cerr << "[ DataReadEvio ]: tag " << tag << std::endl; 
	std::cerr << "[ DataReadEvio ]: fpga " << fpga << std::endl; 
	
	data_obj->copy(&(*data)[0], data->size());
	//data_list.push_back(tracker_event);
	
	//return tracker_event; 
}
