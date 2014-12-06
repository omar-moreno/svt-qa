/**
 *  @file SvtCalibrations.h
 *  @brief Singleton class used to load and retrieve SVT calibrations
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *	        Santa Cruz Institute for Particle Physics
 *	        University of California, Santa Cruz
 *	@date   March 05, 2014
 */

#ifndef __SVT_CALIBRATIONS_H__
#define __SVT_CALIBRATIONS_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>

//--------------//
//--- SVT QA ---//
//--------------//
#include <SvtXmlReader.h>

class SvtCalibrations {

    public:

        /**
         *  Destructor
         */
        ~SvtCalibrations();

        /**
         *
         */
        static SvtCalibrations* getInstance();

        /**
         *
         */
        bool loadCalibrations(std::string);

        /**
         *
         */
        double getChannelBaseline(int, int, int);

        /**
         *
         */
        double getChannelNoise(int, int, int);

    private:

        /**
         *  Private Constructor.  This will keep the user from instantiating
         *  the class.
         */
        SvtCalibrations();

        // Singleton instance of this class
        static SvtCalibrations* instance;

        // The SVT XML reader
        SvtXmlReader* reader;

        // Maps used to store the calibrations for a specific FPGA/hybrid pair
        std::map<std::pair<int, int>, std::vector<double> > baseline_map;
        std::map<std::pair<int, int>, std::vector<double> > noise_map;

        // Flag indicating whether calibrations have been loaded
        bool calibrations_loaded;

        // File name from where the calibrations have been loaded from
        std::string xml_file_name;
};

#endif // _SVT_CALIBRATIONS_H_
