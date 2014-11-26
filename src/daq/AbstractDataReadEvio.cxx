/**
 *
 * @file    AbstractDataReadEvio.cxx
 * @brief   Abstract class containing common methods used in converting
 *          EVIO to Data objects.
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date    November 13, 2014
 *
 */

#include <AbstractDataReadEvio.h>

AbstractDataReadEvio::AbstractDataReadEvio(int physics_event_tag, int svt_bank_tag)
    :   file_channel(NULL), event(NULL), data_banks(new evioDOMNodeList), event_n(0),
        physics_event_tag(physics_event_tag), svt_bank_tag(svt_bank_tag)
{
}

AbstractDataReadEvio::~AbstractDataReadEvio()
{
    delete file_channel;
    delete event;
}

bool AbstractDataReadEvio::open(string evio_file_name, bool compressed = false) {
    
    try {

        // Create the EVIO file channel that will be used to read
        file_channel = new evioFileChannel(evio_file_name, "r");
        // Open the EVIO file channel 
        file_channel->open(); 

    } catch(evioException e){ 
        std::cerr << "[ AbstractDataReadEvio ]: Couldn't open file " 
            << evio_file_name << "." << std::endl;
        std::cerr << e.toString() << std::endl;
        return(EXIT_FAILURE);
    }
}

void AbstractDataReadEvio::close() {
    try {
        // Close the EVIO file channel
        file_channel->close(); 
    } catch(evioException e){ 
        std::cerr << "[ AbstractDataReadEvio ]: Couldn't close file." << std::endl; 
        std::cerr << e.toString() << std::endl;
    }
}


bool AbstractDataReadEvio::next(Data* data) {

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
        evioDOMNodeP physics_bank = event->getFirstNode(tagEquals(this->physics_event_tag));
        std::cout << "[ DataReadEvio ]:\tFound physics event " << std::endl;

        // Get the SVT bank encapsulated by the physics event.  
        // There should only be a single SVT bank.
        evioDOMNodeListP svt_banks = physics_bank->getChildren(tagEquals(this->svt_bank_tag));
        std::cout << "[ DataReadEvio ]: \tTotal number of SVT banks " << svt_banks->size() << std::endl;

        // Get the data banks (either FPGA or FEB) encapsulated by the SVT bank.
        data_banks = svt_banks->front()->getChildren();
        std::cout << "[ DataReadEvio ]: \tTotal number of data banks " << data_banks->size() << std::endl;

        // Get an iterator to the list of data banks
        data_iterator = data_banks->begin();

    } catch(evioException e) { 
        std::cerr << e.toString() << std::endl;
    }

    // Once the list of data banks from the physics event has been created, 
    // process each bank and create Data objects
    return this->next(data); 
}

evioDOMTree* AbstractDataReadEvio::getPhysicsEvent() {

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
        if((event->getFirstNode(tagEquals(this->physics_event_tag))) != NULL) return event;
        delete event;   
    }
}


