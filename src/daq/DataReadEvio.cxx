/**
 *
 * @file	DataReadEvio.cxx
 * @brief	EVIO to Data event converter used for the engineering run
 *          and beyond.
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date:   October 16, 2014
 *
 */

#include <DataReadEvio.h>

DataReadEvio::DataReadEvio() : AbstractDataReadEvio(1,3) {}

DataReadEvio::~DataReadEvio() {}

void DataReadEvio::processDataBank(Data* data_obj, uint16_t tag, std::vector<uint32_t>* data) {
	 
    // Copy the data to the data object
	data_obj->copy(&(*data)[0], data->size());
}

int DataReadEvio::getMaxRocBankTag() { 
	return 17;
}

int DataReadEvio::getMinRocBankTag() { 
	return 12; 
}
