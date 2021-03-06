
//libraries
#include <Wire.h>

//FUNCTION DECLARATIONS
void setRegisterPointer(int registerAddress);
void sendCommand2ControlRegister(uint8_t command);
uint8_t readRegisterValue(); 
void writeRegisterValue(uint8_t add,uint8_t val);
void printlnByteBinaryLeadingZeros(uint8_t val);
void printControlRegisterStatusCODEstring(uint8_t readControl);
int measureTemperature();

//VARIABLE DECLARATIONS
//uint8_t reg0{0};
//uint8_t reg1{0};
int T{0}; //TEMPERATURE
int R{0}; //REAL PART OF THE FREQUENCY INFORMATION
int I{0}; //IMAGINARY PARTY OF THE FREQUENCY INFORMATION

//uint16_t reg_16{0};  //SHOULD THIS BE INT?
uint8_t STATUS; 
uint8_t BIN_TEMPERATURE_MEASUREMENT_DONE;
uint8_t BIN_FREQUENCY_MEASUREMENT_DONE;
uint8_t BIN_FREQUENCY_SWEEP_DONE;
int ii;

/*Register Map*/

#define IA_ADDRESS              0x0D  // IA 
#define BLOCK_WRITE_CMD         0xA0  // block write command
#define BLOCK_READ_CMD          0xA1  // block read command
#define ADR_PTR_CMD             0xB0  // address pointer command

/*Register Map*/
#define CONTROL_REG             0x80 //0x81
#define START_FREQUENCY_REG     0x82
#define FREQUENCY_INCREMENT_REG 0x85
#define NUM_INCREMENTS_REG      0x88
#define NUM_SETTLING_TIME_REG   0x8A
#define STATUS_REG              0x8F
#define TEMP_DATA_REG_0         0x92
#define TEMP_DATA_REG_1         0x93
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
 
  /*WORKING UNDER THE ASSUMPTIONS: 
  i)  THAT EVERY ADDRESS HOLDS A BITE
 
  v)
    
    */

  initialize();

  

  
  sendCommand2ControlRegister(MEASURE_TEMP);
    
  setRegisterPointer(STATUS_REG);//
  ii=0;
  do{
        Wire.requestFrom(IA_ADDRESS,1);
        BIT_1= Wire.read();

        BIT_1 = BIT_1 & 0b00000001;        
        ii++;
      }while(BIT_1 != 0x01 && ii<255);  
    
  setRegisterPointer(TEMP_DATA_REG_0); reg0=readRegisterValue();
  printlnByteBinaryLeadingZeros(reg0);
  setRegisterPointer(TEMP_DATA_REG_1); reg1=readRegisterValue();
  printlnByteBinaryLeadingZeros(reg1);
  reg_16=reg0<<8 | reg1; //CONCATENATION OF TWO BYTES
  
  Serial.print("ii "); Serial.println(ii);
  Serial.print("BIT_1 "); Serial.println(BIT_1);
  Serial.print("REG_16 "); Serial.println(reg_16,BIN);
}

void loop()

{

}
/***************
 * FUNCTIONS


*/

void initialize(){
  //INITIALIZE COMMUNICATION WITH SLAVE AND MASTER
  Wire.begin();
  Serial.begin(9600);   
  }

void setRegisterPointer(int registerAddress){
  //SET THE POINTER TO AN ADDRESS
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(ADR_PTR_CMD); 
  Wire.write(registerAddress);
  Wire.endTransmission();
  } 

void sendCommand2ControlRegister(uint8_t command){
  //ISSUE A COMMAND TO THE CONTROL REGSITER
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(CONTROL_REG);
  Wire.write(command); 
  Wire.endTransmission();    
  }
  
uint8_t readRegisterValue(){
    // READ A REGISTER VALUE
    Wire.requestFrom(IA_ADDRESS, 1);
    uint8_t disp_status = Wire.read(); 
    Wire.endTransmission();
    return disp_status;    
    }

void writeRegisterValue(uint8_t add,uint8_t val){
  // WRITE TO A REGISTER VALUE
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(add); 
  Wire.write(val);
  Wire.endTransmission();  
  }
    
void printlnByteBinaryLeadingZeros(uint8_t val){
  //THIS FUNCTION PRINTS A BINARY NUMBER (BYTE SIZE) WITH LEADING ZEROS
  if(!val){Serial.println("00000000");}
  else{
  int i=7;
  while(val<pow(2,i)){Serial.print(B0);i--;};
  Serial.print(val,BIN);
  Serial.print('\n');
  }  
  }
void printControlRegisterStatusCODEstring(uint8_t readControl){
//PRINTS THE STATUSREGISTER STATE
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


int measureTemperature(){
  // MEASURES THE TEMPERATURE
  // DOES IT WORK?? MW2018jan05
  //Serial.println("INSIDE METHOD MEASURETEMPERATURE");
  int reg0{0};
  int reg1{0};
  sendCommand2ControlRegister(MEASURE_TEMP);//Execute measurement

  //setRegisterPointer(STATUS_REG);// CHECK THAT IT HAPPENED
  //uint8_t STAT=readRegisterValue();

  //if( STAT & 0b1){Serial.println("VALID TEMPERATURE MEASUREMENT");}
  //else{Serial.println("TEMPERATURE MEASUREMENT DIDN*T WORK!");};
 
  setRegisterPointer(TEMP_DATA_REG_0);
  reg0=readRegisterValue();
  setRegisterPointer(TEMP_DATA_REG_1);
  reg1=readRegisterValue();
  // binary concatenation
  int reg_16=reg0<<8 | reg1;
  Serial.print("REG0 ");
  printlnByteBinaryLeadingZeros(reg0);
  Serial.print("REG1 ");
  printlnByteBinaryLeadingZeros(reg1);
  Serial.print("REGTOT ");
  Serial.println(reg_16,BIN);
  return reg_16;
  /*
  Positive Temperature = ADC Code (D)/32 
  Negative Temperature = (ADC Code (D) – 16384)/32 
  where ADC Code uses all 14 bits of the data byte, including the 
  sign bit. 
  Negative Temperature = (ADC Code (D) – 8192)/32 
  where ADC Code (D) is D13, the sign bit, and is removed from the 
  ADC code.)
  */
  }

