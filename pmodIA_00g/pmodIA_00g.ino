
//libraries
#include <Wire.h>

//functions
void setRegisterPointer(int registerAddress);
void sendCommand2ControlRegister(uint8_t command);
uint8_t readRegisterValue(); 
void writeRegisterValue(uint8_t add,uint8_t val);
void printlnByteBinaryLeadingZeros(uint8_t val);
void printControlRegisterStatusCODEstring(uint8_t readControl);
uint16_t measureTemperature();
uint8_t BIT_1;
uint8_t reg0{0};
  uint8_t reg1{0};
  uint16_t R{0};  
  uint16_t I{0};
  uint8_t STAT;
  int ii;
  uint32_t M;
  double IMP;
  double GF=1528*pow(10,-12);
  int ij=0;  


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
  // initialization
  Wire.begin();
  Serial.begin(9600);  
  /*WORKING UNDER THE ASSUMPTIONS: 
  i)  THAT EVERY ADDRESS HOLDS A BITE
  ii) THAT THE MSBs ARE IN THE LOWER REGISTERS IN A RANGE
 
  v)
    
    */
  //START THE RUN  
  Serial.println("START OF THE RUN");
  
  /*Serial.println("SETTING POINTER TO CONTROL REGISTER"); 
  setRegisterPointer(CONTROL_REG);
  Serial.println("READING THE CONTROL REGISTER");
  uint8_t bla= readRegisterValue();
  printControlRegisterStatusCODEstring(bla>>4);
  */
  //Serial.println("START FREQ SWEEP");
  //uint16_t theTemp=measureTemperature();

  //Serial.println("SET THE SWEEP PARAMETERS!!");
  //START FREQUENCY 30kHz
  writeRegisterValue((uint8_t)0x82,(uint8_t)0x0F);
  writeRegisterValue((uint8_t)0x83,(uint8_t)0x5C);
  writeRegisterValue((uint8_t)0x84,(uint8_t)0x28);
  
  //FREQ INC 
  //INCREMENT 10 Hz
  writeRegisterValue((uint8_t)0x85,(uint8_t)0x00);
  writeRegisterValue((uint8_t)0x86,(uint8_t)0x01);
  writeRegisterValue((uint8_t)0x87,(uint8_t)0x4F);  
  
  //NUMBER OF INC. 511
  writeRegisterValue((uint8_t)0x88,(uint8_t)0x1);
  writeRegisterValue((uint8_t)0x89,(uint8_t)0xFF);
  
  //Settling time x2, 100 cycles
  writeRegisterValue((uint8_t)0x8A,(uint8_t)0x01);  
  writeRegisterValue((uint8_t)0x8B,(uint8_t)0x64);
  
 // Serial.println("STAND BY MODE");
  sendCommand2ControlRegister(STAND_BY);//Execute
  //Serial.println("INIT_START_FREQ");
  sendCommand2ControlRegister(INIT_START_FREQ);//Execute
  //Serial.println("START_FREQ_SWEEP");
  sendCommand2ControlRegister(START_FREQ_SWEEP);//Execute
  setRegisterPointer(STATUS_REG);//
  while( ij<512 || BIT_1 !=0b100){
  ii=0;
  do{
        Wire.requestFrom(IA_ADDRESS,1);
        BIT_1 = Wire.read();        
        BIT_1 = BIT_1 & 0b00000010;                
        ii++;
      }while(BIT_1 != 0x02 && ii<255); 
      //Serial.println(BIT_1,BIN);
      //Serial.println(ii);
      //Serial.println(ij);
      //STAT=readRegisterValue();  
      Serial.print("STATUS REG ");  printlnByteBinaryLeadingZeros(STAT);
      ij++; 
  }
}

void loop()

{


  
 /* setRegisterPointer(STATUS_REG);//
  ii=0;
  do{
        Wire.requestFrom(IA_ADDRESS,1);
        BIT_1 = Wire.read();
        if(BIT_1 == 0b100){Serial.println("FINISHED SWEEP!");};
        BIT_1 = BIT_1 & 0b00000010;
                
        ii++;
      }while(BIT_1 != 0x02 && ii<255);  
  
  //setRegisterPointer(STATUS_REG);//
  //STAT=readRegisterValue();  
  //Serial.print("STATUS REG ");  printlnByteBinaryLeadingZeros(STAT);

  setRegisterPointer(0x94); reg0=readRegisterValue();
  setRegisterPointer(0x95); reg1=readRegisterValue();
  R=reg0<<8 | reg1;
    
  setRegisterPointer(0x96);  reg0=readRegisterValue();
  setRegisterPointer(0x97);  reg1=readRegisterValue();
  I=reg0<<8 | reg1;
  M=pow(pow(R,2)+pow(I,2),0.5);
  //IMP=pow(GF*M,-1);
  
  Serial.println(M);
  //Serial.print("ii ");Serial.println(ii);
  //Serial.print("BIT_1 "); Serial.println(BIT_1,BIN);
  //Serial.print("M ");Serial.println(M);
  //Serial.print("IMP "); Serial.println(IMP);

ij++;
Serial.println(ij);*/
}
/***************
 * FUNCTIONS


*/
void setRegisterPointer(int registerAddress){
  //this function sets the pointer to an adress on the AD5933
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(ADR_PTR_CMD); 
  Wire.write(registerAddress);
  Wire.endTransmission();
  } 

void sendCommand2ControlRegister(uint8_t command){
//void sendCommand2ControlRegister(int command){
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

void writeRegisterValue(uint8_t add,uint8_t val){
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(add); 
  Wire.write(val);
  Wire.endTransmission();  
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

//uint16_t measureRealData(



uint16_t measureTemperature(){
  // 
  Serial.println("INSIDE METHOD MEASURETEMPERATURE");
  uint8_t reg0{0};
  uint8_t reg1{0};
  sendCommand2ControlRegister(MEASURE_TEMP);//Execute measurement

  setRegisterPointer(STATUS_REG);// CHECK THAT IT HAPPENED
  uint8_t STAT=readRegisterValue();

  if( STAT & 0b1){Serial.println("VALID TEMPERATURE MEASUREMENT");}
  else{Serial.println("TEMPERATURE MEASUREMENT DIDN*T WORK!");};
 
  setRegisterPointer(TEMP_DATA_REG_0);
  reg0=readRegisterValue();
  setRegisterPointer(TEMP_DATA_REG_1);
  reg1=readRegisterValue();
  // binary concatenation
  uint16_t reg_16=reg0<<8 | reg1;
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

