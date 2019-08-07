/**
 *
 *
 */

#include "Phys2019SvtEvent.h" 

#include "TrackerSample.h" 

#include <iostream>
#include <bitset>

Phys2019SvtEvent::Phys2019SvtEvent() 
    : Data() { 
    }

Phys2019SvtEvent::~Phys2019SvtEvent() { 
}

void Phys2019SvtEvent::copy( uint* data, uint size ) { 

    int current_sample{(int)size - 1}; 
    while (current_sample != 0) {

        // Start by extracting the number of expected multi-samples
        // from the tail.
        if ( ((data[current_sample] >> 27) & 0x1F) == 0x15 ) {
            current_sample -= 3;
            int multiSampleCount = data[current_sample] & 0xFFF; 
            for (int imsample = 0; imsample < multiSampleCount; imsample++) {
                std::vector<uint> samples = { 0, 0, 0, 0};
                current_sample -=4;
                for (int iword{0}; iword < 4; iword++) { 
                    samples[iword] = data[current_sample + iword]; 
                }
                if (((samples[3] >> 30) & 0x1) == 1) { 
                    continue; 
                } 
                multisamples.push_back(samples); 
            }
            //std::cout << "Total number of multisamples extracted: " << multisamples.size() << std::endl;
        }
        current_sample -=1;  
    }
}

uint Phys2019SvtEvent::count() {
    return multisamples.size(); 
}

void Phys2019SvtEvent::clearSamples() { 
    multisamples.clear(); 
}

TrackerSample* Phys2019SvtEvent::sample(uint index) {
      sample_.setData(&multisamples[index][0]);
      return(&sample_);
}
