int raw0;
int raw1;
int raw;
double samplePeriod=10.0;
double startTime=0.0;
double endTime=0.0;
int thresh =10;

void setup()
{
Serial.begin(9600);
raw=0;
raw0=0;
raw1=0;
}

void loop(){
  //startTime=millis();
  raw1=analogRead(A0);  
  //if(raw<thresh){raw=raw0;}else{raw=raw1; raw0=raw1;}; 
  if(raw1>0 && raw1<1023){
  Serial.println(raw1);
  }
  //endTime=millis();
  //delay(samplePeriod-endTime+startTime);   
  delay(0); 
}

