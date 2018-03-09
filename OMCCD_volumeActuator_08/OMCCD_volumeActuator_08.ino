
//LIBRARIES
#include "PID_v1.h"

// PINS
const int pwm_pin=5;
const int pinDirectionA=2;
const int pinDirectionB=8;
const int pinGoA=3;
const int pinGoB=9;

//PARAMETERS
int delayLength = 5;//Inverse of speed
int posMin=0;
int posMax=1000;
double Kp=0.1;
double KI=0.1;
double KD=0.0;
double setPoint=300;
double rangeLim=1000;

//VARIABLES
bool cond;
bool goDirA=false;
bool goDirB=true;
int posCounter=1;
double input;
double output;
double newPos=0;
PID myPID(&input, &output, &setPoint,Kp,KI,KD, DIRECT);

void setup() {
  
  //establish motor direction toggle pins

  pinMode(pwm_pin,INPUT);
  pinMode(pinDirectionA, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(pinDirectionB, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  pinMode(pinGoA,OUTPUT);
  pinMode(pinGoB,OUTPUT);
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);
 }

void loop(){
  //READ IN A VECTOR
  input= pulseIn(pwm_pin, HIGH);
  
  myPID.Compute();
  newPos=floor((output/255)*posMax);
  Serial.print(input);
  Serial.print(" ");
  Serial.println(newPos);
  cond=true;
  while(cond){
      if(posCounter>newPos && posCounter>posMin){
        moveDirectionA(); //MOVE DOWNWARDS
        posCounter--;
      }
      else if(posCounter<newPos && posCounter<posMax){
        moveDirectionB(); //MOVE UPWARDS
        posCounter++;
      }
      else{cond=false;}
  }
    delay(10);
  }

void moveDirectionA(){

  digitalWrite(pinDirectionA, HIGH);   //Sets direction of CH A
  analogWrite(pinGoA, 255);   //Moves CH A
  
  delay(delayLength);
  
  digitalWrite(pinDirectionB, LOW);   //Sets direction of CH B
  analogWrite(pinGoB, 255);   //Moves CH B
  
  delay(delayLength);
  
  digitalWrite(pinDirectionA, LOW);   //Sets direction of CH A
  analogWrite(pinGoA, 255);   //Moves CH A
  
  delay(delayLength);
  
  digitalWrite(pinDirectionB, HIGH);   //Sets direction of CH B
  analogWrite(pinGoB, 255);   //Moves CH B
  
  delay(delayLength);
};

void moveDirectionB(){
  
  digitalWrite(pinDirectionA, HIGH);   //Sets direction of CH A
  analogWrite(pinGoA, 255);   //Moves CH A
  
  delay(delayLength);
  
  digitalWrite(pinDirectionB, HIGH);   //Sets direction of CH B
  analogWrite(pinGoB, 255);   //Moves CH B
  
  delay(delayLength);

  digitalWrite(pinDirectionA, LOW);   //Sets direction of CH A
  analogWrite(pinGoA, 255);   //Moves CH A
  
  delay(delayLength);
    
  digitalWrite(pinDirectionB, LOW);   //Sets direction of CH B
  analogWrite(pinGoB, 255);   //Moves CH B
  
  delay(delayLength);

}

