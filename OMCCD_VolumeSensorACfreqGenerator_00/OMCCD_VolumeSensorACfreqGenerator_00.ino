const int relay1 = 12; // the number of the relay1 pin// variables will change:
const int relay2 = 3; 
const double period = 10.0;

void setup()
{
pinMode(relay1, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards
}

void loop(){

digitalWrite(relay1, HIGH);
analogWrite(relay2,255);
delay(0.5*period); 
digitalWrite(relay1, LOW);
analogWrite(relay2,255);
delay(0.5*period);   
}

