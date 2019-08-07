/**
 *
 */

#ifndef __PHYS2019_DATA_READ_EVIO_H__
#define __PHYS2019_DATA_READ_EVIO_H__

#include "AbstractDataReadEvio.h"

class Phys2019DataReadEvio : public AbstractDataReadEvio { 

	public:

		/**
		 */
		Phys2019DataReadEvio();
		
		/**
		 */
		~Phys2019DataReadEvio();

	private: 

		/**
		 */
		void processDataBank(Data*, uint16_t, std::vector<uint32_t>*);

		int getMaxRocBankTag();  

		int getMinRocBankTag();

        int getPhysicsBankTag(); 

}; // Phys2019DataReadEvio

#endif // __PHYS2019_DATA_READ_EVIO_H__
