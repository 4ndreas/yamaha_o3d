/*
 *
 * Runs on Rogers Core by stm32duino
 * board manager Link:
 * http://dan.drown.org/stm32duino/package_STM32duino_index.json
 * 
 * STM32F1xx/GDF1xx boards
 * 2020.6.7
 *
 * This is used with a maple mini and Rogers STM32 Arduino Core, since the stm32core does not support composite USB midi devices etc.
 *
 */

#include <USBComposite.h>
#include <RotaryEncoder.h>
#include "yamaha_o3d.h"


#define ENC_A PC13
#define ENC_B PC14

YamahaKeypad keypad;
RotaryEncoder encoder(ENC_A, ENC_B);


void Update_IT_callback()
{
  keypad.timerWorker();
  encoder.tick();
}


class myMidi : public USBMIDI {
 virtual void handleControlChange(unsigned int channel, unsigned int controller, unsigned int value) {
    if(channel == 1)
    {
      keypad.setLED(controller, value);
    }
  }
};

myMidi midi;
//USBCompositeSerial CompositeSerial;


void setup() {

  keypad.begin();

  USBComposite.setProductId(0x0030);
  USBComposite.setProductString("YamahaKeypad");
  USBComposite.setManufacturerString("chaozlabs_YK");
  USBComposite.setSerialString("yamahaKeypad");
  midi.registerComponent();
  //CompositeSerial.registerComponent();
  USBComposite.begin();

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  
  // Rogers core
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(1000); 
  Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer2.attachInterrupt(TIMER_CH1, Update_IT_callback);
}

int loopCounter = 0;
int ledBlinkColumn = 0;
int ledBlinkRow = 0;
int interval = 1000;
int previousMillis = 0;

// encoder 
int lastEncPos = 0;

void loop() {
  midi.poll();
 
  int channel = 0;
  int bstat = 0;
  for(int i= 0; i < BUTTEN_NUMBER; i++)
  {
    if(keypad.getButton(i, &bstat) != 0)
    {
      midi.sendControlChange(channel, i, bstat);
    }
  }
  int newPos = encoder.getPosition();
  if(lastEncPos !=newPos)
  {
    
    lastEncPos = newPos;

    if(newPos >= 0)
    {
      newPos = newPos % 128;
    }
    else
    {
       newPos = 128 +(newPos % 128);
    }
    midi.sendControlChange(channel, BUTTEN_NUMBER+1, newPos);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    // not uses now
  }  
} 
