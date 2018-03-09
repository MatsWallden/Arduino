// from http://www.circuitbasics.com/arduino-ohm-meter/
#include "Timer.h" 
// sends a signal via the pin
//THIS IS FOR THE ARDUINO MOTOR SHIELD (NOT FOR VELLEMAN VM03 MOTOR SHELD)

int setPoint=200; //the reference signal
int analogPin= A2;
int e=0; //STEERING ERROR
int count=0;
int skipCount=100;
const int currentDirectionPin = 12; // the number of the currentDirectionPin pin// variables will change:
const int currentEngagePin = 3;
const int comPinA =2; //communication with the other Arduino 
const int comPinB =4; //communication with the other Arduino

Timer t;
int raw= 0;     //Raw is the measured value from A0. Is that a voltage?
int lastOne=1; //FIX ME! POOR NAMING
bool setDir=false;

void setup()
{
pinMode(currentDirectionPin, OUTPUT);
pinMode(currentEngagePin, OUTPUT);
pinMode(comPinA,OUTPUT);
pinMode(comPinB,OUTPUT);
Serial.begin(9600);
t.every(10,signalGeneration);
}

void loop(){
  t.update();
}

void signalGeneration(){

  if(lastOne){
    digitalWrite(currentDirectionPin, HIGH);
    analogWrite(currentEngagePin,255);  
    takeReading();
    lastOne=0;
  }
  else{
    digitalWrite(currentDirectionPin, LOW);
    analogWrite(currentEngagePin,255);
    if(count>skipCount){
    transmitDir(); //COMMUNICATE NEXT DIRECTION TO COUNTERPART
    }
    lastOne=1;
  } 
  count++;
  }

void takeReading(){
  raw=analogRead(analogPin); //JUST READS THE SIGNAL GOES TO 1 kHZ
  e=raw-setPoint;
  setDir= e > 0;
  }

void transmitDir(){
    //THIS FUNCTION TRANSMITS THE DIRECTION OF THE NEXT MOVE TO THE COUNTERPART
    
    if(setDir){
      digitalWrite(comPinA,HIGH);
      digitalWrite(comPinB,LOW);
    }else{
      digitalWrite(comPinB,HIGH);
      digitalWrite(comPinA,LOW);
    }
    count=0;
  }
  
