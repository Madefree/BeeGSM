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
#define _GSM_DATA_TOUT_ 10

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

void TeltonikaTM1Q::debug(boolean value)
{
  _tf.setDebug(value);
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
    _cell << "AT+CREG?" <<  _BYTE(cr) << endl; 

    //Se espera la unsolicited response de registered to network.
    while (_tf.find("+CREG: 0,"))  // CHANGE!!!!
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
  _cell << msg << _BYTE(ctrlz);

  //Expect "OK".
  if(!_tf.find("OK"))
    return 0;
  else  
    return 1;
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
  char number_buff[20];

  if (getStatus()==IDLE)
    return false;
  
  _tf.setTimeout(1);
  _cell.flush();
  _cell << "AT+CMGL=\"REC UNREAD\"" << endl;
  if(_tf.find("+CMGL: "))
  {
    index=_tf.getValue();
    _tf.getString("\"+", "\"", number_buff, nlength);
    _tf.getString("\n", "\nOK", msg, msglength);
    strcpy(number,"+");
    strcat(number,number_buff);
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
  
  _tf.setTimeout(10);

  if(_tf.find("+CLIP: "))
  {
    _tf.getString("\"", "\",", number, nlength);
		if(_tf.find("RING"))
			_cell << "ATH\r";
    delay(500);
    _cell.flush();
    return true;
  };
  return false;
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

int TeltonikaTM1Q::readAllPhoneBook(char* phonebook)
{
  char index[2];
  char number[20];
  char text[20];
  int blength = 0;

  if (getStatus()==IDLE)
    return 0;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);
  _cell.flush();
  
  //Phonebook
  _cell << "AT+CPBR=1,99" <<  _BYTE(cr) << endl;
  
  while(_tf.find("+CPBR: "))
  {
    _tf.getString("",",",index, 2);
    _tf.getString(",\"","\",",number, 20);
    _tf.getString(",\"","\"",text, 20);
    strcat(phonebook, index);
    strcat(phonebook, ",");
    strcat(phonebook, number);
    strcat(phonebook, ",");
    strcat(phonebook, text);
    strcat(phonebook, ";\n");
    blength++;
  }

  return blength;
}

int TeltonikaTM1Q::readPhoneBook(int index, char* number, char* text)
{
  if (getStatus()==IDLE)
    return 0;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);
  _cell.flush();
  
  //Phonebook
  _cell << "AT+CPBR=" << index << "," << index <<  _BYTE(cr) << endl;
  
  if(_tf.find("+CPBR: "))
  {
    _tf.getString(",\"","\",",number, 20);
    _tf.getString(",\"","\"",text, 20);
    _tf.find("OK");
    return 1;
  }
  else
    return 0;
}

int TeltonikaTM1Q::findPhoneBook(char* findtext, int &index, char* number, char* text)
{
  char indexbuf[20];
  
  if (getStatus()==IDLE)
    return 0;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);
  _cell.flush();
  
  _cell << "AT+CPBF=\"" << findtext << "\"" <<  _BYTE(cr) << endl;
  
  if(_tf.find("+CPBF: "))
  {
    index=_tf.getValue();
    _tf.getString("\"","\",",number, 20);
    _tf.getString(",\"","\"",text, 20);
    _tf.find("OK");
    return 1;
  }
  else
    return 0;
}

int TeltonikaTM1Q::writePhoneBook(char* number, char* text)
{
  if (getStatus()==IDLE)
    return 0;
  
  _tf.setTimeout(_GSM_DATA_TOUT_);
  _cell.flush();
  
  _cell << "AT+CPBW=,\"" << number << "\",,\"" << text << "\"" <<  _BYTE(cr) << endl;
  
  if(_tf.find("OK"))
    return 1;
  else
    return 0;
}

int TeltonikaTM1Q::getCCI(char *cci)
{
  //Status must be READY
  if((getStatus() != READY))
    return 2;

  _tf.setTimeout(_GSM_DATA_TOUT_);  //Timeout for expecting modem responses. 	
  _cell.flush();
	
  //AT command to get CCID.
  _cell << "AT+CCID" << _BYTE(cr) << endl; // Establecemos el pin

  //Read response from modem
  _tf.getString("+CCID: ","\r\n",cci, 21);

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
  _cell << "AT+CGSN" << _BYTE(cr) << endl; 
  
  //Read response from modem
    _tf.getString("\n","\n",imei, 16);
  
  //Expect "OK".
  if(!_tf.find("OK"))
    return 0;
  else  
    return 1;
}

