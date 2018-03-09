//circuit from http://www.circuitbasics.com/arduino-ohm-meter/
//AC GENERATED USING THE ARDUINO MOTOR SHIELD (NOT FOR VELLEMAN VM03 MOTOR SHELD)
//NOTE MAKE SURE THAT YOU UPLOAD TO THE ARDUINO YOU THINK YOU ARE
//LIBRARIES
#include "Timer.h" 
#include "PID_v1.h"


// PIN CONFIGURATION
const int currentDirectionPin = 12; // the number of the currentDirectionPin pin// variables will change:
const int currentEngagePin = 3;
const int comPinA =2; //communication with the other Arduino 
const int comPinB =4; //communication with the other Arduino
const int analogPin= A2;

//PARAMETERS
double setPoint=250; //the reference signal
int skipCount=1; //Measures 1 per skipCount
//double signalFrequency=0.0;
//double upDateEveryMillis=pow(2*signalFrequency,-1);
double updateEveryMillis=10; //THIS IS RELATED TO THE FREQUENCY
double Kp=0.01;
double KI=0.01;
double KD=1.00;
double input;
double output;
double outputLast;
double pwm=0.0;

//VARIABLES
int count=128;
int lastHalfPeriodWasNegative=1;
int numberTotalCycles=256;
int numberActiveCycles=numberTotalCycles;
int numberPassiveCycles=0; 
bool setDir=false;
Timer t;
PID myPID(&input, &output, &setPoint,2,5,1, DIRECT);

void setup()
{
  Serial.begin(9600); //NOT NEEDED
  pinMode(currentDirectionPin, OUTPUT);
  pinMode(currentEngagePin, OUTPUT);
  pinMode(comPinA,OUTPUT);
  pinMode(comPinB,OUTPUT);
  t.every(updateEveryMillis,signalGeneration);
  myPID.SetMode(AUTOMATIC);
  outputLast=0;
}

void loop(){
  t.update();
}

void signalGeneration(){ //GENERATES A SQUARE SINE WAVE

  if(lastHalfPeriodWasNegative){
    digitalWrite(currentDirectionPin, HIGH);
    analogWrite(currentEngagePin,255);
    if(count>=numberTotalCycles){
      input=takeReading();
      myPID.Compute();      
      //pwm=abs(output-outputLast);
      setDir=input-setPoint>0; //WHICH DIRECTION?
      numberActiveCycles=abs(output);
      count=0;
    } // END IF
    Serial.print(input);
    Serial.print(" ");
    Serial.println(output);
    lastHalfPeriodWasNegative=0;
    count++;
  } // END IF
  else{
    digitalWrite(currentDirectionPin, LOW);
    analogWrite(currentEngagePin,255);
    if(count<=numberActiveCycles){
      transmitDir(); //COMMUNICATE NEXT DIRECTION TO COUNTERPART
      } //END ELSE
    lastHalfPeriodWasNegative=1;    
  } //END ELSE

  } // END FUNCTION signalGeneration

double takeReading(){
  return analogRead(analogPin);
  }

void transmitDir(){
    //THIS FUNCTION TRANSMITS THE DIRECTION OF THE NEXT MOVE TO THE COUNTERPART
    if(setDir){ //FIX ME!! NO GLOBAL VARIABLES IN FUNCTIONS!!!
      digitalWrite(comPinA,HIGH);
      digitalWrite(comPinB,LOW);
    }else{
      digitalWrite(comPinB,HIGH);
      digitalWrite(comPinA,LOW);
    }
  }  
