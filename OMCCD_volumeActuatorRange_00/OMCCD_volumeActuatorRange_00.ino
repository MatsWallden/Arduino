
//LIBRARIES

// PINS
const int pinPosLim=2;
const int pinDirectionA=2;
const int pinDirectionB=8;
const int pinGoA=3;
const int pinGoB=9;

//PARAMETERS
int delayLength = 5;//Inverse of speed
int posMin=0;
int slipOutPosLim=50; 

//VARIABLES
bool posLim;
bool cond;
bool goDirA; //UPWARDS
bool goDirB; //
int posCounter=1;
int posMax=0;
int slipCount=0;

void setup() {
  
  pinMode(pinPosLim,INPUT);
  pinMode(pinDirectionA, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(pinDirectionB, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  pinMode(pinGoA,OUTPUT);
  pinMode(pinGoB,OUTPUT);
  Serial.begin(9600);
  goDirA=true;
  goDirB=false; //start with downwards
  cond=true;
 }

void loop(){
    
     posLim=digitalRead(pinPosLim);
    
    if(posLim){ // A POSITION LIMIT IS REACHED    
      if(goDirA && !goDirB){// GOING DOWNWARDS
        posCounter=0; //SET THE POSITION TO LOWEST
        goDirB=true; goDirA=false; //CHANGE DIRECTION TO UPWARD
        delay(5);
        for(int ii=0; ii<slipOutPosLim; ii++){          
          moveDirectionB(); 
          Serial.println("MOVING UPWARDS FROM PIN");
          posCounter++;
        }
        cond=true;        
      }
      else if(!goDirA && goDirB){// GOING UPWARDS 
        posMax=posCounter; //SETTING THE UPPER LIMIT
        goDirB=false; goDirA=true; //CHANGE DIRECTION TO DOWNWARDS
        delay(5);
        for(int ii=0; ii<slipOutPosLim; ii++){
          moveDirectionA(); 
          Serial.println("MOVING DWNWARDS FROM PIN");
          posCounter--;
        }
        cond=true;
      }      
    } // END IF POSLIM
    
    if(goDirA && !goDirB){ // MOVE DOWNWARDS
          moveDirectionA();
          posCounter--;
    }
    else if(!goDirA && goDirB){ //MOVE UPWARDS
          moveDirectionB();
          posCounter++;
    }
    else{Serial.println("WTF??");}        
    //slipCount++; 

    Serial.print(" goDirA ");
    Serial.print(goDirA);
    Serial.print(" goDirB ");
    Serial.print(goDirB);
    Serial.print(" posLim ");
    Serial.print(posLim);
    Serial.print(" posCounter ");    
    Serial.print(posCounter);
    Serial.print(" posMax ");
    Serial.print(posMax);
    Serial.print(" slipCount ");
    Serial.println(slipCount);
    
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

