// communicates with the AC board


const int pinDirectionA=2;
const int pinDirectionB=8;
const int pinGoA=3;
const int pinGoB=9;
const int readGoA=5; //read move direction A 
const int readGoB=4; //read move direction A
const int comPinA=4;
const int comPinB=5;
bool goDirA=false;
bool goDirB=true;
int comThresh=10;
int delayLength = 5;
int posCounter=0;

void moveDirectionA();
void moveDirectionB();

void setup() {
  
  //establish motor direction toggle pins
  pinMode(pinDirectionA, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(pinDirectionB, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  pinMode(pinGoA,OUTPUT);
  pinMode(pinGoB,OUTPUT);
  Serial.begin(9600);
  }

void loop(){

  //READ FROM THE PINS

  /*Serial.print(1023-analogRead(comPinA));
  Serial.print("  ");
  Serial.println(1023-analogRead(comPinB));  
  delay(1);*/
  goDirA=analogRead(comPinA)<comThresh;
  goDirB=analogRead(comPinB)<comThresh;
  
  if(goDirA && !goDirB){
    //Serial.println("MOVING DIR A");
    moveDirectionA(); 
  }
  else if(!goDirA && goDirB){
        //Serial.println("MOVING DIR B");
    moveDirectionB();
  }
  else{
    //Serial.println("STALL");
    delay(1);    
  }

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

