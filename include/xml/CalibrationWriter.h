/**
 *
 */

#ifndef __CALIBRATION_WRITER__
#define __CALIBRATION_WRITER__

//--------------//
//--- StdLib ---//
//--------------//
#include <string>

#include <SvtXmlWriter.h>

class CalibrationWriter : public SvtXmlWriter { 

	public:

		CalibrationWriter();
		~CalibrationWriter();

		void writeBaseline(int, int, int, int, int, double);
		void writeNoise(int, int, int, int, int, double);
};

#endif // __CALIBRATION_WRITER__
