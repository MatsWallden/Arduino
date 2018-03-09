// from http://www.circuitbasics.com/arduino-ohm-meter/

int analogPin= 0;
const int relay1 = 2; // the number of the relay1 pin// variables will change:


void setup()
{
pinMode(relay1, OUTPUT);  
Serial.begin(9600);
}

void loop()
{

digitalWrite(relay1, HIGH);
delay(500);  
  digitalWrite(relay1,LOW);
  delay(500);    //Here we influence how often we read the value

}

