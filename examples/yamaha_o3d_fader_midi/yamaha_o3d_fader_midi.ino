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


#include "yamaha_o3d.h"
#include <USBComposite.h>



int newdata = 0;
int _channel = 0;
int _note = 0;
int _velocity = 0;

YamahaFader yfader;

class myMidi : public USBMIDI {
 virtual void handleControlChange(unsigned int channel, unsigned int controller, unsigned int value) {
    if(channel == 1)
    {
      if((controller % 2 ) == 0)
      {
        yfader.setTargetHigh(controller/2, value);
      }
      else
      {
        yfader.setTargetLow((controller-1)/2, value);
        yfader.moveFader((controller-1)/2);
      }
    }
  }
};

myMidi midi;
#ifdef SERIAL_DEBUG
USBCompositeSerial CompositeSerial;
#endif

int loopCounter = 0;

void Update_IT_callback()
{
  yfader.timerWorker();
}

void setup() {
  yfader.begin();

  USBComposite.setProductId(0x0031);
  USBComposite.setProductString("YamahaFader");
  USBComposite.setManufacturerString("chaozlabs");
  USBComposite.setSerialString("yamahaFader");
  
  midi.registerComponent();

#ifdef SERIAL_DEBUG
  CompositeSerial.registerComponent();
#endif
  USBComposite.begin();

  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(1000); 
  Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer2.attachInterrupt(TIMER_CH1, Update_IT_callback);
}

int interval = 10;
int previousMillis = 0;
int lastValue = 0;

int lastFaderValue[FADER_NUM];

void loop() {
  yfader.faderWorker();
  midi.poll();

  int val = 0;
  int channel = 0;

  int MSB = 0;
  int LSB = 0;
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    loopCounter++;
    previousMillis = currentMillis;

    int sfader = 0;
    for(int i = 0; i< FADER_NUM; i++)
      {
        if(yfader.getMidiFader(i, &MSB, &LSB) == 1)
        {
           midi.sendControlChange(channel, i *2, MSB);
           midi.sendControlChange(channel, i *2 +1, LSB);
        }
      }
  }
}
