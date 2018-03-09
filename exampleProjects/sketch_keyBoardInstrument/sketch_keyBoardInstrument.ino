/*Never really worked. First button never yielded a signal. 
I Expected it to give a full 1023 signal but it was always 0 when pressing
and of course when no other buttons were pressed.*/
int buttons[]={2,0,0,0,0,0}; //array of six integers
//int buttons[0] = 2; //having trouble with this.. 
int notes[]={262,294,330,349}; //frequency in Hz, C,D,E 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int keyVal= analogRead(A0);
Serial.println(keyVal);
/*if (keyVal==1023){
  tone(8,notes[0]);
  }
  
  else if(keyVal>= 990 && keyVal<=1010){
    tone(8,notes[1]);
    }
    else if(keyVal>=505 && keyVal < 515){
      tone(8,notes[2]);
      }
      else if(keyVal>=5 && keyVal<=10){
        tone(8,notes[3]);
        }
        else{
          noTone(8);
          }
          */
}
