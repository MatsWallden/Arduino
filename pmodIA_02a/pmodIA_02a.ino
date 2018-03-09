
//libraries
#include <Wire.h>

//functions
void setRegisterPointer(int registerAddress);
void sendCommand2ControlRegister(uint8_t command);
void sendCommand2ControlRegister_2(uint8_t command);
uint8_t readRegisterValue(); 
void writeRegisterValue(uint8_t add,uint8_t val);
void printlnByteBinaryLeadingZeros(uint8_t val);
void printControlRegisterStatusCODEstring(uint8_t readControl);
uint16_t measureTemperature();
uint8_t BIT_1;
uint8_t BIT_2;
uint16_t reg0{0};
uint16_t reg1{0};
uint8_t reg00{0};
uint8_t reg11{0};
double R{0};  //Atena: changed to signed integer
double I{0};  //Atena: changed to signed integer
  uint8_t STAT; 
  int ii;
  int jj;
  double M{0.0}; //Atena: changed to double 
  double Sum{0.0};
  double AVG{0.0};
  double IMP{0.0};
  double GF=105*pow(10,-12);


/*Register Map*/

#define IA_ADDRESS              0x0D  // IA 
#define BLOCK_WRITE_CMD         0xA0  // block write command
#define BLOCK_READ_CMD          0xA1  // block read command
#define ADR_PTR_CMD             0xB0  // address pointer command

/*Register Map*/
#define CONTROL_REG             0x80 
#define CONTROL_REG_2           0x81 //Atena
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
// Atena: Now by conneting SEL jumper to GND we have Rfb=100 kOhm
//Atena: Remember that: 0<(Excitation voltage* (Rfb/Zunknown)*PGA gain)<VDD
//VDD= 3.3 V
#define INIT_START_FREQ         0x10
//#define INIT_START_FREQ         0x17 //Atena: Excitation voltage=1 V and PGA gain=1
#define START_FREQ_SWEEP        0x20
//#define START_FREQ_SWEEP        0x27 //Atena: Excitation voltage=1 V and PGA gain=1
#define INC_FREQ                0x30
//#define INC_FREQ                0x37 //Atena: Excitation voltage=1 V and PGA gain=1
#define REPEAT_FREQ             0x40
//#define REPEAT_FREQ             0x47  //Atena: Excitation voltage=1 V and PGA gain=1
#define MEASURE_TEMP            0x90
#define PWR_DOWN                0xA0
//#define PWR_DOWN                0xA7 //Atena: Excitation voltage=1 V and PGA gain=1
#define STAND_BY                0xB0
//#define STAND_BY                0xB7 //Atena: Excitation voltage=1 V and PGA gain=1
#define RESET               0x10 //Atena


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
 // Serial.println("START OF THE RUN");
  
  /*Serial.println("SETTING POINTER TO CONTROL REGISTER"); 
  setRegisterPointer(CONTROL_REG);
  Serial.println("READING THE CONTROL REGISTER");
  uint8_t bla= readRegisterValue();
  printControlRegisterStatusCODEstring(bla>>4);
  */
  //Serial.println("START FREQ SWEEP");
  //uint16_t theTemp=measureTemperature();
/////See here
 //Serial.println("STAND BY MODE");
 //sendCommand2ControlRegister(STAND_BY);//Execute
  //////////////////////////
 // Serial.println("SET THE SWEEP PARAMETERS!!");
  //START FREQUENCY 30kHz
 /* writeRegisterValue((uint8_t)0x82,(uint8_t)0x0F);
  writeRegisterValue((uint8_t)0x83,(uint8_t)0x5C);
  writeRegisterValue((uint8_t)0x84,(uint8_t)0x28);*/

  //START FREQUENCY 90kHz
  writeRegisterValue((uint8_t)0x82,(uint8_t)0x2E);
  writeRegisterValue((uint8_t)0x83,(uint8_t)0x14);
  writeRegisterValue((uint8_t)0x84,(uint8_t)0x7B);

  
  //FREQ INC 
  //INCREMENT 10 Hz
  writeRegisterValue((uint8_t)0x85,(uint8_t)0x00);
  writeRegisterValue((uint8_t)0x86,(uint8_t)0x01);
  writeRegisterValue((uint8_t)0x87,(uint8_t)0x4F);  
  
  //NUMBER OF INC. 1
  writeRegisterValue((uint8_t)0x88,(uint8_t)0x00);
  writeRegisterValue((uint8_t)0x89,(uint8_t)0x01);
  
  //Settling time x2, 100 cycles
  writeRegisterValue((uint8_t)0x8A,(uint8_t)0x01);  
  writeRegisterValue((uint8_t)0x8B,(uint8_t)0x64);
  ///See here
  //Serial.println("STAND BY MODE");
  sendCommand2ControlRegister(STAND_BY);//Execute
  //Atena: Resetting the device by setting the reset pin in Register 0x81
  //Serial.println("RESET");
  sendCommand2ControlRegister_2(RESET);//Execute
  delay(50); //Atena
  ///////////////////
  //Serial.println("INIT_START_FREQ");
  sendCommand2ControlRegister(INIT_START_FREQ);//Execute
  //Serial.println("START_FREQ_SWEEP");
  sendCommand2ControlRegister(START_FREQ_SWEEP);//Execute
  delay(10); //Atena
 
 // Serial.print("IMP "); Serial.println(IMP);

  //Serial.print("R ");Serial.println(R);
  //Serial.print("I ");Serial.println(I);

//Serial.println("Power down mode");
 // sendCommand2ControlRegister(PWR_DOWN);//Execute

  
}

void loop()

{

Sum=0.0;
  for (int ik=1; ik<11; ik++){
  setRegisterPointer(STATUS_REG);//
  
  ii=0;
  do{
        Wire.requestFrom(IA_ADDRESS,1);
        BIT_1 = Wire.read();
        BIT_1 = BIT_1 & 0b00000010;        
        ii++;
      }while(BIT_1 != 0x02 && ii<255);  
  
  setRegisterPointer(STATUS_REG);//
  STAT=readRegisterValue(); // Serial.print("STATUS REG ");  printlnByteBinaryLeadingZeros(STAT);

  setRegisterPointer(0x94); reg00=readRegisterValue();
  setRegisterPointer(0x95); reg11=readRegisterValue();
  //Serial.print("REG00 ");Serial.println(reg00,HEX);
 // Serial.print("REG11");Serial.println(reg11,HEX);
  R=(reg00<<8) | reg11;
    
  setRegisterPointer(0x96);  reg00=readRegisterValue();
  setRegisterPointer(0x97);  reg11=readRegisterValue();
 // Serial.print("REG00 ");Serial.println(reg00,HEX);
 // Serial.print("REG11");Serial.println(reg11,HEX);
  I=(reg00<<8) | reg11;
  M=pow(pow(R,2)+pow(I,2),0.5);
 // M=pow(pow(R,2),0.5);
  Sum+=M;
 // IMP=pow(GF*M,-1);
   jj=0;
 do{
        Wire.requestFrom(IA_ADDRESS,1);
        BIT_2 = Wire.read();
        BIT_2 = BIT_2 & 0b00000100; 
        sendCommand2ControlRegister(REPEAT_FREQ);//Execute       
        jj++;
      }while(BIT_2 != 0x04 && jj<255); 
        // Serial.print("JJ ");Serial.println(jj);
        // Serial.print("BIT_2");Serial.println(BIT_2);
  

  
  
 // Serial.print("ii ");Serial.println(ii);
 // Serial.print("BIT_1 "); Serial.println(BIT_1,BIN);
 // Serial.print("M ");
 //Serial.println(M);
 delay(10); //THIS HAS TO BE HERE!! AT LEAST 5 
  };
   AVG=Sum/10;
   Serial.println(AVG);
   delay(1000);

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

void sendCommand2ControlRegister_2(uint8_t command){
//void sendCommand2ControlRegister(int command){
  Wire.beginTransmission(IA_ADDRESS);
  Wire.write(CONTROL_REG_2);
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

