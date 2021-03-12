
byte slotMachine(int rollTime)
{
  byte oldBrightness = getBrightness();
  setBrightness(0);
  int number;
  int number2;
  int finalNumber;
  int x;
  bool flagSlot = false;

  byte finalH = (faboRTC.getHours()  + rollTime/60)%24;
  byte finalM = ((int)faboRTC.getMinutes()  + rollTime)%60;
  
  while(faboRTC.getHours() != finalH || faboRTC.getMinutes() != finalM )    //sparaflasha fino a quando non passa rollTime
  {
    buttonsCheck();

    if(getButton3())
      break;
      
    if(flagSlot == false)
      {
        number = 0;
        flagSlot = true;    
      }
      
    number+= SLOT_NUM;
    
    if(number > 99)
      number = 0;
      
    writeTime(number ,number,(millis()/SLOT_DELAY_BLINK)%2);  
    delay(SLOT_DELAY);
  }

    if(flagSlot == true)     //fine sparaflashio, setta flag x up to speed
    {
    flagSlot = false;
    while(number != 0)
    {
      number+= SLOT_NUM;
      if(number >= 99)
        number = 0;
      delay(SLOT_DELAY);
      writeTime(number,number,(millis()/SLOT_DELAY_BLINK)%2);  
    }

    setBrightness(oldBrightness);
    writeTime(0,0,1);  
    delay(2000);
    writeTime(255,255,1); 
    delay(2000);
    writeTime(0,0,1);  
    delay(1000);
    //writeTime(255,255,1); 
    //delay(200);
    
    number = 0;
    finalNumber = faboRTC.getHours()*100 + faboRTC.getMinutes();
  
    while(finalNumber-number > SLOT_TRESHOLD)
    {
      number++;
      delay(SLOT_DELAY_ROLL);
      writeTime(number/100,number%100,1);  
    }
    finalNumber = faboRTC.getHours()*100 + faboRTC.getMinutes();
    
    while(true) //up to speed
    {
      number++;
      if(number == finalNumber)
        break;
        
      writeTime(number/100,number%100,1);  //ToDo: funzione logaritmica per il tempo di avanzamento delle cifre
      
      x = ((float)1500*SLOT_MUL)/(finalNumber-number+1);  //<- fatta 
      delay(x ? x : 1);   //short if, se x vero ( >= 1 ) aspetta x, altrimenti 1 ms
    }
    delay(1000);
  } 
}


void rollIn()
{
  int number;
  int finalNumber;
  int x;
  
  number = 0;
  finalNumber = faboRTC.getHours()*100 + faboRTC.getMinutes();

  while(finalNumber-number > SLOT_TRESHOLD)
  {
    number++;
    delay(SLOT_DELAY_ROLL);
    writeTime(number/100,number%100,1);  
  }
  finalNumber = faboRTC.getHours()*100 + faboRTC.getMinutes();
  
  while(true) //up to speed
  {
    number++;
    if(number == finalNumber)
      break;
      
    writeTime(number/100,number%100,1);  //ToDo: funzione logaritmica per il tempo di avanzamento delle cifre
    
    x = ((float)1500*SLOT_MUL)/(finalNumber-number+1);  //<- fatta 
    delay(x ? x : 1);   //short if, se x vero ( >= 1 ) aspetta x, altrimenti 1 ms
  }
}
