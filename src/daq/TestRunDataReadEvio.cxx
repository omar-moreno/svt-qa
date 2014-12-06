/**
 *
 * @file	TestRunDataReadEvio.cxx
 * @brief	Test run EVIO to Data event converter.
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date	November 13, 2014
 *
 */


#include <TestRunDataReadEvio.h>

TestRunDataReadEvio::TestRunDataReadEvio() : AbstractDataReadEvio(1,3)
{}

TestRunDataReadEvio::~TestRunDataReadEvio()
{}

void TestRunDataReadEvio::processDataBank(Data* data_obj, uint16_t tag, std::vector<uint32_t>* data) {

    // The first word in the int data corresponds to the event number.
    // Erase it and replace it with the FPGA address.
    data->erase(data->begin());
    unsigned short fpga = tag & 0xFFFF;
    data->insert(data->begin(), fpga);

    // Copy the data to the data object
    data_obj->copy(&(*data)[0], data->size());    
}

int TestRunDataReadEvio::getMaxRocBankTag() { 
	return 3;
}

int TestRunDataReadEvio::getMinRocBankTag() { 
	return 3; 
}
