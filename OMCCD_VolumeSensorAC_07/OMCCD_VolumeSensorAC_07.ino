//circuit from http://www.circuitbasics.com/arduino-ohm-meter/

#include "Timer.h" 
//AC GENERATEDO USING THE ARDUINO MOTOR SHIELD (NOT FOR VELLEMAN VM03 MOTOR SHELD)
//HOW DOES AVERAGING OF THE ERROR WORK?

// PIN CONFIGURATION
const int currentDirectionPin = 12; // the number of the currentDirectionPin pin// variables will change:
const int currentEngagePin = 3;
const int comPinA =2; //communication with the other Arduino 
const int comPinB =4; //communication with the other Arduino
const int analogPin= A2;

int setPoint=200; //the reference signal
int skipCount=20; //Measures 1 per skipCount
double updateEveryMillis=1;

double e=0.0; //STEERING ERROR
double eSum=0.0;
double eAve0=0.0;
int count=0;
Timer t;
int lastHalfPeriodWasNegative=1; 
bool setDir=false;

void setup()
{
  Serial.begin(9600); //NOT NEEDED
  pinMode(currentDirectionPin, OUTPUT);
  pinMode(currentEngagePin, OUTPUT);
  pinMode(comPinA,OUTPUT);
  pinMode(comPinB,OUTPUT);
  t.every(updateEveryMillis,signalGeneration);
}

void loop(){
  t.update();
}

void signalGeneration(){

  if(lastHalfPeriodWasNegative){
    digitalWrite(currentDirectionPin, HIGH);
    analogWrite(currentEngagePin,255);  
    e=takeReading()-setPoint;
    eSum+=e;
    lastHalfPeriodWasNegative=0;
    count++;
  }
  else{
    digitalWrite(currentDirectionPin, LOW);
    analogWrite(currentEngagePin,255);
    if(count>skipCount){ //SKIP
      setDir=eSum/(count+1)>0.0;
      transmitDir(); //COMMUNICATE NEXT DIRECTION TO COUNTERPART
      count=0;
      eSum=0.0; 
    } // END IF
    lastHalfPeriodWasNegative=1;    
  } //END IF

  } // END FUNCTION signalGeneration

double takeReading(){
  return analogRead(analogPin);
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
  }  
