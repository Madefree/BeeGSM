#include "TeltonikaTM1Q.h"
#include <NewSoftSerial.h>

void setup() 
{
  //Serial connection.
  Serial.begin(9600);
  Serial.println("BeeGSM testing.");
  //Start configuration.
   gsm.debug(false);
  if (gsm.begin())
    Serial.println("\nstatus=READY");
  else Serial.println("\nstatus=IDLE");
  
//  if (gsm.sendSMS("0123456789", "Hi Guy!"))
//    Serial.println("\nSMS sent OK");

};

void loop() 
{
  char smsbuffer[160];
  char n[20];

  if(gsm.readSMS(smsbuffer, 160, n, 20))
  {
    Serial.print("Number: ");
    Serial.println(n);
    Serial.print("Message: ");
    Serial.println(smsbuffer);
  }
  delay(1000);
};

