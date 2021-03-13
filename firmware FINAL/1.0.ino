#include <Wire.h>
#include <EEPROM.h>
#include <FaBoRTC_PCF2129.h>

void writeTime(byte A, byte B, byte DOT);
byte slotMachine(byte H, byte M, byte rollTime);
byte getButton1();
byte getButton2();
byte getButton3();

FaBoRTC_PCF2129 faboRTC;

//pin definitions
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
  //initialise the I2C pins on PA1 and PA2 
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

  //initializes for the first time the BurnIn function
  randomSeed(pwm);                                      
  randHour = (random(8,12) + faboRTC.getHours())%24;
  randMin = 30;

  //restore from the eeprom the saved settings
  restoreSettings();   
  //it checks the buttons status
  buttonsCheck();
  //is used as a workaround. the first time i call this function it return as the button is pressed. form the second time it work normally
  getButton3();


  if(faboRTC.getSeconds() > 60 )    //if seconds >= 60 the RTC has lost power and has to be set again
    initializeClock();
  
  //this two elses are used to detect if a daylight saving time chiange has occurred while the clock was powered off
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

     
  buttonsCheck(); //it continuosly check if the buttons are pressed
 
  if(randHour == faboRTC.getHours() && randMin == faboRTC.getMinutes())    //every time the BurnIn occcurs it set the next BurnIn time randomly
    {
      randHour = (randHour + random(8,12))%24;
      randomSeed(faboRTC.getMonths()+100*faboRTC.getHours()+faboRTC.getDays()*10000); 
      slotMachine((int)random(5,10));
    }
      
  if(getButton3())
    settings();
  
  //refreshes the written time. 
  writeTime(faboRTC.getHours(),faboRTC.getMinutes(),blinking && (millis()/1000)%2); 
}
