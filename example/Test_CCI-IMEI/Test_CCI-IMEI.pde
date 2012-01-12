#include <TeltonikaTM1Q.h>
#include <NewSoftSerial.h>

char imei[16];
char cci[21];

void setup() 
{ 
  //Serial connection.
  Serial.begin(9600);
  Serial.println("GSM + Daisy11 test started");
  //Start configuration.
  gsm.debug(false);
  if (gsm.begin())
    Serial.println("\nstatus=READY");
  else Serial.println("\nstatus=IDLE");
  //Read phone book by index
  if (gsm.getIMEI(imei)) {
    Serial.print("IMEI: ");
    Serial.println(imei);
  }
  if (gsm.getCCI(cci)) {
    Serial.print("CCI: ");
    Serial.println(cci);
  }
}

void loop() 
{
}

