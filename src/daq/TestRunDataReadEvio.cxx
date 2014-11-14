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

TestRunDataReadEvio::TestRunDataReadEvio() : AbstractDataReadEvio(1,2)
{}

TestRunDataReadEvio::~TestRunDataReadEvio()
{}

void TestRunDataReadEvio::processDataBank(Data* data_obj, uint16_t tag, std::vector<uint32_t>* data)
{
	data->erase(data->begin(), data->begin()+2);

	// Create the FPGA tag and insert it into the front of the data list
	unsigned short fpga; 
	fpga = tag & 0xFFFF;
	data->insert(data->begin(), fpga);
	
	// Copy the data to the data object
	data_obj->copy(&(*data)[0], data->size());
}

