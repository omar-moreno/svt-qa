
#include <CalibrationWriter.h>

CalibrationWriter::CalibrationWriter() 
{}

CalibrationWriter::~CalibrationWriter()
{}

void CalibrationWriter::writeBaseline(int feb_id, int feb_hybrid_id, 
		int apv, int channel, int sample, double value)
{
	addChannelData("baseline", feb_id, feb_hybrid_id, apv, channel, sample, value);
}

void CalibrationWriter::writeNoise(int feb_id, int feb_hybrid_id, 
		int apv, int channel, int sample, double value)
{
	addChannelData("noise", feb_id, feb_hybrid_id, apv, channel, sample, value);
}

