
//libraries
#include <Wire.h>

//FUNCTION DECLARATION
void setStartFrequency2_30kHz(); //FIX ME!
void setRegisterPointer(int registerAddress);
void sendCommand2ControlRegister(uint8_t command);
int readRegisterValue(); //THIS MAY HAVE TO RETURN AN INT
void writeRegisterValue(uint8_t add,uint8_t val);
void printlnByteBinaryLeadingZeros(uint8_t val);
void printControlRegisterStatusCODEstring(uint8_t readControl);
int measureTemperature();
int getImaginaryPart();
int getRealPart();
bool waitForValidFreqData();
void initialize();
void setVpp_2V();
void setPGA_x1();


// VARIABLE DECLARATIONS
int R{0};  
int I{0};
uint8_t STAT;
double M;
double IMP;

/*Register Map*/
#define IA_ADDRESS              0x0D  // pmod IA 
#define BLOCK_WRITE_CMD         0xA0  // block write command
#define BLOCK_READ_CMD          0xA1  // block read command
#define ADR_PTR_CMD             0xB0  // address pointer command

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

/* STATUS CODES*/
#define VALID_TEMP_DATA         0b00000001
#define VALID_FREQ_DATA         0b00000010
#define FREQ_SWEEP_COMPLETE     0b00000100

void setup()
{
  // initialization 
  initialize();
  setVpp_2V();
  setPGA_x1();
  
  //Serial.println("START OF THE RUN");
  //Serial.println("SET THE SWEEP PARAMETERS!!");
  //Serial.println("SET THE START FREQUENCY TO 30KHZ");
  setStartFrequency2_30kHz();
  //Serial.println("SET THE FREQUENCY INCREMENT TO 10 HZ");
  setFrequencyIncrement2_10Hz();

  //NUMBER OF INC. 511
  writeRegisterValue((uint8_t)0x88,(uint8_t)0x1);
  writeRegisterValue((uint8_t)0x89,(uint8_t)0xFF);
  
  //Settling time x2, 100 cycles
  writeRegisterValue((uint8_t)0x8A,(uint8_t)0x01);  
  writeRegisterValue((uint8_t)0x8B,(uint8_t)0x64);
  
  //Serial.println("STAND BY MODE");
  sendCommand2ControlRegister(STAND_BY);//Execute
  //Serial.println("INIT_START_FREQ");
  sendCommand2ControlRegister(INIT_START_FREQ);//Execute
  //Serial.println("START_FREQ_SWEEP");
  /*sendCommand2ControlRegister(START_FREQ_SWEEP);//Execute
  I=getImaginaryPart();
  R=getRealPart();
  M=pow(pow(R,2)+pow(I,2),0.5);
  Serial.println(M);   
  
  //set the D3 of the status register, abort sweep
  writeRegisterValue(STATUS_REG,FREQ_SWEEP_COMPLETE);
  */
  sendCommand2ControlRegister(START_FREQ_SWEEP);//Execute
  
  I=getImaginaryPart();
  R=getRealPart();
  M=pow(pow(R,2)+pow(I,2),0.5);
  Serial.println(M);   
  
  //set the D3 of the status register, abort sweep
  writeRegisterValue(STATUS_REG,FREQ_SWEEP_COMPLETE);
}

void loop()
{
  
  
  
}
/***************
 * FUNCTIONS


*/

int getImaginaryPart(){
  int reg_08_0{0};
  int reg_08_1{0};
  int reg_16{0};
  bool timeOut{false};
  timeOut=waitForValidFreqData();
  if(!timeOut){
    setRegisterPointer(0x96); reg_08_0=readRegisterValue();
    //Serial.print("reg_08_0 ");
    //Serial.println(reg_08_0);
    setRegisterPointer(0x97); reg_08_1=readRegisterValue();
    //Serial.print("reg_08_1 ");
    //Serial.println(reg_08_1);
    reg_16= reg_08_0 << 8 | reg_08_1;
    //Serial.print("reg_16_0 ");
    //Serial.println(reg_16);
  }
  else{Serial.println("TIMEOUT");}
  //FIX ME!! HOW DO WE HANDLE IF THERE IS A TIMEOUT??
  return reg_16;
  }

int getRealPart(){
  int reg_08_0{0};
  int reg_08_1{0};
  int reg_16{0};
  bool timeOut{false};
  timeOut=waitForValidFreqData();
  if(!timeOut){
    //FIX ME!! HARDCODED REGISTERVALUES!!
    setRegisterPointer(0x94); reg_08_0=readRegisterValue();
    //Serial.print("reg_08_0 ");
    //Serial.println(reg_08_0);
    setRegisterPointer(0x95); reg_08_1=readRegisterValue();
    //Serial.print("reg_08_1 ");
    //Serial.println(reg_08_1);
    reg_16= reg_08_0 << 8 | reg_08_1;
    //Serial.print("reg_16_0 ");
    //Serial.println(reg_16);
  }
  else{Serial.println("TIMEOUT");}
  //FIX ME!! HOW DO WE HANDLE IF THERE IS A TIMEOUT??
  return reg_16;
  }
  
void setPGA_x1(){  
  //SETS THE PGA GAIN TO x1
  setRegisterPointer(CONTROL_REG);
  uint8_t cReg0x80=readRegisterValue();
  cReg0x80 ^= (-1 ^ cReg0x80) & (1UL << 0); // SETS BIT D08 TO 1
  writeRegisterValue(CONTROL_REG,cReg0x80);
  }
  
void setVpp_2V(){  
  //SETS THE VPP EXCITATION TO 2 V
  setRegisterPointer(CONTROL_REG);
  uint8_t cReg0x80=readRegisterValue();
  cReg0x80 ^= (-0 ^ cReg0x80) & (1UL << 2); // SETS BIT D10 TO 0
  cReg0x80 ^= (-0 ^ cReg0x80) & (1UL << 1); // SETS BIT D09 TO 0
  writeRegisterValue(CONTROL_REG,cReg0x80);
  }

void initialize(){
  Wire.begin();
  Serial.begin(9600);
  } 
  
bool waitForValidFreqData(){
  int ii=0;
  int ii_timeOutLim=255; //FIX ME!! HARDCODED NUMBER!
  uint8_t BIT_1;
  //bool timeOut{false};
  setRegisterPointer(STATUS_REG);//POLL THE STATUS REGISTER
  do{ //ASK IF THERE IS VALID FREQUENCY DATA      
    Wire.requestFrom(IA_ADDRESS,1);
    BIT_1 = Wire.read();        
    BIT_1 = BIT_1 & VALID_FREQ_DATA;                
    ii++;
    }while(BIT_1 != 0x02 && ii<ii_timeOutLim); //END DO-WHILE LOOP
        //FIX ME!! WHAT DO WE DO WHEN THIS FAILS??
        return ii >= ii_timeOutLim; //TIME OUT 
    }

void setFrequencyIncrement2_10Hz(){
  //FIX ME!! THIS IS A TEMPORARY FUNCTION  
  writeRegisterValue((uint8_t)0x85,(uint8_t)0x00);
  writeRegisterValue((uint8_t)0x86,(uint8_t)0x01);
  writeRegisterValue((uint8_t)0x87,(uint8_t)0x4F);  
  }
  
void setStartFrequency2_30kHz(){    
    //THIS IS A TEMPORARY SOLUTION
    //FIX ME!! BLOCK WRITE
  writeRegisterValue((uint8_t)0x82,(uint8_t)0x0F);
  writeRegisterValue((uint8_t)0x83,(uint8_t)0x5C);
  writeRegisterValue((uint8_t)0x84,(uint8_t)0x28);
  }
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

int readRegisterValue(){
    //READ REGISTER VALUE
    Wire.requestFrom(IA_ADDRESS, 1);
    int disp_status = Wire.read(); //INT SO THAT SIGNED CAN BE READ
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

int measureTemperature(){
  // 
  Serial.println("INSIDE METHOD MEASURETEMPERATURE");
  int reg0{0};
  int reg1{0};
  sendCommand2ControlRegister(MEASURE_TEMP);//Execute measurement
  setRegisterPointer(STATUS_REG);// CHECK THAT IT HAPPENED
  uint8_t STAT=readRegisterValue();

  //if( STAT & 0b1){Serial.println("VALID TEMPERATURE MEASUREMENT");}
  //else{Serial.println("TEMPERATURE MEASUREMENT DIDN*T WORK!");};
 
  setRegisterPointer(TEMP_DATA_REG_0);
  reg0=readRegisterValue();
  setRegisterPointer(TEMP_DATA_REG_1);
  reg1=readRegisterValue();
  // binary concatenation
  int reg_16=reg0<<8 | reg1;
  //Serial.print("REG0 ");
  //printlnByteBinaryLeadingZeros(reg0);
  //Serial.print("REG1 ");
  //printlnByteBinaryLeadingZeros(reg1);
  //Serial.print("REGTOT ");
  //Serial.println(reg_16,BIN);
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

