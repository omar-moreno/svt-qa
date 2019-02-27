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
 * to within ~2%. 
 * 
 * The humidity is calculated using the equation
 * 
 * RH = 0.0375*V_{out} - 37.7
 * 
 * Omar Moreno, SLAC National Accelerator Laboratory
 * 
 */

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
  // a voltage (0-5V)
  float voltage = sensorValue * (5.0/1023.0);

  // Calculate the relative humidity
  float RH = 0.0375*voltage - 37.7; 

  // Print the value
  Serial.println(RH);

}
