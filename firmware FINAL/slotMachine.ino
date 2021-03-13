//this file contains the BurnIn core. it's used to avoid chatode poisoning, in which a chatode that is usually off gets "dirty" 
//and after a while it stops turning on or its very dim.

//the function is basically divided in two, slotMachine keep ciclyng all the digits for a specific amount of time, 
//then rollIn counts up to the current time in a aesthetically pleasing way.


byte slotMachine(int rollTime)
{
  byte oldBrightness = getBrightness(); // before starting it saves the brightness and it crank it to the maximum. at the end it restores the old value.
  setBrightness(0);
  int number;
  int number2;
  int finalNumber;
  int x;
  bool flagSlot = false;

  byte finalH = (faboRTC.getHours()  + rollTime/60)%24;
  byte finalM = ((int)faboRTC.getMinutes()  + rollTime)%60;
  
  while(faboRTC.getHours() != finalH || faboRTC.getMinutes() != finalM )    //it keeps rolling the digits till the necessary time has passed 
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

    if(flagSlot == true)     //the necessary time has passed. now it exit the function and (outside) rollIn has to be invoked
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

    setBrightness(oldBrightness);   //this is just a visual gimmick, just to "break" from rolling to counting back to the correct time
    writeTime(0,0,1);  
    delay(2000);
    writeTime(255,255,1); 
    delay(2000);
    writeTime(0,0,1);  
    delay(1000);
    
    number = 0;
    finalNumber = faboRTC.getHours()*100 + faboRTC.getMinutes();
  
    while(finalNumber-number > SLOT_TRESHOLD)
    {
      number++;
      delay(SLOT_DELAY_ROLL);
      writeTime(number/100,number%100,1);  
    }
    finalNumber = faboRTC.getHours()*100 + faboRTC.getMinutes();
    
    while(true) 
    {
      number++;
      if(number == finalNumber)
        break;
        
      writeTime(number/100,number%100,1);  
      
      x = ((float)1500*SLOT_MUL)/(finalNumber-number+1);  //this function set the speed at which the counter counts. as we get nearer the desired value it slows down
      delay(x ? x : 1);   //if the advancment time is too fast, it just wait 1ms
    }
    delay(1000);
  } 
}
