/*----------------------------------------------------------------------------------------------------
  BOARD 1 PROGRAM (COUNTDOWN AND IGNITION TRIGGER PROGRAM)

 The Board 1 program starts a countdown from 99 seconds on a seven-segment display using the
 "SevSeg.h" library. Once the countdown is over, the digital pin 13 is made high for 3 seconds for
 heating of nickel-chromium wire.

 ALGORITHM:
 1. Include the SevSeg library.
 2. Initialize a SevSeg object with the pins for the seven segment display.
 3. Set the initial count value to 99 seconds.
 4. Enter a while loop that runs until the count value reaches 0.
 5.Update the seven segment display with the current count value.
 6.Decrement the count value by 1 second.
 7. When the count value reaches 0, set digital pin 13 to high for 3 seconds.
 8. End the program.

 Refer GitHub repository for complete details: https://github.com/SPARK-TECH-INDUSTRIES/SPECTRA

  Designed by SPARK TECH INDUSTRIES:
  https://www.thesparktech.xyz/

  ----------------------------------------------------------------------------------------------------
*/


#include <SevSeg.h>
SevSeg sevseg;

int i=99;         //Here you can change the countdown seconds
unsigned long previoustime=0;
unsigned long currenttime;


void setup() 
{
  byte numDigits = 2;
  byte digitPins[] = {9,10}; 
  byte segmentPins[] = {1, 2, 3, 4, 5, 6, 7, 8}; 
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(10);
  pinMode(13,OUTPUT);         // Here define the MOSFET trigger pin address
}

void loop() 
{
  currenttime=millis();
  if(currenttime-previoustime==1000)
  {
    previoustime = currenttime;
    sevseg.setNumber(i);
    i--;
  }

  sevseg.refreshDisplay();

  if(i == -2)
  {

   digitalWrite(13,HIGH);
   delay(3000);         //Here define the dealy for short circuit of Nichrome wire
   digitalWrite(13,LOW);
  }
  if(i == -2)
  {
   exit 0;
  }
}
