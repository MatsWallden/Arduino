/*************************************************************
Motor Shield Stepper Demo
by Randy Sarafan

For more information see:
https://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/

int delaylegnth = 30;

void setup() {
  
  //establish motor direction toggle pins
  pinMode(2, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(8, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???

  pinMode(3,OUTPUT);
  pinMode(9,OUTPUT);
  //establish motor brake pins
  //pinMode(9, OUTPUT); //brake (disable) CH A
  //pinMode(8, OUTPUT); //brake (disable) CH B


  
  
}

void loop(){
 
  //digitalWrite(9, LOW);  //ENABLE CH A
  //digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(2, HIGH);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  
  delay(delaylegnth);
  
  //digitalWrite(9, HIGH);  //DISABLE CH A
 // digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(8, LOW);   //Sets direction of CH B
  analogWrite(9, 255);   //Moves CH B
  
  delay(delaylegnth);
  
  //digitalWrite(9, LOW);  //ENABLE CH A
  //digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(2, LOW);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  
  delay(delaylegnth);
    
  //digitalWrite(9, HIGH);  //DISABLE CH A
  //digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(8, HIGH);   //Sets direction of CH B
  analogWrite(9, 255);   //Moves CH B
  
  delay(delaylegnth);

}

