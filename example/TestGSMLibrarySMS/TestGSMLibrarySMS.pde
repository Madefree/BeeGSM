#include "TeltonikaTM1Q.h"
#include <NewSoftSerial.h>

/*
 * We should put here some copyright stuff.
 *
 * This program is developed just to test-develop the Arduino-TID-GSM libraries.
*/

char msg[200];
int numdata;

void setup() 
{
  //Serial connection.
  Serial.begin(9600);
  Serial.println("BeeGSM testing.");
  //Start configuration.
  if (gsm.begin())
    Serial.println("\nstatus=READY");
  else Serial.println("\nstatus=IDLE");
  
  if (gsm.sendSMS("1234567890", "Hi Guy!"))
    Serial.println("\nSMS sent OK");

};

void loop() 
{
  char smsbuffer[160];
  char n[20];

  if(gsm.readSMS(smsbuffer, 160, n, 20))
  {
    Serial.println(n);
    Serial.println(smsbuffer);
  }
  delay(1000);
};

