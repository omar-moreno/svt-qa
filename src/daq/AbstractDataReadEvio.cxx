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

AbstractDataReadEvio::AbstractDataReadEvio()
    :   event(NULL), file_channel(NULL),  data_banks(new evioDOMNodeList),
        roc_banks(new evioDOMNodeList), event_n(0)
{}

AbstractDataReadEvio::~AbstractDataReadEvio() {
    delete event;
    delete file_channel;
}

bool AbstractDataReadEvio::open(std::string evio_file_name, bool compressed = false) {
    
    try {

        // Instatiate the EVIO file channel and open the file for reading.
        // If the file can't be opened, an exception is thrown.
        file_channel = new evioFileChannel(evio_file_name, "r");
        file_channel->open(); 

    } catch(evioException e){ 
        // If the file can't be opened, warn the user and exit.
        std::cerr << "[ AbstractDataReadEvio ]: Couldn't open file " 
                  << evio_file_name << "." << std::endl;
        std::cerr << e.toString() << std::endl;
        return false;
    }

    return true; 
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

    // If the list of data banks isn't empty, process them
    if (!data_banks->empty()) {
        
        /*
        std::cout << "[ AbstractDataReadEvio ]: Processing data bank " 
                  << (*data_iterator)->tag << std::endl; 
        */

        // Use the data enclosed by the data bank to create a Data object 
        this->processDataBank(data, (*data_iterator)->tag, (*data_iterator)->getVector<uint32_t>());
        
        // Remove the data bank from the list of data banks
        data_iterator = data_banks->erase(data_iterator); 

        return true;
    }

    try { 

        // Get a physics event.  If the physics event is NULL, the end of the
        // EVIO file was reached.
        if ((event = this->getPhysicsEvent()) == NULL) return false;
       
        //data->setEventNumber(++event_n);
        
        /*
        std::cout << "[ AbstractDataReadEvio ]: --------------------------------------" << std::endl;
        std::cout << "[ AbstractDataReadEvio ]: --------------------------------------" << std::endl;
        std::cout << "[ AbstractDataReadEvio ]:\tEevent " << ++event_n << std::endl;
        std::cout << "[ AbstractDataReadEvio ]: --------------------------------------" << std::endl;
        */

        // Get the physics bank from the physics event.  There
        // should only be a single physics bank.
        evioDOMNodeP physics_bank = event->getFirstNode(tagEquals(this->getPhysicsBankTag()));
        
        /*
        std::cout << "[ AbstractDataReadEvio ]: Physics event with tag " 
                  << physics_bank->tag << " has been found" << std::endl;
        */

        // Delete all roc banks that were previously created
        while (!roc_banks->empty()) delete roc_banks->front(), roc_banks->pop_front();

        // Loop through all of the ROC banks encapsulated by the physics bank,
        // extract those which belong to the SVT and add them to a list of banks.
        // The ROC bank range is set in the subclass.  
        evioDOMNodeListP result;
        for (int roc_bank_tag = this->getMinRocBankTag(); 
                 roc_bank_tag <= this->getMaxRocBankTag(); ++roc_bank_tag) {
            
            /*
            std::cout << "[ AbstractDataReadEvio ]: Retrieving ROC bank: "
                      << roc_bank_tag << std::endl;
            */

            // Retrieve only ROC banks that match the ROC bank tag of interest
            result = physics_bank->getChildren(tagEquals(roc_bank_tag));
            
            /*
            std::cout << "[ AbstractDataReadEvio ]: Total ROC banks retrieved: "
                      << result->size() << std::endl;
            */

            roc_banks->insert(roc_banks->end(), result->begin(), result->end());
        }
        
        /*
        std::cout << "[ AbstractDataReadEvio ]: Total ROC banks found: " 
                  << roc_banks->size() << std::endl;
        */

        // Loop through each of the ROC banks and traverse them until the data
        // banks (either FPGA or RCE) are found.  This is done because the 
        // engineering run data is contained in a bank of banks.
        for (evioDOMNodeList::iterator roc_it = roc_banks->begin(); 
                roc_it != roc_banks->end(); ++roc_it) { 
            
            result = this->getDataBanks((*roc_it));
            
            /* 
            std::cout << "[ AbstractDataReadEvio ]: Total data banks retrieved: " 
                      << result->size() << std::endl;
            */

            data_banks->insert(data_banks->end(), result->begin(), result->end()); 
        }
        
        /*   
        std::cout << "[ AbstractDataReadEvio ]: Total number of data banks " 
                  << data_banks->size() << std::endl;
        */

        /// Get an iterator to the list of data banks
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
    while (true) {

        // Read an event from the EVIO channel.  If the end of the EVIO file 
        // has been reached, return NULL
        if(!file_channel->read()) return NULL;

        // Create the event tree from the contents of the channel
        evioDOMTree* event = new evioDOMTree(file_channel);

        // If the event is a physics event, return a pointer to the event tree.
        // Otherwise, destroy the object.
        if ((event->getFirstNode(tagEquals(this->getPhysicsBankTag()))) != NULL)
            return event;

        delete event;   
    }
}

evioDOMNodeListP AbstractDataReadEvio::getDataBanks(evioDOMNode* roc_bank) { 
   
    if (roc_bank->getChildList()->front()->isContainer()) {  
        return this->getDataBanks(roc_bank->getChildList()->front());  
    }

    evioDOMNodeListP result(new evioDOMNodeList);  
    for (evioDOMNodeList::iterator roc_it = roc_bank->getChildList()->begin(); 
            roc_it != roc_bank->getChildList()->end(); ++roc_it) { 

        // Ignore the trigger bank    
        if ((*roc_it)->tag == 16) continue; 

        result->push_back(*roc_it); 
    }

    return result;
}


