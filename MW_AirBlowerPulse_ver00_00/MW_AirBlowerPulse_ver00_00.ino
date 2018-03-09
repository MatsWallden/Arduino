const int relay1 = 2; // the number of the relay1 pin// variables will change:
void setup() {
// initialize the relay pin as an output:
pinMode(relay1, OUTPUT);
}

void loop(){
digitalWrite(relay1, HIGH);
delay(3000); // this is the time the blower is on in milliseconds
digitalWrite(relay1,LOW);
delay(2000); // this is the time the blower is off in milliseconds
}
