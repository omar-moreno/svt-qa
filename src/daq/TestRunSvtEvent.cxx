/**
 *	@file		TestRunSvtEvent.cxx
 *	@brief		HPS test run event container.
 *	@author		Omar Moreno <omoreno1@ucsc.edu>
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date		November 12, 2014
 *
 */

#include <TestRunSvtEvent.h>

void TestRunSvtEvent::update() {}

TestRunSvtEvent::TestRunSvtEvent() : Data() {
   double       temp;
   double       tk;
   double       res;
   double       volt;
   unsigned int idx;

   // Fill temperature lookup table
   temp = minTemp_;
   while ( temp < maxTemp_ ) {
      tk = k0_ + temp;
      //res = t25_ * exp(coeffA_+(coeffB_/tk)+(coeffC_/(tk*tk))+(coeffD_/(tk*tk*tk)));      
      res = constA_ * exp(beta_/tk);
      volt = (res*vmax_)/(rdiv_+res);
      idx = (uint)((volt / vref_) * (double)(adcCnt_-1));
      if ( idx < adcCnt_ ) tempTable_[idx] = temp; 
      temp += incTemp_;
   }

   // Fill new temperature lookup table
   temp = minTemp_;
   while ( temp < maxTemp_ ) {
      tk = k0_ + temp;
      //res = t25_ * exp(coeffA_+(coeffB_/tk)+(coeffC_/(tk*tk))+(coeffD_/(tk*tk*tk)));      
      res = constA_ * exp(beta_/tk);
      volt = (res*vmax_)/(rdiv_+res);
      idx = (uint)((volt / vrefNew_) * (double)adcCnt_);
      if ( idx < adcCnt_ ) tempTableNew_[idx] = temp; 
      temp += incTemp_;
   }
}

TestRunSvtEvent::~TestRunSvtEvent()
{}

bool TestRunSvtEvent::isTiFrame() {
   return((data_[0] & 0x80000000) != 0);
}

uint TestRunSvtEvent::fpgaAddress() {
   return(data_[0] & 0xFFFF);
}

uint TestRunSvtEvent::sequence() {
   return(data_[1]);
}

uint * TestRunSvtEvent::tiData() {
   return(&(data_[2]));
}


double TestRunSvtEvent::temperature(uint index) {
   uint adcValue;
   uint convValue;

   if ( isTiFrame () ) return(0.0);

   switch (index) {
      case  0: adcValue = (data_[2]&0xFFFF);
      case  1: adcValue = ((data_[2]>>16)&0xFFFF);
      case  2: adcValue = (data_[3]&0xFFFF);
      case  3: adcValue = ((data_[3]>>16)&0xFFFF);
      case  4: adcValue = (data_[4]&0xFFFF);
      case  5: adcValue = ((data_[4]>>16)&0xFFFF);
      case  6: adcValue = (data_[5]&0xFFFF);
      case  7: adcValue = ((data_[5]>>16)&0xFFFF);
      case  8: adcValue = (data_[6]&0xFFFF);
      case  9: adcValue = ((data_[6]>>16)&0xFFFF);
      case 10: adcValue = (data_[7]&0xFFFF);
      case 11: adcValue = ((data_[7]>>16)&0xFFFF);
      default: return(0.0);
   }

   if ( adcValue & 0x8000 ) convValue = ((adcValue >> 3) & 0xFFF);
   else convValue = adcValue & 0xFFF;

   return (tempTableNew_[convValue]);
}

uint TestRunSvtEvent::count() {
   if ( isTiFrame () ) return(0);
   else return((size_-(headSize_ + tailSize_))/sampleSize_);
}

void TestRunSvtEvent::sample(uint index, TestRunSvtSample* sample) {
	if( index < count() ) { 
      sample->setData(&(data_[headSize_ + (index*sampleSize_)]));
	}	
}

const double TestRunSvtEvent::coeffA_     = -1.4141963e1;
const double TestRunSvtEvent::coeffB_     =  4.4307830e3;  
const double TestRunSvtEvent::coeffC_     = -3.4078983e4;
const double TestRunSvtEvent::coeffD_     = -8.8941929e6;
const double TestRunSvtEvent::beta_       = 3750;
const double TestRunSvtEvent::constA_     = 0.03448533;
const double TestRunSvtEvent::t25_        = 10000.0;
const double TestRunSvtEvent::k0_         = 273.15;
const double TestRunSvtEvent::vmax_       = 2.5;
const double TestRunSvtEvent::vref_       = 2.5;
const double TestRunSvtEvent::vrefNew_    = 2.048;
const double TestRunSvtEvent::rdiv_       = 10000;
const double TestRunSvtEvent::minTemp_    = -50;
const double TestRunSvtEvent::maxTemp_    = 150;
const double TestRunSvtEvent::incTemp_    = 0.01;

const unsigned int TestRunSvtEvent::headSize_   = 7;
const unsigned int TestRunSvtEvent::tailSize_   = 1;
const unsigned int TestRunSvtEvent::sampleSize_ = 4;

