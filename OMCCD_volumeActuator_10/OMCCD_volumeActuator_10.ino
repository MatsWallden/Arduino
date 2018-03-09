
//LIBRARIES
#include "PID_v1.h"

// PINS
const int pinPosLim=7;
const int pwm_pin=5;
const int pinDirectionA=2;
const int pinDirectionB=8;
const int pinGoA=3;
const int pinGoB=9;

//PARAMETERS
int delayLength = 5;//Inverse of speed
int posMin=10;      //THIS IS THE LOWEST POSITION UNDER NORMAL OPERATION
int posMax=1280;    //THIS IS THE HIGHEST POSITION UNDER NORMAL OPERATION
int posLimLo=0;     //THIS IS THE LOWEST POSITION BEFORE THE MOTOR BREAKS
int posLimHi=1291;  //THIS IS THE HIGHEST POSITION BEFORE THE MOTOR BREAKS
double Kp=0.05;
double KI=0.05;
double KD=0.01;
double setPoint=330; //TEST AGAINST A DROP OF X µL FIRST
double posInitial=200; //THE INITIAL POSITION
int limMove=2; //HOW MANY STEPS DIFFERENT THAN LAST BEFORE MOVING? 

//VARIABLES
bool cond;
bool goDirA=false;
bool goDirB=true; //THIS IS NOT REQUIRED
int posCounter=1;
double input;
double output;
double newPos=0;
PID myPID(&input, &output, &setPoint,Kp,KI,KD, DIRECT);

//FUNCTIONS 
void getRange(); //CHECK ME: IS THIS NECESSARY?
//void goToPosition();
void setup() {
  
  //establish motor direction toggle pins
  pinMode(pinPosLim,INPUT);   //THIS PIN ALLOWS SENSING END OF THE RANGE
  pinMode(pwm_pin,INPUT);     //THIS PIN ALLOWS SENSING THE INPUT 
  pinMode(pinDirectionA, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(pinDirectionB, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  pinMode(pinGoA,OUTPUT);
  pinMode(pinGoB,OUTPUT);
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);

  //SENSE THE RANGE, MOVE DOWNWARDS TO EXTREMUS, UPWARD TO EXTREMUS
  Serial.println("######RANGING#####");
   getRange();

  //GO TO INITIAL POSITION
  goToPosition(posInitial);
 }

void loop(){
  //FIX ME AVERAGE THE SIGNAL OVER SEVERAL SAMPLES
  input= pulseIn(pwm_pin, HIGH); //READ SIGNAL FROM SAMPLE
  Serial.print(" input is "); Serial.println(input);
  myPID.Compute();  // COMPUTE STEERING SIGNAL ACCORDING TO PID
  Serial.print(" output is "); Serial.println(output);
  newPos=posMin + floor((output/255)*(posMax-posMin)); //FIX ME MUST DEAL WITH ADDED RANGE STUFF
  Serial.print(" new position is "); Serial.println(newPos);
  if(abs(newPos-posCounter)>limMove){goToPosition(newPos);}
}

//FUNCTIONS
void goToPosition(int posNew){
  goDirA=posCounter>posNew; //DETERMINE NEW DIRECTION
  while(posCounter!=posNew){
    goDirection();
    if(digitalRead(pinPosLim)){ //SENSES THE RANGE EXTREMUS
      if(goDirA){posCounter=0; //LOWER LIMIT, RESETS THE POSITION MINIMUM
      }else{posCounter=posLimHi; //UPPER LIMIT, RESETS THE POSITION MAXIMUM
      }//UPDATE POSITION INFORMATION
      goDirA=!goDirA; //DETERMINE NEW DIRECTION
      do{goDirection();}while(digitalRead(pinPosLim)); //MOVE AWAY FROM POSITION LIMIT
    }//END IF   
  } //END WHILE
} //END GOTOPOSITION


void getRange(){
  goDirA=true; //SET DIRECTION TO MOVE DOWNWARDS  
  while(!digitalRead(pinPosLim)){ goDirection();}//MOVE UNTIL AT THE EDGE OF RANGE 
  posCounter=0; //RESETS THE POSITION COUNTER
  goDirA=false; //SET DIRECTION TO MOVE UPWARDS  
  do{goDirection();}while(digitalRead(pinPosLim)); //MOVE CLEAR OF RANGE SENSOR
  while(!digitalRead(pinPosLim)){goDirection();} //END WHILE
  posLimHi=posCounter;  //RESETS THE HIGHEST AVAILABLE POSITION
  goDirA=true;
  do{goDirection();}while(digitalRead(pinPosLim)); //MOVE CLEAR OF RANGE SENSOR
  }//END FUNCTION GETRANGE

void goDirection(){
  //FIX ME! GLOBAL VARIABLES!!
  if(goDirA){moveDirectionA(); posCounter--;}
  else{moveDirectionB(); posCounter++;}  
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

