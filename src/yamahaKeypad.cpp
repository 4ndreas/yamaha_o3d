#include "yamahaKeypad.h"



YamahaKeypad::YamahaKeypad(void)
{
  
}

void YamahaKeypad::begin(void)
{
  pinMode(MATRIX_LOW_0, OUTPUT);
  pinMode(MATRIX_LOW_1, OUTPUT);
  pinMode(MATRIX_LOW_2, OUTPUT);
  pinMode(MATRIX_LOW_3, OUTPUT);
  pinMode(MATRIX_LOW_4, OUTPUT);
  pinMode(MATRIX_LOW_5, OUTPUT);
  pinMode(MATRIX_LOW_6, OUTPUT);
  pinMode(MATRIX_LOW_7, OUTPUT);
  pinMode(MATRIX_LOW_8, OUTPUT);
  pinMode(MATRIX_LOW_9, OUTPUT);

  pinMode(MATRIX_BUTTON_0, INPUT_PULLUP);
  pinMode(MATRIX_BUTTON_1, INPUT_PULLUP);
  pinMode(MATRIX_BUTTON_2, INPUT_PULLUP);
  pinMode(MATRIX_BUTTON_3, INPUT_PULLUP);
  pinMode(MATRIX_BUTTON_4, INPUT_PULLUP);
  pinMode(MATRIX_BUTTON_5, INPUT_PULLUP);
  pinMode(MATRIX_BUTTON_6, INPUT_PULLUP);
  pinMode(MATRIX_BUTTON_7, INPUT_PULLUP);
  
  pinMode(MATRIX_LED_0, OUTPUT);
  pinMode(MATRIX_LED_1, OUTPUT);
  pinMode(MATRIX_LED_2, OUTPUT);
  pinMode(MATRIX_LED_3, OUTPUT);
  pinMode(MATRIX_LED_4, OUTPUT);
  pinMode(MATRIX_LED_5, OUTPUT);
  pinMode(MATRIX_LED_6, OUTPUT);
  pinMode(MATRIX_LED_7, OUTPUT);

  digitalWrite(MATRIX_LED_0, LOW);
  digitalWrite(MATRIX_LED_1, LOW);
  digitalWrite(MATRIX_LED_2, LOW);
  digitalWrite(MATRIX_LED_3, LOW);
  digitalWrite(MATRIX_LED_4, LOW);
  digitalWrite(MATRIX_LED_5, LOW);
  digitalWrite(MATRIX_LED_6, LOW);
  digitalWrite(MATRIX_LED_7, LOW); 
  
}

int YamahaKeypad::getButtonRAW(int but)
{
  int c = but / MATRIX_ROWS;
  int r = but % MATRIX_ROWS ;

  return (readBuffer[c][r]);
}

int YamahaKeypad::getButton(int but, int * stat)
{
  int butStat = getButtonRAW(but);
  int ret = 0;
  if( lastreadBuffer[but] != butStat)
  {
    // something changed since last read
    ret = 1;
  }
  *stat = butStat;
  lastreadBuffer[but] = butStat;
  return ret;
}

void YamahaKeypad::setLED(int led, int stat)
{
  int c = led / MATRIX_ROWS;
  int r = led % MATRIX_ROWS ;
  ledBuffer[c][r] = stat;
}


// Private Hardware functions

void YamahaKeypad::timerWorker(void)
{
  setLedOff();              // set LED off
  readMatrix(matrix_stat);  // read Button State
  
  matrix_stat++;
  if(matrix_stat >= MATRIX_COLUMNS)
  {
    matrix_stat = 0;
  }
  
  setMatrixLow(matrix_stat);  // switch to next column
  setLed(matrix_stat);// set next LED ROW
}

void YamahaKeypad::setLedOff(void)
{
   digitalWrite(MATRIX_LED_0, HIGH);
   digitalWrite(MATRIX_LED_1, HIGH);
   digitalWrite(MATRIX_LED_2, HIGH);
   digitalWrite(MATRIX_LED_3, HIGH);
   digitalWrite(MATRIX_LED_4, HIGH);
   digitalWrite(MATRIX_LED_5, HIGH);
   digitalWrite(MATRIX_LED_6, HIGH);
   digitalWrite(MATRIX_LED_7, HIGH); 
}

void YamahaKeypad::setLed(int column)
{

   if(ledBuffer[column][0] == 0){digitalWrite(MATRIX_LED_0, LOW);}
   if(ledBuffer[column][1] == 0){digitalWrite(MATRIX_LED_1, LOW);}
   if(ledBuffer[column][2] == 0){digitalWrite(MATRIX_LED_2, LOW);}
   if(ledBuffer[column][3] == 0){digitalWrite(MATRIX_LED_3, LOW);}
   if(ledBuffer[column][4] == 0){digitalWrite(MATRIX_LED_4, LOW);}
   if(ledBuffer[column][5] == 0){digitalWrite(MATRIX_LED_5, LOW);}
   if(ledBuffer[column][6] == 0){digitalWrite(MATRIX_LED_6, LOW);}
   if(ledBuffer[column][7] == 0){digitalWrite(MATRIX_LED_7, LOW);}  

//   for(int j = 0; j< MATRIX_ROWS; j++)
//   {
//     if( lastreadBuffer[column][j] != readBuffer[column][j])
//     {
//        ledBuffer[column][j] = readBuffer[column][j];
//     }
//     lastreadBuffer[column][j] = readBuffer[column][j];
//   }
}


void YamahaKeypad::setMatrixLow(int column)
{
   digitalWrite(MATRIX_LOW_0, LOW);
   digitalWrite(MATRIX_LOW_1, LOW);
   digitalWrite(MATRIX_LOW_2, LOW);
   digitalWrite(MATRIX_LOW_3, LOW);
   digitalWrite(MATRIX_LOW_4, LOW);
   digitalWrite(MATRIX_LOW_5, LOW);
   digitalWrite(MATRIX_LOW_6, LOW);
   digitalWrite(MATRIX_LOW_7, LOW);
   digitalWrite(MATRIX_LOW_8, LOW);
   digitalWrite(MATRIX_LOW_9, LOW);

   if(column == 0)digitalWrite(MATRIX_LOW_0, HIGH);
   else if(column == 1)digitalWrite(MATRIX_LOW_1, HIGH);
   else if(column == 2)digitalWrite(MATRIX_LOW_2, HIGH);
   else if(column == 3)digitalWrite(MATRIX_LOW_3, HIGH);
   else if(column == 4)digitalWrite(MATRIX_LOW_4, HIGH);
   else if(column == 5)digitalWrite(MATRIX_LOW_5, HIGH);
   else if(column == 6)digitalWrite(MATRIX_LOW_6, HIGH);
   else if(column == 7)digitalWrite(MATRIX_LOW_7, HIGH);
   else if(column == 8)digitalWrite(MATRIX_LOW_8, HIGH);
   else if(column == 9)digitalWrite(MATRIX_LOW_9, HIGH);
}



void YamahaKeypad::readMatrix(int column)
{
  readBuffer[column][0] = 1 - digitalRead(MATRIX_BUTTON_0);
  readBuffer[column][1] = 1 - digitalRead(MATRIX_BUTTON_1);
  readBuffer[column][2] = 1 - digitalRead(MATRIX_BUTTON_2);
  readBuffer[column][3] = 1 - digitalRead(MATRIX_BUTTON_3);
  readBuffer[column][4] = 1 - digitalRead(MATRIX_BUTTON_4);
  readBuffer[column][5] = 1 - digitalRead(MATRIX_BUTTON_5);
  readBuffer[column][6] = 1 - digitalRead(MATRIX_BUTTON_6);
  readBuffer[column][7] = 1 - digitalRead(MATRIX_BUTTON_7);
}

void YamahaKeypad::copyBuffer(void)
{
  for(int i = 0; i< MATRIX_COLUMNS; i++)
  {
      for(int j = 0; j< MATRIX_ROWS; j++)
      {
        ledBuffer[i][j] = readBuffer[i][j];
      }
  }
}

void YamahaKeypad::setAllLeds(int val)
{
  for(int i = 0; i< MATRIX_COLUMNS; i++)
  {
      for(int j = 0; j< MATRIX_ROWS; j++)
      {
        ledBuffer[i][j] = val;
      }
  }
}
