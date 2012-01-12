#include <TeltonikaTM1Q.h>
#include <NewSoftSerial.h>

int index;
char name[20];
char number[20];

char findtext[] = "daniele";

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
  //Find pattern in phone book by text
  if (gsm.findPhoneBook(findtext,index,number,name)) {
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

