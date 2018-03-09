
//libraries
#include <Wire.h>

//functions
void setRegisterPointer(int registerAddress);
void sendCommand2ControlRegister(int command);
int readRegisterValue(); 

/*Register Map*/

#define IA_ADDRESS              0x0D  // IA 
#define BLOCK_WRITE_CMD         0xA0  // block write command
#define BLOCK_READ_CMD          0xA1  // block read command
#define ADR_PTR_CMD             0xB0  // address pointer command

/*Register Map*/
#define CONTROL_REG             0x80
#define START_FREQUENCY_REG     0x82
#define FREQUENCY_INCREMENT_REG 0x85
#define NUM_INCREMENTS_REG      0x88
#define NUM_SETTLING_TIME_REG   0x8A
#define STATUS_REG              0x8F
#define TEMP_DATA_REG           0x92
#define REAL_DATA_REG           0x94
#define IMAG_DATA_REG           0x96

/*CONTROL REGISTER COMMANDS*/

#define INIT_START_FREQ         0x10
#define START_FREQ_SWEEP        0x20
#define INC_FREQ                0x30
#define REPEAT_FREQ             0x40
#define MEASURE_TEMP            0x90
#define PWR_DOWN                0xA0
#define STAND_BY                0xB0

void setup()
{
  // initialization
  Wire.begin();
  Serial.begin(9600);  
  
  uint16_t disp_control01;
  setRegisterPointer(CONTROL_REG);
  disp_control01=readRegisterValue();
  Serial.println(disp_control01,BIN);
  // send Command to Control Register
  sendCommand2ControlRegister(MEASURE_TEMP); 

  //read the temperature
  /*
   * setRegisterPointer(TEMP_DATA_REG);
    Serial.println("IS THIS THE TEMPERATURE?");
    Serial.println(readRegisterValue(),BIN);
 */ 

   // Read from temperature Register
  setRegisterPointer(TEMP_DATA_REG);
   delay(20);
   /*Wire.requestFrom(IA_ADDRESS, 1); 
   uint8_t disp_temp;
   disp_temp = Wire.read(); 
   Wire.endTransmission();
   Serial.println("the temperature register is:");
   Serial.println(disp_temp,BIN);*/
       
  //set Register Pointer to Status Register
  setRegisterPointer(STATUS_REG);
    
  // Read from Status Register
  delay(20);

  Serial.println("Current register status is:");
  // int disp_status=readRegisterValue();
  //Serial.println(disp_status,BIN);
  Serial.println(readRegisterValue(),BIN);

  // set Register Pointer to Control Register
  setRegisterPointer(CONTROL_REG);
  delay(20);

  // Read from Status Register
  delay(500);
  Wire.requestFrom(IA_ADDRESS, 1); 
  //uint8_t disp_control;
  int disp_control;
  disp_control = Wire.read(); 
  Wire.endTransmission();
  Serial.println("Current control register is:");
  Serial.println(disp_control,BIN);
}

void loop()

{

}

void setRegisterPointer(int registerAddress){
  //this function sets the pointer to an adress on the AD5933
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(ADR_PTR_CMD); 
  Wire.write(registerAddress);
  Wire.endTransmission();
  } 

void sendCommand2ControlRegister(int command){
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(CONTROL_REG);
  Wire.write(command); 
  Wire.endTransmission();    
  }
int readRegisterValue(){
    Wire.requestFrom(IA_ADDRESS, 1);
    int disp_status = Wire.read(); 
    Wire.endTransmission();
    return disp_status;    
    }
 


