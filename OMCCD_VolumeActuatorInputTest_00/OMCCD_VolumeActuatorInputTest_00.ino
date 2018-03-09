double input=0.0;
void setup() {
  // put your setup code here, to run once:
pinMode(5,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
input= pulseIn(5, HIGH); //READ SIGNAL FROM SAMPLE
Serial.println(input);
}
