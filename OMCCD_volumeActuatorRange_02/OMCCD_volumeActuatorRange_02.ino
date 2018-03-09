
//LIBRARIES

// PINS
const int pinPosLim=7;
const int pinDirectionA=2;
const int pinDirectionB=8;
const int pinGoA=3;
const int pinGoB=9;

//PARAMETERS
int delayLength = 5;//Inverse of speed

//VARIABLES
bool posLim;
bool goDirA; //DOWNWARDS
int posCounter=1;
int posMax=0;
int count=1;

void setup() {  
  pinMode(pinPosLim,INPUT);
  pinMode(pinDirectionA, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(pinDirectionB, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  pinMode(pinGoA,OUTPUT);
  pinMode(pinGoB,OUTPUT);
  Serial.begin(9600);
  goDirA=true;
 }

void loop(){
    posLim=digitalRead(pinPosLim);
    Serial.println(posMax);
    
    if(posLim){
        //UPDATE POSITION INFORMATION
        if(goDirA){posCounter=0;}else{posMax=posCounter;}
        
        //DETERMINE NEW DIRECTION
        goDirA=!goDirA; 
        
        //MOVE AWAY FROM POSITION LIMIT
        do{goDirection();}while(digitalRead(pinPosLim));
        
    }else{
        goDirection();
        }   
}

void goDirection(){
  //FIX ME! GLOBAL VARIABLES!!
  if(goDirA){moveDirectionA(); posCounter--;}
  else{moveDirectionB(); posCounter++;}  
  }

void moveDirectionA(){
  // SLIDING ELEMENT MOVES TOWARDS THE MOTOR 
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
    // SLIDING ELEMENT MOVES AWAY FROM THE MOTOR 
  
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

