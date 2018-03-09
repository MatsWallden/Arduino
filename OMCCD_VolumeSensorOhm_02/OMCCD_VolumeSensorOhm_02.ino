// from http://www.circuitbasics.com/arduino-ohm-meter/

int analogPin= 0;
const int relay1 = 11; // the number of the relay1 pin// variables will change:

int raw= 0;     //Raw is the measured value from A0. Is that a voltage?
int Vin= 2.5;   //Vin is the voltage provided by the Arduino
float Vout= 0;    //Vout is the voltage out
float R1= 10000000;   //Known resistance
float R2= 0;    //unknown resistance
float buffer00= 0;  //Buffer is the levels of voltage measured (multiplied by 1024)
float buffer01=0; // 

void setup()
{
pinMode(relay1, OUTPUT);  
Serial.begin(9600);
analogReference(EXTERNAL);
}

void loop()
{

digitalWrite(relay1, HIGH);
delay(500);  
raw= analogRead(analogPin);
if(raw) 
{
  buffer00= raw * Vin;  //here buffer has a unit
  Vout= (buffer00)/1024.0;
  Vout=Vout/1.5;
  buffer01= (Vin/Vout) -1;  
  R2= R1 * buffer01;  //
  //Serial.print("Vout: ");
  //Serial.println(Vout);
  //Serial.print("R2: ");
  Serial.println(R2);
  
}
  digitalWrite(relay1,LOW);
  delay(500);    //Here we influence how often we read the value

}

