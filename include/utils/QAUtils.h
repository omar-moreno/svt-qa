
#ifndef __QA_UTILS_H__
#define __QA_UTILS_H__

namespace QAUtils {

    // Total number of channels per APV
    const int APV25_CHANNELS  = 128; 
    // Total number of channels per hybrid
    const int HYBRID_CHANNELS = 640;
    
	/**
     *
     */
    int getPhysicalChannel(int, int);

};

#endif // __QA_UTILS_H__
