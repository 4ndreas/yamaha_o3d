#ifndef yamahaKeypad_h
#define yamahaKeypad_h

#include "Arduino.h"

#define MATRIX_BUTTON_0 PA10
#define MATRIX_BUTTON_1 PA9
#define MATRIX_BUTTON_2 PA8
#define MATRIX_BUTTON_3 PB15
#define MATRIX_BUTTON_4 PB14
#define MATRIX_BUTTON_5 PB13
#define MATRIX_BUTTON_6 PB12
#define MATRIX_BUTTON_7 PB0

#define MATRIX_LED_0 PA0
#define MATRIX_LED_1 PA1
#define MATRIX_LED_2 PA2
#define MATRIX_LED_3 PA3
#define MATRIX_LED_4 PA4
#define MATRIX_LED_5 PA5
#define MATRIX_LED_6 PA6
#define MATRIX_LED_7 PA7

#define MATRIX_LOW_0 PB8
#define MATRIX_LOW_1 PB7
#define MATRIX_LOW_2 PB6
#define MATRIX_LOW_3 PB5
#define MATRIX_LOW_4 PB4
#define MATRIX_LOW_5 PB3
#define MATRIX_LOW_6 PA15
#define MATRIX_LOW_7 PA14
#define MATRIX_LOW_8 PC15
#define MATRIX_LOW_9 PB2

#define MATRIX_ROWS 8
#define MATRIX_COLUMNS 10

#define BUTTEN_NUMBER (MATRIX_ROWS * MATRIX_COLUMNS)

class YamahaKeypad
{
  public:
    YamahaKeypad(void);
    void begin(void);
    void timerWorker(void);
    
    void setLED(int led, int stat);
    void setLedOff(void);
    void setLed(int column);

    int getButton(int but, int * stat);
    void setAllLeds(int val);
    int getButtonRAW(int but);

  private:
    void setMatrixLow(int column);
    void readMatrix(int column);
    void copyBuffer(void);


    
    // store last Button state when read
    int lastreadBuffer[MATRIX_COLUMNS * MATRIX_ROWS] ;

    // RAW Matrix
    int readBuffer[MATRIX_COLUMNS][MATRIX_ROWS] ;
    int ledBuffer[MATRIX_COLUMNS][MATRIX_ROWS] ;
    int matrix_stat = 0;
    
};

#endif
