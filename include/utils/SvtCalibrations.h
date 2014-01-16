/**
 *	SvtCalibrations.h
 *
 *		@author:  Omar Moreno <omoreno1@ucsc.edu>
 *		@version: v 1.0
 */

#ifndef _SVT_CALIBRATIONS_H_
#define _SVT_CALIBRATIONS_H_

//--- C++ ---//
//-----------//
#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>

class SvtCalibrations {

public:

	~SvtCalibrations();

	static SvtCalibrations* getInstance();
	bool loadCalibrations(std::string);
	double getChannelBaseline(int, int, int);
	double getChannelNoise(int, int, int);

  private:

	// Private ctor to keep a user from instantiating the class
	SvtCalibrations(){};

	//
	static SvtCalibrations* instance;

	// Maps used to store the calibrations for a specific FPGA/hybrid pair
    std::map<std::pair<int, int>, std::vector<double> > baseline_map;
    std::map<std::pair<int, int>, std::vector<double> > noise_map;

    //
    static bool calibrations_loaded;
};

#endif // _SVT_CALIBRATIONS_H_
