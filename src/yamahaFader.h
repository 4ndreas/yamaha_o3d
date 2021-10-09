#ifndef yamahaFader_h
#define yamahaFader_h

#include "Arduino.h"

//#define SERIAL_DEBUG

#define DRV_CLR    PA10

#define DRV_DATA0  PC13
#define DRV_DATA1  PB8
#define DRV_DATA2  PB13
#define DRV_DATA3  PB12
#define DRV_DATA4  PB15
#define DRV_DATA5  PB14
#define DRV_DATA6  PA9
#define DRV_DATA7  PA8

#define DRV_CLK5   PB7
#define DRV_CLK4   PB6
#define DRV_CLK3   PB5
#define DRV_CLK2   PB4
#define DRV_CLK1   PB3

// ADC Inputs
#define DRV_MUX_B  PC14
#define DRV_MUX_A  PC15

#define DRV_ADC3   PA1
#define DRV_ADC2   PA2
#define DRV_ADC1   PA3
#define DRV_MUX2_Y PA4
#define DRV_MUX2_X PA5
#define DRV_MUX1_Y PA6
#define DRV_MUX1_X PA7

#define MOTOR_FADER_NUM 18
#define FADER_NUM 19

#define FADER_FILTER 20
#define FADER_UPDATE 12

#define PWM_MAX 100
#define SPEED_MAX 255

#define MAX_MOTOR_FADER_ERROR 100
#define MOTOR_TIMEOUT 750

class YamahaFader
{
  public:
    YamahaFader(void);
    
    void begin(void);
    void timerWorker(void);
    void faderWorker(void);
    
    // Fader
    int getFaderRAW(unsigned int number);
    int getFader(unsigned int number, int * val);
    int getMidiFader(unsigned int number, int * MSB, int * LSB);

    // Motors
    void moveFader(unsigned int number);
    void moveFader(unsigned int number, int target);
    void setTargetLow(unsigned int number, int targetLow);
    void setTargetHigh(unsigned int number, int targetHigh);
 
  private:
    // Fader functions
    void adcRead(void);
    void setFader(int fader, int val);
    // Motor functions
    void moveMotor(int number);
    void setMFader(int number, int fader);
    void setMotorBuffer(int number, int pin_A, int pin_B);
    void setMotor(int number, int speed, int enable);
    void setOutputLine(int row, int val);


    // Private Variables
    int pwmCounter;
    int motorMoving = 0;
    int fader_raw_A = 0;
    int fader_raw_B = 0;
    int fader_raw_C = 0;
    
    int fader_raw[FADER_NUM];

    int fader_filterIndex[FADER_NUM];
    int lastFader[FADER_NUM];
    uint32_t fader_filter[FADER_NUM][FADER_FILTER];
    int readCounter = 0;
    
    int out_Buff[8][5];

    uint8_t motorMoveing[MOTOR_FADER_NUM];
    uint32_t motorStart[MOTOR_FADER_NUM];
    uint32_t motorTarget[MOTOR_FADER_NUM];
    uint32_t motorTargetLow[MOTOR_FADER_NUM];
    uint32_t motorTargetHigh[MOTOR_FADER_NUM];
};


#endif
