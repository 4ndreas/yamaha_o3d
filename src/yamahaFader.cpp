#include "yamahaFader.h"
#include "Arduino.h"

#ifdef SERIAL_DEBUG
#include <USBComposite.h>
extern USBCompositeSerial CompositeSerial;
#endif

YamahaFader::YamahaFader(void)
{
  
}

void YamahaFader::begin(void)
{
  pinMode(DRV_MUX2_Y, INPUT_ANALOG);
  pinMode(DRV_MUX2_X, INPUT_ANALOG);
  pinMode(DRV_MUX1_Y, INPUT_ANALOG);
  pinMode(DRV_MUX1_X, INPUT_ANALOG);
  
  pinMode(DRV_ADC1, INPUT_ANALOG);
  pinMode(DRV_ADC2, INPUT_ANALOG);
  pinMode(DRV_ADC3, INPUT_ANALOG);

  pinMode(DRV_MUX_B, OUTPUT);
  pinMode(DRV_MUX_A, OUTPUT);

  pinMode(DRV_CLR,OUTPUT);

  pinMode(DRV_DATA0, OUTPUT);
  pinMode(DRV_DATA1, OUTPUT);
  pinMode(DRV_DATA2, OUTPUT);
  pinMode(DRV_DATA3, OUTPUT);
  pinMode(DRV_DATA4, OUTPUT);
  pinMode(DRV_DATA5, OUTPUT);
  pinMode(DRV_DATA6, OUTPUT);
  pinMode(DRV_DATA7, OUTPUT);

  pinMode(DRV_CLK1, OUTPUT);
  pinMode(DRV_CLK2, OUTPUT);
  pinMode(DRV_CLK3, OUTPUT);
  pinMode(DRV_CLK4, OUTPUT);
  pinMode(DRV_CLK5, OUTPUT);


  uint32_t currenttime = millis();
  for(int i = 0; i < MOTOR_FADER_NUM; i++)
  {
    motorStart[i] = currenttime;
    motorMoveing[i] = 0;
  }  
}


void YamahaFader::timerWorker(void)
{
  if(pwmCounter > PWM_MAX)
  {
    digitalWrite(DRV_CLR,LOW);
    pwmCounter = 0;
    digitalWrite(DRV_CLR,HIGH);
  }
  setOutputLine(0,pwmCounter);
  setOutputLine(1,pwmCounter);
  setOutputLine(2,pwmCounter);
  setOutputLine(3,pwmCounter);
  setOutputLine(4,pwmCounter);
  pwmCounter++;
}

int YamahaFader::getFaderRAW(unsigned int number)
{
  if(number < FADER_NUM)
  {
    return( fader_raw[number]);
  }
  return 0;
}


int YamahaFader::getFader(unsigned int number, int * val)
{
  int ret = 0;
  int sfader = 0;

  // apply filtering 
  for(int i = 0; i < FADER_FILTER; i++)
  {
    sfader += fader_filter[number][i];
  }
  sfader = sfader / FADER_FILTER;
  
  if(abs(lastFader[number] - sfader) > FADER_UPDATE)
  {
    lastFader[number] = sfader;
    ret = 1;
  }
  * val = sfader;
  return ret;
}


int YamahaFader::getMidiFader(unsigned int number, int * MSB, int * LSB)
{
  int sfader = 0;
  int ret = getFader(number, &sfader);

  if (motorMoving > 0)
  {
    return 0;
  }

//  *MSB = sfader >>5;
//  *LSB = ((sfader & 0x1F) << 2 | (sfader >>10));

  if(ret == 1) 
  {
    float fader_relative = (float)sfader / (float)0x0FFF ;
    sfader = int( (float)0x3FFF * fader_relative);
    *MSB = sfader >>7;
    *LSB = (sfader & 0x7F);
    
#ifdef SERIAL_DEBUG
    CompositeSerial.print(number);
    CompositeSerial.print(" fr:");
    CompositeSerial.print(fader_relative);
    CompositeSerial.print(" sf:");
    CompositeSerial.println(sfader);
#endif
  }
  
  return ret;
}

void YamahaFader::setOutputLine(int row, int val)
{
  if(out_Buff[0][row] > val){digitalWrite(DRV_DATA0, HIGH);}  // M0 M4  M8  M12 M17
  if(out_Buff[1][row] > val){digitalWrite(DRV_DATA1, HIGH);}  
  if(out_Buff[2][row] > val){digitalWrite(DRV_DATA2, HIGH);}  // M1 M5  M9  M13
  if(out_Buff[3][row] > val){digitalWrite(DRV_DATA3, HIGH);}
  if(out_Buff[4][row] > val){digitalWrite(DRV_DATA4, HIGH);}  // M2 M7  M11 M15 M16
  if(out_Buff[5][row] > val){digitalWrite(DRV_DATA5, HIGH);}
  if(out_Buff[6][row] > val){digitalWrite(DRV_DATA6, HIGH);}  // M3 M6  M10 M14 M18
  if(out_Buff[7][row] > val){digitalWrite(DRV_DATA7, HIGH);}   

  if(row == 0){digitalWrite(DRV_CLK1, HIGH);}
  else if(row == 1){digitalWrite(DRV_CLK2, HIGH);}
  else if(row == 2){digitalWrite(DRV_CLK3, HIGH);}
  else if(row == 3){digitalWrite(DRV_CLK4, HIGH);}
  else if(row == 4){digitalWrite(DRV_CLK5, HIGH);}

  digitalWrite(DRV_DATA0, LOW);
  digitalWrite(DRV_DATA1, LOW);
  digitalWrite(DRV_DATA2, LOW);
  digitalWrite(DRV_DATA3, LOW);
  digitalWrite(DRV_DATA4, LOW);
  digitalWrite(DRV_DATA5, LOW);
  digitalWrite(DRV_DATA6, LOW);
  digitalWrite(DRV_DATA7, LOW);
  
  digitalWrite(DRV_CLK1, LOW);
  digitalWrite(DRV_CLK2, LOW);
  digitalWrite(DRV_CLK3, LOW);
  digitalWrite(DRV_CLK4, LOW);
  digitalWrite(DRV_CLK5, LOW);
}



void YamahaFader::setMotor(int number, int speed, int enable)
{
  int pwm = abs(speed);
  pwm = map(pwm, 0, SPEED_MAX, 0, PWM_MAX);
  pwm = constrain(pwm, 0, PWM_MAX);
  
  if(enable  ==0)  // disable
  {
    setMotorBuffer(number, 0,0);
    motorMoving = 0;
  }
  else
  {
    motorMoving = 1;
    if(speed < 0)
    {
      setMotorBuffer(number, pwm,0);
    }
    else if(speed >0)
    {
      setMotorBuffer(number, 0,pwm);
    }
    else  // break
    {
      setMotorBuffer(number, PWM_MAX,PWM_MAX);
    }
  }
}
void YamahaFader::setTargetLow(unsigned int number, int targetLow)
{
  motorTargetLow[number] = constrain(targetLow,0,4096);
}

void YamahaFader::setTargetHigh(unsigned int number, int targetHigh)
{
  motorTargetHigh[number] = constrain(targetHigh,0,4096);
}
void YamahaFader::moveFader(unsigned int number)
{ 
  int target =  motorTargetHigh[number]<<5 +  motorTargetLow[number];
  moveFader(number, target);
}

void YamahaFader::moveFader(unsigned int number, int target)
{
  if(number < MOTOR_FADER_NUM)
  {
      motorTarget[number] = constrain(target,0,4096);
      motorStart[number] = millis();
      motorMoveing[number] = 1;
  }
}

void YamahaFader::moveMotor(int number)
{
  if(motorMoveing[number] != 0)
  {
     if (millis() - motorStart[number] < MOTOR_TIMEOUT) 
      {
        int error = motorTarget[number] - fader_raw[number];
        if(abs(error) > MAX_MOTOR_FADER_ERROR)
        {
           setMotor(number, error,1);
           return;
        }
        else
        {
           setMotor(number, 0,1);
           return;         
        }
      }
  }
  setMotor(number, 0,0);
  motorMoveing[number] = 0;
}

void YamahaFader::faderWorker(void)
{
  adcRead();
  for(int i = 0; i < MOTOR_FADER_NUM; i++)
  {
    moveMotor(i);
  }
}

void YamahaFader::setFader(int fader, int val)
{
  fader_raw[fader] = val;


  fader_filter[fader][fader_filterIndex[fader]] = val;

  fader_filterIndex[fader]++;
  if(fader_filterIndex[fader] >= FADER_FILTER)
  {
    fader_filterIndex[fader] = 0;
  }
}

void YamahaFader::adcRead(void)
{
   setFader(16, analogRead(DRV_ADC1));
   setFader(17, analogRead(DRV_ADC2));
   setFader(18, analogRead(DRV_ADC3));

  digitalWrite(DRV_MUX_B, LOW);
  digitalWrite(DRV_MUX_A, LOW);
  setFader(0, analogRead(DRV_MUX1_X));
  setFader(4, analogRead(DRV_MUX1_Y));
  setFader(8, analogRead(DRV_MUX2_X));
  setFader(12, analogRead(DRV_MUX2_Y));

  digitalWrite(DRV_MUX_B, LOW);
  digitalWrite(DRV_MUX_A, HIGH);
  setFader(1, analogRead(DRV_MUX1_X));
  setFader(5, analogRead(DRV_MUX1_Y));
  setFader(9, analogRead(DRV_MUX2_X));
  setFader(13, analogRead(DRV_MUX2_Y));
  
  digitalWrite(DRV_MUX_B, HIGH);
  digitalWrite(DRV_MUX_A, LOW);
  setFader(2, analogRead(DRV_MUX1_X));
  setFader(6, analogRead(DRV_MUX1_Y));
  setFader(10, analogRead(DRV_MUX2_X));
  setFader(14, analogRead(DRV_MUX2_Y));
  
  digitalWrite(DRV_MUX_B, HIGH);
  digitalWrite(DRV_MUX_A, HIGH);
  setFader(3, analogRead(DRV_MUX1_X));
  setFader(7, analogRead(DRV_MUX1_Y));
  setFader(11, analogRead(DRV_MUX2_X));
  setFader(15, analogRead(DRV_MUX2_Y));
}


void YamahaFader::setMotorBuffer(int number, int pin_A, int pin_B)
{
  number = number*2;
  int w = number % 8;
  int u = number / 8;
  out_Buff[w][u] = pin_A;
  
  number++;
  w = number % 8;
  u = number / 8;
  out_Buff[w][u] = pin_B;
}



void YamahaFader::setMFader(int number, int fader)
{
  int testval = map(fader, 0,4096,- SPEED_MAX, SPEED_MAX);
  setMotor(number, testval, 1);  
}
