/**
 *  @file SvtCalibrations.cxx
 *  @brief Singleton class used to load and retrieve SVT calibrations
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *	        Santa Cruz Institute for Particle Physics
 *	        University of California, Santa Cruz
 *	@date   March 05, 2014
 */

#include <SvtCalibrations.h>

SvtCalibrations* SvtCalibrations::instance = NULL;

SvtCalibrations::SvtCalibrations()
    : reader(new SvtXmlReader()),
      calibrations_loaded(false),
      xml_file_name("")
{}

SvtCalibrations::~SvtCalibrations() {
	delete instance;
	delete reader;
	baseline_map.clear();
	noise_map.clear();
}

SvtCalibrations* SvtCalibrations::getInstance() {

    if(instance == NULL){
		instance = new SvtCalibrations();
	}
	return instance;
}

bool SvtCalibrations::loadCalibrations(std::string xml_file_path) {

    // If the calibrations have already been loaded and the file name hasn't
    // changed, don't bother reloading them
	if(calibrations_loaded && this->xml_file_name == xml_file_name) return true;

	// Set the file name
	this->xml_file_name = xml_file_name;

    // Open the file using the SVT XML reader
    //reader->open(xml_file_name);

    // Get the list of FebNodes from the reader and iterate over them
    /*std::vector<FebNode*> feb_nodes = reader->getFebNodes();
	std::vector<FebNode*>::iterator feb_node = feb_nodes.begin();
	for (feb_node; feb_node != feb_nodes.end(); ++feb_node) {
	    for (int hybrid_id = 0; hybrid_id < 4; ++hybrid_id) {

	        // Check if the baseline map already contains the DAQ pair key.
	        // If it doesn't, add a container to contain the calibrations for
	        // that sensor
	        std::pair<int, int> daq_pair = std::make_pair((*feb_node)->getFebID(), hybrid_id);
	        if (baseline_map.find(daq_pair) == baseline_map.end()) {
	            baseline_map[daq_pair] = std::vector<double>(640);
	            noise_map[daq_pair] = std::vector<double>(640);
	        }

	        // Loop through all of the channels and samples and retrieve the
	        // calibrations
	        for (int channel = 0; channel < 640; ++channel) {
	            for (int sample = 0; sample < 6; ++sample) {
	                baseline_map[daq_pair][channel]
	                            = (*feb_node)->getHybridNode(hybrid_id)->getChannelData("baseline", channel, sample);
	                noise_map[daq_pair][channel]
	                            =(*feb_node)->getHybridNode(hybrid_id)->getChannelData("noise", channel, sample);
	            }
	        }
	    }
	}*/

    std::cout << "[ SvtCalibrations ]: Calibrations have been loaded." << std::endl;
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


