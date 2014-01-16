/**
 *
 *
 */

#ifndef _APV25_UTILS_
#define _APV25_UTILS_

//--- C++ ---//
#include <iostream>
#include <time.h>

//--- ROOT ---//
#include <TF1.h>
#include <TGraph.h>

namespace Apv25Utils
{ 

    // Total number of channels per APV
    const int TOTAL_APV25_CHANNELS  = 128; 
    // Total number of channels per hybrid
    const int TOTAL_HYBRID_CHANNELS = 640;
    // Total number of samples read out
    const int TOTAL_SAMPLES   = 6;  

    /**
     *
     */
    int getPhysicalChannel(int, int);
     
    /**
     *
     */
    void fitShaperSignal(std::string, double*, double*); 

    /**
     *
     */
    TF1 getShaperSignalFit();

};

#endif // _APV25_UTILS_
