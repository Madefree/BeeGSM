/**
 * Copyright (C) 2011 Madefree Electronics <madefree.info@gmail.com>
 * Daniele Sdei, Marco Antonini
 * http://www.madefree.eu
 *
 * This file is part of BeeGSM.
 *
 * BeeGSM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BeeGSM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BeeGSM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TeltonikaTM1Q.h"  
#include "Streaming.h"

#define _GSM_CONNECTION_TOUT_ 5
#define _TCP_CONNECTION_TOUT_ 20
#define _GSM_DATA_TOUT_ 5

//#define RESETPIN 4
#undef RESETPIN

TeltonikaTM1Q gsm;

TeltonikaTM1Q::TeltonikaTM1Q(){};

TeltonikaTM1Q::~TeltonikaTM1Q(){};
  
int TeltonikaTM1Q::restart(char* pin)
{
#ifdef RESETPIN
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH);
  delay(10000);
  digitalWrite(RESETPIN, LOW);
  delay(1000);
#endif

  return configandwait(pin);
}

int TeltonikaTM1Q::start(char* pin)
{

  _tf.setTimeout(_TCP_CONNECTION_TOUT_);

  _cell.flush();
// Just for old style software restart();  
//  _cell << "AT+CFUN=1" <<  _BYTE(cr) << endl; //Comprobar
//   if (!_tf.find("OK")) 
//   {
//     setStatus(IDLE);
//     return 0;
//   }

#ifdef RESETPIN
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH);
  delay(10000);
  digitalWrite(RESETPIN, LOW);
  delay(1000);
#endif

  return configandwait(pin);
}

int TeltonikaTM1Q::configandwait(char* pin)
{
  int connCode;
  _tf.setTimeout(_GSM_CONNECTION_TOUT_);

  if(pin) setPIN(pin); //syv

  // Try 10 times to register in the network. Note this can take some time!
  for(int i=0; i<10; i++)
  {  	
    //Ask for register status to GPRS network.
    _cell << "AT+CGREG?" <<  _BYTE(cr) << endl; 

    //Se espera la unsolicited response de registered to network.
    while (_tf.find("+CGREG: 0,"))  // CHANGE!!!!
	{
		connCode=_tf.getValue();
		if((connCode==1)||(connCode==5))
		{
		  setStatus(READY);
		  
		_cell << "AT+CMGF=1" <<  _BYTE(cr) << endl; //SMS text mode.
		delay(400);
		  // Buah, we should take this to readCall()
		_cell << "AT+CLIP=1" <<  _BYTE(cr) << endl; //SMS text mode.
		delay(400);
		//_cell << "AT+CGACT=0" <<  _BYTE(cr) << endl; //PDP context deactivate to make sure not pending connection.
		//delay(1000);
	  
		  return 1;
		}
	}
  }
  return 0;
}

int TeltonikaTM1Q::shutdown()
{
#ifdef RESETPIN
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH);
  delay(800);
  digitalWrite(RESETPIN, LOW);
  delay(1000);
#endif

  _tf.setTimeout(_TCP_CONNECTION_TOUT_);
  _cell.flush();
  _cell << "AT+CPWROFF" <<  _BYTE(cr) << endl; //Comprobar
   if (_tf.find("OK")) 
   {
     setStatus(IDLE);
     return 1;
   }
   // After shutdown the modem may accept commands giving no answer.
   // We'll play safe
   delay(1000);
   return 0;
}     
  
int TeltonikaTM1Q::sendSMS(const char* to, const char* msg)
{

  //Status = READY or ATTACHED.
  if((getStatus() != READY)&&(getStatus() != ATTACHED))
    return 0;
      
  _tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.

  _cell.flush();

  //AT command to send a SMS. Destination telephone number 
  _cell << "AT+CMGS=\"" << to << "\"" <<  _BYTE(cr) << endl; // Establecemos el destinatario

  //Expect for ">" character.
  if(!_tf.find(">")) return 0;

  //SMS text.
  _cell << msg << _BYTE(ctrlz) << _BYTE(cr) << endl;

  //Expect "OK".
  if(!_tf.find("OK"))
    return 0;
  else  
    return 1;
}

int TeltonikaTM1Q::attachGPRS(char* domain, char* dom1, char* dom2)
{
	
   //Status = READY.
   if (getStatus()!=READY) 
     return 0;
   
  _tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.
 
  _cell.flush();
  
  //Attach to GPRS service.
  _cell << "AT+CGATT=1" <<  _BYTE(cr) << endl;
  
  //Expect "OK".
  if(!_tf.find("OK")) return 0;

  delay(500);
  
  //Set 0 as PDP context.
  _cell << "AT+CGDCONT=0" <<  _BYTE(cr) << endl;
  
  //Expect "OK".
  if(!_tf.find("OK")) return 0;

  delay(200);

  //Set APN parameter for the GPRS profile 0.
  _cell << "AT+NPSD=0,1,\""<< domain << "\"" << _BYTE(cr) << endl;
  
  //Expect "OK".
  if(!_tf.find("OK")) return 0;
  
  delay(200);

  //Set Username parameter for GPRS profile 0
  _cell << "AT+NPSD=0,2,\""<< dom1 << "\"" << _BYTE(cr) << endl;
  
  //Expect "OK".
  if(!_tf.find("OK")) return 0;
  
  delay(200);

  //Set Password parameter for GPRS profile 0
  _cell << "AT+NPSD=0,3,\""<< dom2 << "\"" << _BYTE(cr) << endl;

  //Expect "OK".
  if(!_tf.find("OK")) return 0;

  delay(200);
  
  //Activate GPRS profile 0.
  _cell << "AT+NPSDA=0,3" <<  _BYTE(cr) << endl;

  //Expect "OK". 
  if(_tf.find("OK"))
  {
    setStatus(ATTACHED);
    delay(1000);
    return 1;
  }
  else
  {
    // In this case we dont know the modem mental position
    setStatus(ERROR);
    return 0;
  }
}

int TeltonikaTM1Q::dettachGPRS()
{
  if (getStatus()==IDLE) return 0;
   
  _tf.setTimeout(_GSM_CONNECTION_TOUT_);

  _cell.flush();

  //Deactivate GPRS profile 0.
  _cell << "AT+NPSDA=0,4" <<  _BYTE(cr) << endl;
  
  if(!_tf.find("OK"))
  {
    setStatus(ERROR);
    return 0;
  }
  delay(500);

  setStatus(READY);
  return 1;
}

int TeltonikaTM1Q::connectTCP(const char* server, int port)
{
  _tf.setTimeout(_TCP_CONNECTION_TOUT_);

  //Status = ATTACHED.
  if (getStatus()!=ATTACHED)
    return 0;

  _cell.flush();
  
  //Create a socket and associate it to TCP protocol.
  _cell << "AT+NSOCR=6" <<  _BYTE(cr) << endl;
  
  //Expect "OK".
  if(!_tf.find("OK")) return 0;
  
  //Visit the remote TCP server.
  _cell << "AT+NSOCO=0,\"" << server << "\"," << port <<  _BYTE(cr) << endl;
  
   //Expect "OK". 
  if(_tf.find("OK"))
  {
    setStatus(TCPCONNECTEDCLIENT); 
    delay(200);
    return 1;
  }
  else 
    return 0;
}

int TeltonikaTM1Q::disconnectTCP()
{
  //Status = TCPCONNECTEDCLIENT or TCPCONNECTEDSERVER.
  if ((getStatus()!=TCPCONNECTEDCLIENT)&&(getStatus()!=TCPCONNECTEDSERVER))
     return 0;

  _tf.setTimeout(_GSM_CONNECTION_TOUT_);


  _cell.flush();

  //Close socket 0.
  _cell << "AT+NSOCL=0" <<  _BYTE(cr) << endl;

  //Expect "OK".
  if(!_tf.find("OK")) return 0;

  delay(200);

  //If remote server close connection AT+QICLOSE generate ERROR
  /*if(_tf.find("OK"))
  {
    if(getStatus()==TCPCONNECTEDCLIENT)
      setStatus(ATTACHED);
    else
      setStatus(TCPSERVERWAIT);
    return 1;
  }
  setStatus(ERROR);
  
  return 0;    */
  if(getStatus()==TCPCONNECTEDCLIENT)
      setStatus(ATTACHED);
  else
      setStatus(TCPSERVERWAIT);   
  return 1;
}

int TeltonikaTM1Q::connectTCPServer(int port)
{
  if (getStatus()!=ATTACHED)
     return 0;

  _tf.setTimeout(_GSM_CONNECTION_TOUT_);

  _cell.flush();

  //Set the socket 0 in listening mode.
  _cell << "AT+NSOLI=0," << port <<  _BYTE(cr) << endl;
  if(!_tf.find("OK")) // Should we leave Status in ERROR?
    return 0;
    
  delay(200);  

  if(_tf.find("OK"))
  {
    setStatus(TCPSERVERWAIT);
    return 1;
  }
  else
    return 0;
}

boolean TeltonikaTM1Q::connectedClient()
{
  if (getStatus()!=TCPSERVERWAIT)
     return 0;
 
  // Alternative: AT+QISTAT, although it may be necessary to call an AT 
  // command every second,which is not wise
  _tf.setTimeout(1);
  if(_tf.find("NUSOLI")) 
  {
    setStatus(TCPCONNECTEDSERVER);
    return true;
  }
  else
    return false;
 }

int TeltonikaTM1Q::write(const uint8_t* buffer, size_t sz)
{
   if((getStatus() != TCPCONNECTEDSERVER)&&(getStatus() != TCPCONNECTEDCLIENT))
    return 0;
    
   if(sz>1460)
     return 0;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);

  _cell.flush();
    
  for(int i=0;i<sz;i++)
    _cell << _BYTE(buffer[i]);
  
  //Not response for a write.
  /*if(_tf.find("OK"))
    return sz;
  else
    return 0;*/
    
  return sz;  
}


int TeltonikaTM1Q::read(char* result, int resultlength)
{
  // Or maybe do it with AT+QIRD

  int charget;
  _tf.setTimeout(3);
  // Not well. This way we read whatever comes in one second. If a CLOSED 
  // comes, we have spent a lot of time
    charget=_tf.getString("","",result, resultlength);

  /*if(strtok(result, "CLOSED")) // whatever chain the Q10 returns...
  {
    // TODO: use strtok to delete from the chain everything from CLOSED
    if(getStatus()==TCPCONNECTEDCLIENT)
      setStatus(ATTACHED);
    else
      setStatus(TCPSERVERWAIT);
  }  */
  
  return charget;
}

 int TeltonikaTM1Q::readCellData(int &mcc, int &mnc, long &lac, long &cellid)
{
  if (getStatus()==IDLE)
    return 0;
    
   _tf.setTimeout(_GSM_DATA_TOUT_);
   _cell.flush();
  _cell << "AT+QENG=1,0" << endl; 
  _cell << "AT+QENG?" << endl; 
  if(!_tf.find("+QENG:"))
    return 0;

  mcc=_tf.getValue(); // The first one is 0
  mcc=_tf.getValue();
  mnc=_tf.getValue();
  lac=_tf.getValue();
  cellid=_tf.getValue();
  _tf.find("OK");
  _cell << "AT+QENG=1,0" << endl; 
  _tf.find("OK");
  return 1;
}


boolean TeltonikaTM1Q::readSMS(char* msg, int msglength, char* number, int nlength)
{
  long index;

  if (getStatus()==IDLE)
    return false;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);
  _cell.flush();
  _cell << "AT+CMGL=\"REC UNREAD\"" << endl;
  if(_tf.find("+CMGL: "))
  {
    index=_tf.getValue();
    _tf.getString("\"+", "\"", number, nlength);
    _tf.getString("\n", "\nOK", msg, msglength);
    _cell << "AT+CMGD=" << index << endl;
    _tf.find("OK");
    return true;
  };
  return false;
};


boolean TeltonikaTM1Q::readCall(char* number, int nlength)
{
  int index;

  if (getStatus()==IDLE)
    return false;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);

  if(_tf.find("+CLIP: \""))
  {
    _tf.getString("", "\"", number, nlength);
    _cell << "ATH" << endl;
    delay(500);
    _cell.flush();
    return true;
  };
  return false;
};


int TeltonikaTM1Q::readCallListNumbers(char* numbersList[], int nlength)
{
  int index;
  char number[50];
  int value=-1;	 

  if (getStatus()==IDLE)
    return value;

  _tf.setTimeout(_GSM_DATA_TOUT_);

  if(_tf.find("+CLIP: \""))
  {
    _tf.getString("", "\"", number, 50);
    _cell << "ATH" << endl;
    delay(1000);
    _cell.flush();
    for (int i=0; i<nlength; i++) {
      if(strcmp(numbersList[i],number)==0) {
        value=i;
        break;
      }
    }
  };
  return value;
};

boolean TeltonikaTM1Q::call(char* number, unsigned int milliseconds)
{ 
  if (getStatus()==IDLE)
    return false;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);

  _cell << "ATD" << number << ";" << endl;
  delay(milliseconds);
  _cell << "ATH" << endl;

  return true;
 
}

int TeltonikaTM1Q::setPIN(char *pin)
{
  //Status = READY or ATTACHED.
  if((getStatus() != IDLE))
    return 2;
      
  _tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.

  _cell.flush();

  //AT command to set PIN.
  _cell << "AT+CPIN=\"" << pin << "\"" <<  _BYTE(cr) << endl; // Establecemos el pin

  //Expect "OK".
  if(!_tf.find("OK"))
    return 0;
  else  
    return 1;
}

int TeltonikaTM1Q::write(uint8_t c)
{
  if ((getStatus() == TCPCONNECTEDCLIENT) ||(getStatus() == TCPCONNECTEDSERVER) )
    return write(&c, 1);
  else
    return 0;
}

int TeltonikaTM1Q::write(const char* str)
{
  if ((getStatus() == TCPCONNECTEDCLIENT) ||(getStatus() == TCPCONNECTEDSERVER) )
      return write((const uint8_t*)str, strlen(str));
  else
      return 0;
}

int TeltonikaTM1Q::getCCI(char *cci)
{
  //Status must be READY
  if((getStatus() != READY))
    return 2;
      
  _tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.

  _cell.flush();

  //AT command to get CCID.
  _cell << "AT+QCCID" << _BYTE(cr) << endl; // Establecemos el pin
  
  //Read response from modem
  _tf.getString("AT+QCCID\r\r\r\n","\r\n",cci, 21);
  
  //Expect "OK".
  if(!_tf.find("OK"))
    return 0;
  else  
    return 1;
}
  
int TeltonikaTM1Q::getIMEI(char *imei)
{
      
  _tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.

  _cell.flush();

  //AT command to get IMEI.
  _cell << "AT+GSN" << _BYTE(cr) << endl; 
  
  //Read response from modem
  _tf.getString("AT+GSN\r\r\r\n","\r\n",imei, 15);
  
  //Expect "OK".
  if(!_tf.find("OK"))
    return 0;
  else  
    return 1;
}

uint8_t TeltonikaTM1Q::read()
{
  return _cell.read();
}
