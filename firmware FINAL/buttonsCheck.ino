// this file contains the button cheking routines. it also implements a fast-pressing if a button is being hold pressed (just for + and -)
byte number1 = 0;
byte number2 = 0;

bool button1Old = true;
bool button2Old = true;
bool button3Old = true;
bool flag1 = false;
bool flag2 = false;
bool flag3 = false;

bool oldStat;

static unsigned long button1Time;
static unsigned long button2Time;
static unsigned long button3Time;

byte getButton1()
  {
    oldStat = flag1;
    flag1 = 0;
    return oldStat;
  } 

byte getButton2()
  {
    oldStat = flag2;
    flag2 = 0;
    return oldStat;
  } 

byte getButton3()
  {
    oldStat = flag3;
    flag3 = 0;
    return oldStat;
  } 

void buttonsCheck()
  {
   if (digitalRead(BUTTON1) == LOW && button1Old == true)
    {
      button1Old = false;
      flag1 = true;
      button1Time = millis() + WAITDELAY_LONG;
    }
  else if (digitalRead(BUTTON1) == true && button1Old == false)
    button1Old = true;
  else if (digitalRead(BUTTON1) == false && button1Old == false && millis()> button1Time)
    {
      flag1 = true;
      button1Time = millis() + WAITDELAY_SHORT;
    }

  if (digitalRead(BUTTON2) == LOW && button2Old == true)
    {
      button2Old = false;
      flag2 = true;
      button2Time = millis() + WAITDELAY_LONG;
    }
  else if (digitalRead(BUTTON2) == true && button2Old == false)
    button2Old = true;
  else if (digitalRead(BUTTON2) == false && button2Old == false && millis()> button2Time)
  {
    flag2 = true;
    button2Time = millis() + WAITDELAY_SHORT;
  }

  if (digitalRead(BUTTON3) == LOW && button3Old == true)
    {
      button3Old = false;
      flag3 = true;
      button3Time = millis() + WAITDELAY_LONG;
    }
  else if (digitalRead(BUTTON3) == true && button3Old == false)
    button3Old = true;
  else if (digitalRead(BUTTON3) == false && button3Old == false && millis()> button3Time)
  {
    flag3 = true;
    button3Time = millis() + WAITDELAY_SHORT;
  }
}
