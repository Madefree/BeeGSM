#include "TeltonikaTM1Q.h"
#include <NewSoftSerial.h>
#include "inetGSM.h"

InetGSM inet;
char msg[200];
int numdata;

void setup() 
{
  //Serial connection.
  Serial.begin(9600);
  gsm.debug(true);
  Serial.println("BeeGSM testing.");
  //Start configuration.
  if (gsm.begin())
    Serial.println("\nstatus=READY");
  else Serial.println("\nstatus=IDLE");

  //GPRS attach
  if (gsm.attachGPRS("ibox.tim.it", "username", "password"))
    Serial.println("status=ATTACHED");
  else Serial.println("status=ERROR");
  //TCP Client GET.
  numdata=inet.httpGET("www.google.com", 80, "/", msg);
  Serial.println("\nNumber of data received:");
  Serial.println(numdata);  
  Serial.println("Data received:"); 
  Serial.println(msg);

  //Tweet
  //inet.tweet("*********************key************", "An Arduino at #cpes15");
  
  // TCP Server.
//  if (gsm.connectTCPServer(80))
//    Serial.println("status=TCPSERVERWAIT");
//  else Serial.println("ERROR in Server");
}

void loop() 
{
/*  if (gsm.connectedClient())   
  {
    gsm.read(msg, 200);
    Serial.println(msg);
    gsm.write((const uint8_t*)"Read Data", 9);
  }
*/
}

