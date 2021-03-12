#include <Wire.h>
#include <EEPROM.h>
#include <FaBoRTC_PCF2129.h>

void writeTime(byte A, byte B, byte DOT);
byte slotMachine(byte H, byte M, byte rollTime);
byte getButton1();
byte getButton2();
byte getButton3();

FaBoRTC_PCF2129 faboRTC;

#define LATCH PIN_PA6
#define PWM PIN_PA5
#define CLOCK PIN_PA4
#define DATA PIN_PA3
#define JS1 PIN_PB5 //spare 1
#define BUTTON1 PIN_PB3
#define BUTTON2 PIN_PB2
#define BUTTON3 PIN_PB1

#define WAITDELAY_LONG 1500
#define WAITDELAY_SHORT 150


#define SLOT_DELAY 20
#define SLOT_DELAY_ROLL 2
#define SLOT_DELAY_BLINK 100
#define SLOT_TRESHOLD 100
#define SLOT_NUM 11
#define SLOT_MUL 1

byte ledStat = 0;
byte pwm = 0;
bool dayLightSaving = true;
bool dayLightSavingStat = 0;
byte timeShift = 0;
bool settingFlag;
bool blinking;

byte randHour;
byte randMin;

void buttonsCheck();


void setup()
{
  Wire.pins(PIN_PA1, PIN_PA2);

  pinMode(LATCH, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(JS1, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  analogWrite(PWM, 0);

  faboRTC.searchDevice();
  faboRTC.configure();

  //inizializza per la prima volta l'orario di burnIn
  randomSeed(pwm);                                      
  randHour = (random(8,12) + faboRTC.getHours())%24;
  randMin = 30;

  restoreSettings();
  buttonsCheck();
  getButton3();


  if(faboRTC.getSeconds() > 60 )    //se getSecond > 60 è perche l'rtc ha perso l'orario e va reimpostato
    initializeClock();
  else if(faboRTC.getMonths() == 3 && faboRTC.getDays()==isLastSundayOfTheMonthPassed() && faboRTC.getHours() == 2 && faboRTC.getMinutes() == 0 && dayLightSaving && dayLightSavingStat == 0)  //se durante il cambio dell'ora legale l'orologio è spento questa routine al primo avvio corregge l'orario
    {
      faboRTC.setHours(faboRTC.getHours() + 1);
      dayLightSavingStat = 1;
      EEPROM.update(4,dayLightSavingStat);
    }
  else if(faboRTC.getMonths() == 10 && faboRTC.getDays()==isLastSundayOfTheMonthPassed() && faboRTC.getHours() == 3 && faboRTC.getMinutes() == 0 && dayLightSaving && dayLightSavingStat == 1)
    {
      faboRTC.setHours(faboRTC.getHours() - 1);
      dayLightSavingStat = 0;
      EEPROM.update(4,dayLightSavingStat);
    }
}

void loop() {

     
  buttonsCheck();
 
  if(randHour == faboRTC.getHours() && randMin == faboRTC.getMinutes())    // randomizza il burnIn
    {
      randHour = (randHour + random(8,12))%24;
      randomSeed(faboRTC.getMonths()+100*faboRTC.getHours()+faboRTC.getDays()*10000); 
      slotMachine((int)random(5,10));
    }
      
  if(getButton3())
    settings();
  
  writeTime(faboRTC.getHours(),faboRTC.getMinutes(),blinking && (millis()/1000)%2);
}
