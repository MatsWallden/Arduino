// from http://www.circuitbasics.com/arduino-ohm-meter/
#include "Timer.h" 
// sends a signal via the pin
//THIS IS FOR THE ARDUINO MOTOR SHIELD (NOT FOR VELLEMAN VM03 MOTOR SHELD)
//HOW DOES AVERAGING OF THE ERROR WORK?
int setPoint=200; //the reference signal
int analogPin= A2;
double e=0.0; //STEERING ERROR
double eSum=0.0;
int count=0;
int skipCount=20;
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
t.every(1,signalGeneration);
}

void loop(){
  t.update();
}

void signalGeneration(){

  if(lastOne){
    digitalWrite(currentDirectionPin, HIGH);
    analogWrite(currentEngagePin,255);  
    e=takeReading()-setPoint;
    eSum+=e;
    lastOne=0;
    count++;
  }
  else{
    digitalWrite(currentDirectionPin, LOW);
    analogWrite(currentEngagePin,255);
    if(count>skipCount){
      setDir=eSum/(count+1)>0.0;
      transmitDir(); //COMMUNICATE NEXT DIRECTION TO COUNTERPART
      count=0;
      eSum=0.0; 
    }
  
    /*else{
      //Serial.print(0);
      //Serial.print(" ");
      //Serial.println(abs(e02/(count+1)));
      //Serial.print("  ");
      //Serial.println(count+1);
    }*/
    
    lastOne=1;
  } 

  }

double takeReading(){
  //raw=analogRead(analogPin); //JUST READS THE SIGNAL GOES TO 1 kHZ
  //e=raw-setPoint;
  return analogRead(analogPin);//-setPoint;
  //Serial.println(count);   
  //setDir= e > 0;
  }

void transmitDir(){
    //THIS FUNCTION TRANSMITS THE DIRECTION OF THE NEXT MOVE TO THE COUNTERPART
    //Serial.println(count);
    if(setDir){
      digitalWrite(comPinA,HIGH);
      digitalWrite(comPinB,LOW);
    }else{
      digitalWrite(comPinB,HIGH);
      digitalWrite(comPinA,LOW);
    }
  }
  
