//circuit from http://www.circuitbasics.com/arduino-ohm-meter/
//AC GENERATED USING THE ARDUINO MOTOR SHIELD (NOT FOR VELLEMAN VM03 MOTOR SHELD)
//NOTE MAKE SURE THAT YOU UPLOAD TO THE ARDUINO YOU THINK YOU ARE
//THIS ONE WILL ONLY MEASURE AND SEND THE VALUE AS PWM

//LIBRARIES
#include "Timer.h" 

// PIN CONFIGURATION
const int pwmOutPin=5; //pwm pins 3, 5, 6, 9, 10, and 11 
const int currentDirectionPin = 12; // the number of the currentDirectionPin pin// variables will change:
const int currentEngagePin = 3;
const int analogPin=A3;

//PARAMETERS
double updateEveryMillis=1; //THIS IS RELATED TO THE FREQUENCY

//VARIABLES
int lastHalfPeriodWasNegative=1;
double input;
Timer t;

void setup()
{
  //Serial.begin(9600); //NOT NEEDED
  pinMode(currentDirectionPin, OUTPUT);
  pinMode(currentEngagePin, OUTPUT);
  pinMode(pwmOutPin,OUTPUT);
  t.every(updateEveryMillis,signalGeneration);
}

void loop(){
  t.update();
}

void signalGeneration(){ //GENERATES A SQUARE SINE WAVE

    if(lastHalfPeriodWasNegative){
      digitalWrite(currentDirectionPin, HIGH);
      analogWrite(currentEngagePin,255);
      input=takeReading();
      lastHalfPeriodWasNegative=0;
       } // END IF
    else{
      digitalWrite(currentDirectionPin, LOW);
      analogWrite(currentEngagePin,255);
      transmitReading(input);      
      lastHalfPeriodWasNegative=1;    
    } //END ELSE
  
    } // END FUNCTION signalGeneration

double takeReading(){
  return analogRead(analogPin);
  }

void transmitReading(double readIn){
  analogWrite(pwmOutPin,(int)readIn);
  }
