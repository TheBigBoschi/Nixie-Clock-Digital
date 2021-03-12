
void writeTime(byte A, byte B, byte DOT);
void setBrightness(byte var);
byte getBrightness();

void settings()
{
  byte menu = 1;
  byte subMenu = 0;
  byte num1;
  byte num2;
  byte run = 1;
  
  while(run)
  {    
    buttonsCheck();
    
    if(getButton1())
      menu++;
    if(getButton2())
      menu--;
    if(getButton3())
      subMenu = 1;
      
    if(menu == 0)
      menu = 7;
    else if(menu == 8)
      menu = 1;

    writeTime(0,menu,ledStat);
    //delay(500);
     
    if(subMenu == 0)
      writeTime(0,menu,0);
    else if(subMenu == 1)
    {
      switch(menu)
      {
        case 1:   // imposta ora
        {
          timeSetting();
          subMenu = 0;
          break;
        }
        case 2:   //imposta anno e data
        {
          dateSetting();
          subMenu = 0;
          break;
        }
        case 3:   //luminosita
        {
          brightnessSetting();
          subMenu = 0;
          break;
        }
        case 4:   //ora legale
        {
          dayLightSavingSetting();
          subMenu = 0;
          break;
        }
        case 5:   // "burn in" forza la slot machine on per num minuti
        {
          storeSettings();
          burnIn();
          subMenu = 0;
          return;
        }
        case 6:
        {
          blinkSetting();
          subMenu = 0;
          break; 
        }
        case 7:
        {
          storeSettings();
          return;
        }
      }
    }
  }
} 


void initializeClock()
{
  faboRTC.setDate(2020,10,1,0,0,0);
  restoreSettings();
  dateSetting();
  timeSetting();
  byte stat = 1;
  
  if(faboRTC.getMonths() > 10 && faboRTC.getMonths() < 3)   //corregge automaticamente l'ora legale nel caso non abbia aggiornato l'ora per mancanza di corrente
    stat = 0;
   else if (faboRTC.getMonths() == 3 && ((faboRTC.getDays() == isLastSundayOfTheMonthPassed() && faboRTC.getHours() >= 2) || faboRTC.getDays() > isLastSundayOfTheMonthPassed()) )
    stat = 1;
  else if(faboRTC.getMonths() == 10 && ((faboRTC.getDays() == isLastSundayOfTheMonthPassed() && faboRTC.getHours() >= 3) || faboRTC.getDays() > isLastSundayOfTheMonthPassed()) )
    stat = 0;
 

  if (stat == 1 && dayLightSavingStat == 0)
     {
      faboRTC.setHours(faboRTC.getHours() + 1);
      dayLightSavingStat = 1;
     }
  else if (stat == 0 && dayLightSavingStat == 1)
    {
      faboRTC.setHours(faboRTC.getHours() - 1);
      dayLightSavingStat = 0;
     }
    
  EEPROM.update(4,dayLightSavingStat);
}

void timeSetting()
{
  byte H = faboRTC.getHours();
  byte M = faboRTC.getMinutes();
  
  while(true)
  {
    buttonsCheck();
    if(getButton1())
      H++;
    if(getButton2())
      H--;
    if(H == 24)
      H = 0;
    if(H == 255)
      H = 23;
    if(getButton3())
      break;
      
    writeTime(H,M,(millis()/100)%2);
  }

  while(true)
  {
    buttonsCheck();
    if(getButton1())
      M++;
    if(getButton2())
      M--;
    if(M == 60)
      M = 0;
    if(M == 255)
      M = 59;
    if(getButton3())
      break;
    writeTime(H,M,(millis()/100)%2);
  }
  
  faboRTC.setHours(H);
  faboRTC.setMinutes(M);
  faboRTC.setSeconds(0);
}


void dateSetting()
{
  byte num1 = (int)faboRTC.getYears()%100;
  byte num2;
  while(true)
  {
    buttonsCheck();
    if(getButton1())
      num1++;
    if(getButton2())
      num1--;
    if(num1 == 100)
      num1 = 1;
    if(num1 == 0)
      num1 = 99;
    if(getButton3())
      break;
    writeTime(20,num1,(millis()/1000)%2);
  }
  faboRTC.setYears(num1);
  num1 = faboRTC.getDays();
  num2=faboRTC.getMonths();
  while(true)
  {
    buttonsCheck();
    if(getButton1())
      num2++;
    if(getButton2())
      num2--;
    if(num2 == 13)
      num2 = 1;
    if(num2 == 0)
      num2 = 12;
    if(getButton3())
      break;
    writeTime(num2,num1,(millis()/100)%2);
  }
  while(true)
  {
    buttonsCheck();
    if(getButton1())
      num1++;
    if(getButton2())
      num1--;
    if(num1 > daysMonth(num2))
      num1 = 1;
    if(num1 == 0)
      num1 = daysMonth(num2);
    if(getButton3())
      break;
    writeTime(num2,num1,(millis()/100)%2);
  }
  faboRTC.setMonths(num2);
  faboRTC.setDays(num1);
}

byte isLastSundayOfTheMonthPassed()
{
  byte M = faboRTC.getMonths();
  byte D = faboRTC.getDays();
  byte DOW = faboRTC.getWeekdays(); //day of week
  byte lastSunday = 0;
  
  while(D <= daysMonth(M))
  {
    if(DOW % 7 == 0)
      lastSunday = D;    
    D++;
    DOW++;
  }

  return lastSunday;
}

void brightnessSetting()
{
  byte num = pwm/8;
  while(true)
  {
    buttonsCheck();
    
    if(getButton1())
      num--;
    if(getButton2())
      num++;
    if(getButton3())
      break;
    if(num > 31)
      num = 1;
    if(num == 0)
      num = 31;
    writeTime(0,32-num,(millis()/100)%2);
    analogWrite(PWM, num*8);
  }
  setBrightness(num*8);
}

void dayLightSavingSetting()
{
  while(true)
  {
    buttonsCheck();
    if(getButton1())
      dayLightSaving = !dayLightSaving;
    if(getButton2())
      dayLightSaving = !dayLightSaving;
    if(getButton3())
      break;
    writeTime(0,dayLightSaving,(millis()/100)%2 );
  }
}

void burnIn()
{
  byte num = 1;
  byte num2 = 0;
  while(true)
  {
    buttonsCheck();
    if(getButton1())
      num++;
    if(getButton2())
      num--;
    if(num == 60)
    {
      num = 0;
      num2++;
    }
    if(num == 255 && num2 > 0)
      num2--;
    else if(num == 255)
      num = 0;
    if(getButton3())
      break;
      
    writeTime(num2,num, (millis()/100)%2 );
  }
  slotMachine((int)num+60*num2);
}

byte daysMonth(byte M)
{
  if(M == 1 || M == 3 || M == 5 || M == 7 || M == 8 || M == 10 || M == 12)
    return 31;
  else 
  {
    if (M == 2)
    {
    byte Y = faboRTC.getYears();
    if((!(Y%4) && Y%100) || (!(Y%100) && !(Y%400)))
      return 28;
    }
    return 30;
  }
}

void storeSettings()
{
  EEPROM.update(1,getBrightness());
  EEPROM.update(2,dayLightSaving);
  EEPROM.update(3,getBlinking());
  EEPROM.update(4,dayLightSavingStat);
}

void restoreSettings()
{
  setBrightness(EEPROM.read(1));
  dayLightSaving = EEPROM.read(2);
  setBlinking(EEPROM.read(3));
  dayLightSavingStat = EEPROM.read(4);
}


void blinkSetting()
{
  byte var = getBlinking();
  while(true)
  {
    buttonsCheck();
    if(getButton1())
      var = !var;
    if(getButton2())
      var = !var;
    if(getButton3())
      break;
    writeTime(0,var,(millis()/100)%2);
  }
  setBlinking(var);
  return;
}

byte getBlinking()
{
  return blinking;
}

void setBlinking(byte var)
{
  blinking = var;
}

void setBrightness(byte var)
{
  pwm = var;
  analogWrite(PWM, var);
}

byte getBrightness()
{
  return pwm;
}
