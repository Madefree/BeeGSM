#include <TeltonikaTM1Q.h>
#include <NewSoftSerial.h>

int index;
char name[] = "lab";
char number[] = "0742670510";

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
  //Write phone book
  if (gsm.writePhoneBook(number,name))
    Serial.println("Write");
  //Find what you are added
  if (gsm.findPhoneBook(name,index,number,name)) {
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

