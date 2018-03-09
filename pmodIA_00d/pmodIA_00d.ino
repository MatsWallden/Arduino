
//libraries
#include <Wire.h>

//functions
void setRegisterPointer(int registerAddress);
void sendCommand2ControlRegister(int command);
uint8_t readRegisterValue(); 
void printlnByteBinaryLeadingZeros(uint8_t val);
void printControlRegisterStatusCODEstring(uint8_t readControl);
/*Register Map*/

#define IA_ADDRESS              0x0D  // IA 
#define BLOCK_WRITE_CMD         0xA0  // block write command
#define BLOCK_READ_CMD          0xA1  // block read command
#define ADR_PTR_CMD             0xB0  // address pointer command

/*Register Map*/
#define CONTROL_REG             0x80
#define CONTROL_REG_II          0x81 //DO WE NEED IT?
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
  /*WORKING UNDER THE ASSUMPTIONS: 
  i)  THAT EVERY ADDRESS HOLDS A BITE
  ii) THAT THE MSBs ARE IN THE LOWER REGISTERS IN A RANGE
  iii) THE CONTROL REGISTER IS NOT SET PROPERLY
    HOW DO WE INTERPRET THE CONTROL REGISTER?

   iv) Printing binary with leading zeros now works
    
    */

  
  Serial.println("START OF THE RUN");
  Serial.println("SETTING POINTER TO CONTROL REGISTER"); 
  setRegisterPointer(CONTROL_REG);
  Serial.println("READING THE CONTROL REGISTER");
  Serial.print("DECIMAL ");
  uint8_t bla= readRegisterValue();
  Serial.println(bla);
  Serial.print("BINARY ");
  Serial.println(bla,BIN);
  printlnByteBinaryLeadingZeros(bla);
  printlnByteBinaryLeadingZeros(bla>>4);
  printControlRegisterStatusCODEstring(bla>>4);
  
  // send Command to Measure Temperature
  sendCommand2ControlRegister(START_FREQ_SWEEP);

  bla= readRegisterValue();
  printlnByteBinaryLeadingZeros(bla>>4);
  printControlRegisterStatusCODEstring(bla>>4);

   // send Command to Measure Temperature
  sendCommand2ControlRegister(MEASURE_TEMP);

  bla= readRegisterValue();
  printlnByteBinaryLeadingZeros(bla>>4);
  printControlRegisterStatusCODEstring(bla>>4);
  //read the temperature
  setRegisterPointer(CONTROL_REG);
  Serial.println("READING THE CONTROL REGISTER");
  Serial.println(readRegisterValue(),BIN);
  
  /* setRegisterPointer(TEMP_DATA_REG);
    Serial.println("IS THIS THE TEMPERATURE?");
    Serial.println(readRegisterValue(),BIN);
 */ 
  /*
   // Read from temperature Register
   setRegisterPointer(TEMP_DATA_REG);
   delay(20);
   */
   /*Wire.requestFrom(IA_ADDRESS, 1); 
   uint8_t disp_temp;
   disp_temp = Wire.read(); 
   Wire.endTransmission();
   Serial.println("the temperature register is:");
   Serial.println(disp_temp,BIN);*/
 /*      
  //set Register Pointer to Status Register
  setRegisterPointer(STATUS_REG);
    
  // Read from Status Register
  delay(20);

  Serial.println("Current register status is:");
  // int disp_status=readRegisterValue();
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
  Serial.println(disp_control,BIN);*/
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
uint8_t readRegisterValue(){
    Wire.requestFrom(IA_ADDRESS, 1);
    uint8_t disp_status = Wire.read(); 
    Wire.endTransmission();
    return disp_status;    
    }
void printlnByteBinaryLeadingZeros(uint8_t val){
  //Serial.println("INSIDE THE FUNCTION");
  if(!val){Serial.println("00000000");}
  else{
  int i=7;
  while(val<pow(2,i)){Serial.print(B0);i--;};
  Serial.print(val,BIN);
  Serial.print('\n');
  }  
  }
void printControlRegisterStatusCODEstring(uint8_t readControl){

if(readControl==0b0)          {Serial.println("0000 No operation");}
else if(readControl==0b1)     {Serial.println("0001 Initialize with start frequency");} 
else if(readControl==0b10)    {Serial.println("0010 Start frequency sweep");} 
else if(readControl==0b11)    {Serial.println("0011 Increment frequency");} 
else if(readControl==0b100)   {Serial.println("0100 Repeat frequency");} 
else if(readControl==0b1000)  {Serial.println("1000 No operation");} 
else if(readControl==0b1001)  {Serial.println("1001 Measure temperature");} 
else if(readControl==0b1010)  {Serial.println("1010 Power-down mode");} 
else if(readControl==0b1011)  {Serial.println("1011 Standby mode");} 
else if(readControl==0b1100)  {Serial.println("1100 No operation A");} 
else if(readControl==0b1101)  {Serial.println("1101 No operation B");}
else                          {Serial.println("UNKNOWN CODE");};
}

