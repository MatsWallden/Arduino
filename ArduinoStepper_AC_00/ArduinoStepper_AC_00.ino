/*************************************************************
Motor Shield Stepper Demo
by Randy Sarafan

For more information see:
https://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/

int delaylegnth = 500;

void setup() {
  Serial.begin(9600);
  
  //establish motor direction toggle pins
  pinMode(12, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
    
  //establish motor brake pins
  //pinMode(9, OUTPUT); //brake (disable) CH A
  int bla=0;
  
  
}

void loop(){

  digitalWrite(12, HIGH);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  Serial.print(analogRead(A0)); Serial.print("  ");
  delay(delaylegnth);
  
  digitalWrite(12, LOW);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  
  Serial.println(analogRead(A0));
  delay(delaylegnth);
  }

