
#include <QAUtils.h>

int QAUtils::getPhysicalChannel(int apv, int channel)
{
	return (QAUtils::HYBRID_CHANNELS - 1) - 
				(apv*QAUtils::APV25_CHANNELS + (QAUtils::APV25_CHANNELS - 1) - channel); 
}
	
