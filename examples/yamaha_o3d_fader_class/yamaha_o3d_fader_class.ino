/*
 * Runs on Rogers Core by stm32duino
 * board manager Link:
 * http://dan.drown.org/stm32duino/package_STM32duino_index.json
 * 
 * STM32F1xx/GDF1xx boards
 * 2020.6.7
 *
 */

#include "yamaha_o3d.h"

YamahaFader yfader;

int loopCounter = 0;

void Update_IT_callback()
{
  yfader.timerWorker();
}

void setup() {
  yfader.begin();
  
  Serial.begin(115200);

  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(500); 
  Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer2.attachInterrupt(TIMER_CH1, Update_IT_callback);
}

int interval = 50;
int previousMillis = 0;
int lastValue = 0;

void loop() {
  yfader.faderWorker();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    loopCounter++;
    previousMillis = currentMillis;

    int i;
    int fader = yfader.getFader(18) ;
    int error = abs(lastValue - fader);
    
    if(error > 25)
    {
      //yfader.moveFader(0,fader);
      lastValue = fader;

      // move all fader
      for(i = 0; i < MOTOR_FADER_NUM; i++)
      {
        yfader.moveFader(i,fader );
      }
    }
//    Serial.println("");
//    Serial.print(loopCounter);
//    Serial.println("");
//
//  
//    Serial.print("Fader A: ");Serial.println(yfader.getFader(18));
//    Serial.print("Fader B: ");Serial.println(yfader.getFader(17));
//    Serial.print("Fader C: ");Serial.println(yfader.getFader(16));
    
//    Serial.print("Fader: "); //;Serial.print(i);Serial.print(":");
    for (i = 0; i < 19; i++)
    {
      //Serial.print(yfader.getFader(i));Serial.print(", ");
      Serial.print(yfader.getFader(i)/ 32);Serial.print(", ");
    }
    
    Serial.println("");
  }
}
