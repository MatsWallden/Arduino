
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
double posLimSum;
bool cond;
bool goDirA; //DOWNWARDS
bool goDirB; //
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
  //goDirB=false; //start with downwards
  cond=true;
 }

void loop(){
    posLim=digitalRead(pinPosLim);
    
    // WHY CAN IT NOT GET AWAY??????!!??!
    if(posLim){
        //ALTER POSITION INFORMATION
        
        if(goDirA){posCounter=0;}else{posMax=posCounter;}
        
        //DETERMINE NEW DIRECTION
        goDirA=!goDirA; //SWITCHES DIRECTION

        for(int ii=0; ii>100; ii++){goDirection();}
        
        posLimSum=1.0;
        count=1;
        while(posLimSum/count>0.5){               
          goDirection();
          posLimSum+=(double)digitalRead(pinPosLim);
          count++;
          Serial.println(posLimSum/count);
          };

        //MOVE AWAY FROM POSLIM
      }else{
        /*Serial.print("goDwn ");
        Serial.print(goDirA);
        Serial.print(" posLim ");
        Serial.print(posLim);
        Serial.print(" posLimAve ");
        Serial.println(posLimSum/count);*/
        delay(10);
        goDirection();
        }   
}

void goDirection(){
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

