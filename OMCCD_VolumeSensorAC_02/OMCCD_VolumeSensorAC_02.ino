// from http://www.circuitbasics.com/arduino-ohm-meter/

int analogPin= A2;
const int relay1 = 12; // the number of the relay1 pin// variables will change:
const int relay2 = 3; 

int raw= 0;     //Raw is the measured value from A0. Is that a voltage?
int Vin= 5;   //Vin is the voltage provided by the Arduino
float Vout= 0;    //Vout is the voltage out
float R1= 1000000;   //Known resistance
float R2= 0;    //unknown resistance
float buffer00= 0;  //Buffer is the levels of voltage measured (multiplied by 1024)
float buffer01=0; // 

void setup()
{
//pinMode(relay1, OUTPUT);
pinMode(relay1, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
Serial.begin(9600);
//analogReference(EXTERNAL);
}

void loop(){

digitalWrite(relay1, HIGH);
analogWrite(relay2,255);
raw=analogRead(analogPin);
Serial.println(raw);
  delay(5); 
  digitalWrite(relay1, LOW);
  analogWrite(relay2,255);
  //raw=analogRead(analogPin);
  //Serial.print("");
  delay(5);   
}

