
#ifndef __QA_UTILS_H__
#define __QA_UTILS_H__

#include <cmath>

namespace QAUtils {

    // Total number of APV25's per hybrid
    const int APV25_PER_HYBRID = 5; 

    // Total number of channels per APV
    const int APV25_CHANNELS  = 128; 
    
    // Total number of channels per hybrid
    const int HYBRID_CHANNELS = APV25_PER_HYBRID*APV25_CHANNELS;
    
	/**
     *  Return the sensor strip channel number based on the physical location
     *  of the APV25 and the channel number.
     *
     *  @param apv : APV25 ID (0-4)
     *  @param channel: APV25 channel number (0-127)
     *  @return Physical channel number
     */
    int getPhysicalChannel(int apv, int channel);

    /**
     *  Return the APV25 ID from the physical channel number
     *
     *  @param channel : Physical channel number
     *  @return APV25 ID (0-4)
     */
    int getApv25FromPhysicalChannel(int channel);

    /**
     *  Return the APV25 channel number from the physical channel number
     *
     *  @param channel : Physical channel number
     *  @return APV25 channel number (0-127)
     */
    int getApv25ChannelFromPhysicalChannel(int channel); 

};

#endif // __QA_UTILS_H__
