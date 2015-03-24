
#include <QAUtils.h>

int QAUtils::getPhysicalChannel(int apv, int channel) {
	return (QAUtils::HYBRID_CHANNELS - 1) - 
			(apv*QAUtils::APV25_CHANNELS + (QAUtils::APV25_CHANNELS - 1) - channel); 
}

int QAUtils::getApv25FromPhysicalChannel(int channel) { 
    return 4 - floor(channel/128);  
}

int QAUtils::getApv25ChannelFromPhysicalChannel(int channel) { 
    return channel%128;
} 
