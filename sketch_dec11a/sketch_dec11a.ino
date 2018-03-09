#include <WireIA.h>
#include <inttypes.h>
#include <Wire.h> //compiles 
#include <EEPROM.h> //compiles

IA theIA; //declares the IA object
void setup() {
// put your setup code here, to run once:

//set up serial
Serial.begin(9600);
Serial.println("PmodIA Demo");
Serial.println("Press 'r' when ready!");
while(Serial.read() !='r');

delay(20);
range(); //what does this do?

//test the frequency dependence of the gain factor
//if(testIA.getGF()){
 //Serial.println("perform a frequency sweep");}
//else{}

} //end setup

void loop() {
  // put your main code here, to run repeatedly:

}
