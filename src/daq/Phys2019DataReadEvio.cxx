
#include "Phys2019DataReadEvio.h"
#include <bitset>

Phys2019DataReadEvio::Phys2019DataReadEvio() : AbstractDataReadEvio() {}

Phys2019DataReadEvio::~Phys2019DataReadEvio() {}

void Phys2019DataReadEvio::processDataBank(Data* data_obj, uint16_t tag, std::vector<uint32_t>* data) {  
    //
    //std::cout << "---- [ Phys2019DataReadEvio ][ processDataBank ]: Total payload: " << data->size() << std::endl;
    data_obj->copy(&(*data)[0], data->size()); 
}

int Phys2019DataReadEvio::getMaxRocBankTag() { 
    return 3;
}

int Phys2019DataReadEvio::getMinRocBankTag() { 
	return 2; 
}

int Phys2019DataReadEvio::getPhysicsBankTag() {
    //return 254;
    return 144;
}
