#include <Wire.h>

/*Register Map*/

#define IA_ADDRESS              0x0D  //IA 
#define BLOCK_WRITE_CMD         0xA0  //block write command
#define BLOCK_READ_CMD          0xA1  //block read command
#define ADR_PTR_CMD             0xB0  //address pointer command

/*Register Map*/
#define CONTROL_REG                               0x80
#define START_FREQUENCY_REG           0x82
#define FREQUENCY_INCREMENT_REG      0x85
#define NUM_INCREMENTS_REG                 0x88
#define NUM_SETTLING_TIME_REG             0x8A
#define STATUS_REG                   0x8F
#define TEMP_DATA_REG            0x92
#define REAL_DATA_REG             0x94
#define IMAG_DATA_REG             0x96

/*CONTROL REGISTER COMMANDS*/

#define INIT_START_FREQ              0x10
#define START_FREQ_SWEEP        0x20
#define INC_FREQ                           0x30
#define REPEAT_FREQ                   0x40
#define MEASURE_TEMP                0x90
#define PWR_DOWN                        0xA0
#define STAND_BY                          0xB0

void setup()
{
  // initialization
  Wire.begin();
  Serial.begin(9600);  

  // send Command to Control Register
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(0x80);
  Wire.write(0x90); 
  Wire.endTransmission();

  // set Register Pointer to Status Register
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(0xB0);   // WRITE TO THE POINTER 
  Wire.write(0x8F);   // THIS IS WHERE THE MEASUREMENT HAPPENS
  Wire.endTransmission();

  // Read from Status Register
  delay(500);
  //Wire.requestFrom(0x0D, 1); 
  Wire.requestFrom(IA_ADDRESS, 1); 

  uint8_t disp_status;
  disp_status = Wire.read(); 
  Wire.endTransmission();
  Serial.println("Current register status is:");
  Serial.println(disp_status,BIN);

  // set Register Pointer to Control Register
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0);  
  Wire.write(0x80);
  Wire.endTransmission();
  delay(20);

  // Read from Status Register
  delay(500);
  Wire.requestFrom(0x0D, 1); 
  uint8_t disp_control;
  disp_control = Wire.read(); 
  Wire.endTransmission();
  Serial.println("Current control register is:");
  Serial.println(disp_control,BIN);
}

void loop()

{

}

 

 

 


