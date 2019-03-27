/*
 * htm2500lf
 * 
 * Reads the digitized analog output voltage associated with the
 * humidity reading taken by the HTM2500LF.  The humidity sensor 
 * output is assumed to be connected to A0. 
 * 
 * When using an Arduino Leonardo, the analog reading is digitized 
 * using a 10 bit ADC. The value is converted back to a voltage before
 * the humidity value is calculated.  This allows reading the humidity
 * to within ~2% between 10% and 95%.  Below and above those regions,  
 * the accuracy becomes ~10%.
 * 
 * The humidity is calculated using the equation
 * 
 * RH = -1.92e-9*V_{out}^3 + 1.44e-5*V_{out}^2 + 3.4e-3*V_{out} - 12.4
 * 
 * where V_{out} is in mV.
 * 
 * Omar Moreno, SLAC National Accelerator Laboratory
 * 
 */

#include <math.h>

// The setup routine runs once when reset is pressed
void setup() {

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

}

// Loop routine
void loop() {

  // Read the digitized output voltage on analog pin 0
  int sensorValue = analogRead(A0);

  // Convert the analog reading (which goes from 0-1023) to
  // a voltage (0-5000 mV)
  float voltage = sensorValue * (5.0/1023.0)*1000;

  // Calculate the Relative Humidity
  float RH = -1.92e-9*pow(voltage,3) + 1.44e-5*pow(voltage,2) + 3.4e-3*voltage - 12.4;

  // Print the value
  Serial.println(RH);

  /*
  sensorValue = analogRead(A1);
  float NTC = sensorValue * (5.0/1023.0);
  float a = 8.54942e-4;
  float b = 2.57305e-4;
  float c = 1.65368e-7;
  float T = 1/(a + b*log(NTC) + c*log(NTC)*log(NTC)*log(NTC));
  T = T - 273.15;

  Serial.println(NTC);
  Serial.println(T);
  */
   

}
