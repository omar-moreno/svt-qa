/**
 *	SvtCalibrations.cpp
 *
 *		@author:  Omar Moreno <omoreno1@ucsc.edu>
 *		@version: v 1.0
 */

#include "SvtCalibrations.h"

SvtCalibrations* SvtCalibrations::instance = NULL;

bool SvtCalibrations::calibrations_loaded = false;

SvtCalibrations::~SvtCalibrations()
{
	delete instance;
}

SvtCalibrations* SvtCalibrations::getInstance()
{
	if(instance == NULL){
		instance = new SvtCalibrations();
		return instance;
	} else return instance;
}

bool SvtCalibrations::loadCalibrations(std::string baseline_path)
{
	// If the calibrations have already been loaded, don't bother
	// reloading them
	if(calibrations_loaded) return true;

	// Check if the specified file is valid.  Otherwise continue by
    // using the first sample as the pedestal
    std::ifstream baseline_file;
    baseline_file.open(baseline_path.c_str());
    if(!baseline_file){
    	std::cerr << "Unable to open " << baseline_path.c_str() << "!" << std::endl;
    	std::cerr << "First sample will be used as the pedestal." << std::endl;
    	return false;
    }
    
	// Loop through each line in the calibration file
    std::string line;
    int fpga, hybrid, channel;
    double baseline, noise;
	std::map<std::pair<int, int>, std::vector<double> >::iterator baseline_it;
    while(std::getline(baseline_file,line)){

    	// If an line is commented out, skip it
    	if(int(line.find("#")) == 0) continue;

    	// Extract the calibration values from the stream
    	std::istringstream stream(line);
    	stream >> fpga >> hybrid >> channel >> baseline >> noise;

		// Check if the maps contain the correct fpga/hybrid pair
    	// Load the baseline and noise maps with the calibration variables
    	std::pair<int, int> daq_pair = std::make_pair(fpga, hybrid);
		baseline_it = baseline_map.find(daq_pair); 
		if(baseline_it == baseline_map.end()){
    		baseline_map[daq_pair] = std::vector<double>(640);
    		noise_map[daq_pair] = std::vector<double>(640);
		}

    	baseline_map[daq_pair][channel] = baseline;
    	noise_map[daq_pair][channel] = noise;

		std::cout << "FPGA: " << fpga << " Hybrid: " << hybrid 
				  << " Channel: " << channel 
				  << " Baseline: " << baseline_map[daq_pair][channel] 
				  << std::endl;

//    	std::cout << "fpga: " << fpga << " hybrid: " << hybrid << " channel: " << channel << " baseline: " << baseline << " noise " << noise << std::endl;

    }

    std::cout << "Calibrations have been loaded." << std::endl;
    calibrations_loaded = true;
    return true;
}

double SvtCalibrations::getChannelBaseline(int fpga, int hybrid, int channel)
{
	return baseline_map[std::make_pair(fpga, hybrid)][channel];
}

double SvtCalibrations::getChannelNoise(int fpga, int hybrid, int channel)
{
	return noise_map[std::make_pair(fpga, hybrid)][channel];
}


