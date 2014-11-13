/**
 *	@file		TestRunSvtSample.cxx
 *	@brief		HPS test run SVT sample container.
 *	@author		Omar Moreno <omoreno1@ucsc.edu>
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date		November 12, 2014
 *
 */

#include <TestRunSvtSample.h>

TestRunSvtSample::TestRunSvtSample() 
	: data(NULL) 
{}

TestRunSvtSample::TestRunSvtSample(uint* data) 
	: data(NULL)
{	
	std::memcpy(this->data, data, 16);	
}	
	
void TestRunSvtSample::setData(uint* data)
{
   this->data = data;
}

bool TestRunSvtSample::error()
{
   return((data[0]>>30)&0x1);
}

bool TestRunSvtSample::drop()
{
   return((data[0]>>27)&0x1);
}

uint TestRunSvtSample::hybrid()
{
   return((data[0]>>28)&0x3);
}

uint TestRunSvtSample::apv()
{
   return((data[0]>>24)&0x7);
}

uint TestRunSvtSample::channel()
{
   return((data[0]>>16)&0x7F);
}

uint TestRunSvtSample::fpgaAddress()
{
   return(data[0]&0xFFFF);
}

uint TestRunSvtSample::value(uint index)
{
   switch(index){
      case 0: return(data[1]&0xFFFF);
      case 1: return((data[1]>>16)&0xFFFF);
      case 2: return(data[2]&0xFFFF);
      case 3: return((data[2]>>16)&0xFFFF);
      case 4: return(data[3]&0xFFFF);
      case 5: return((data[3]>>16)&0xFFFF);
      default: return(0);
   }
}

