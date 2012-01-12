#include <TeltonikaTM1Q.h>
#include <NewSoftSerial.h>

int index = 1;
char name[20];
char number[20];

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
  if (gsm.readPhoneBook(index,number,name)) {
    Serial.print("Index: ");
    Serial.println(index);
    Serial.print("Name: ");
    Serial.println(name);
    Serial.print("Number: ");
    Serial.println(number);
  }
}

void loop() 
{
}

