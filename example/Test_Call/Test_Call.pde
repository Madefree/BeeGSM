#include "TeltonikaTM1Q.h"
#include <NewSoftSerial.h>

int milliseconds = 6000;
char number[] = "0123456789";

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
  
  if (gsm.call(number, milliseconds)) {
    Serial.print("\nCALL ");
    Serial.println(number);
  }

};

void loop() 
{
  char n[20];

  if(gsm.readCall(n, 20))
  {
    Serial.println("Received call!");
    Serial.print("Number: ");
    Serial.println(n);
  }
  delay(1000);
};

