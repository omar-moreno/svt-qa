/**
 *  @file BaselineProcessor.h
 *  @brief Processor that create a tuple out of an SVT baseline run. 
 *  @author Omar Moreno, SLAC National Accelerator Laboratory
 *
 */

#include <BaselineProcessor.h>

BaselineProcessor::BaselineProcessor() {
}

BaselineProcessor::~BaselineProcessor() {
}

void BaselineProcessor::initialize() {

    // Event variables
    _tuple->addVariable("event"); 

    // Sample sets
    _tuple->addVariable("hybrid"); 
    _tuple->addVariable("apv");
    _tuple->addVariable("pchannel");
    _tuple->addVariable("rchannel");
    for (int isample = 0; isample < 6; ++isample) { 
        _tuple->addVariable("sample" + std::to_string(isample));
    }

    // Create the readout channel map
    for(int n = 0; n < 128; n++){
        int channel = (32*(n%4)) + (8*(n/4)) - (31*(n/16));
        channel_map[channel] = n; 
    }
}

void BaselineProcessor::process(TrackerEvent* event) {

    // Increment the event count
    _event_counter++;
    _tuple->setVariable("event", _event_counter);  

    /*std::cout << "[ BaselineProcessor ]: Event: " << _event_counter
              << std::endl;
    std::cout << "[ BaselineProcessor ]: Total samples: "  << event->count()
              << std::endl; */
    
    // Loop through all of the collected sets of samples.  Here event
    // count returns the number of sample collected in an event.  In our
    // case, this is equal to the number of channels.
    for (int sample_set = 0; sample_set < event->count(); ++sample_set) {
        
        // Get the set of samples from a channel.
        _samples = event->sample(sample_set);
     
        // Get the physical channel number 
        int pchannel = QAUtils::getPhysicalChannel(_samples->apv(), _samples->channel());

        // Get the channel number in readout order
        int rchannel = channel_map[_samples->channel()] + _samples->apv()*128; 

        // Fill the tuple 
        _tuple->setVariable("hybrid", _samples->hybrid()); 
        _tuple->setVariable("apv", _samples->apv());
        _tuple->setVariable("pchannel", pchannel);  
        _tuple->setVariable("rchannel", rchannel); 
        for (int isample = 0; isample < 6; ++isample) { 
            _tuple->setVariable("sample" + std::to_string(isample), 
                    _samples->value(isample)); 
        }
        _tuple->fill();

    }
}

void BaselineProcessor::finalize() {
    _tuple->close(); 
}

std::string BaselineProcessor::toString() {
    std::string string_buffer = "Class Name: " + class_name; 
    return string_buffer;   
}

void BaselineProcessor::printSamples(TrackerSample* samples) { 
        std::cout << "[ BaselineProcessor ]: \n"  
                  << "Hybrid: " << samples->hybrid() << "\n" 
                  << "APV: " << samples->apv() << "\n" 
                  << "Channel: " << samples->channel() << "\n" 
                  << "Samples: [ " 
                  << samples->value(0) << ", " 
                  << samples->value(1) << ", " 
                  << samples->value(2) << ", " 
                  << samples->value(3) << ", " 
                  << samples->value(4) << ", " 
                  << samples->value(5) << " ] " 
                  << std::endl;
}
