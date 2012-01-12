#include <TeltonikaTM1Q.h>
#include <NewSoftSerial.h>

char phonebook[120];

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
  //Read all phone book
  gsm.readAllPhoneBook(phonebook);
  Serial.println(phonebook);
}

void loop() 
{
}

