#include <Arduino.h>

class vma03Stepper
{
  public:
    vma03Stepper( byte pwmaPin, byte diraPin, 
                  byte pwmbPin, byte dirbPin, 
                  int stepsPerRevolution, 
                  int speed, 
                  int speedOffset);
    void setSpeed(int speed);
    int speed();
    void begin();
    void end();
    void subStep(long steps);
    void setDegree(int degree);
    double degree();
    int currentStep();
    

    
  private:
    byte _pwmaPin;
    byte _diraPin;
    byte _pwmbPin;
    byte _dirbPin;
    int _stepsPerRevolution;
    int _speed;
    int _speedOffset;
    int _currStep = 0;             //current step, corresponds to angular position,  limits: 0...STEPS
    int _sub = 0;                  //current halfstep within repeating sequence (8 halfsteps), limits: 0...7
    
    
    //total count of halfsteps per full rotation (*2 for half steps!)
    // e.g. 1.8deg stepper => 200 steps => 400 
    const int STEPS = 2 * _stepsPerRevolution;  
    
    double _degree;

};



