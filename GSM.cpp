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

#include "GSM.h"
#include "WideTextFinder.h"


#define _GSM_TXPIN_ 7
#define _GSM_RXPIN_ 8


GSM::GSM():_cell(_GSM_TXPIN_,_GSM_RXPIN_),_tf(_cell, 10),_status(IDLE)
{
	
};

int GSM::begin(char* pin)
{

  _cell.begin(9600);
  setStatus(IDLE);
  return start(pin);
	
}

int GSM::restart(char* pin){ return 1;}

int GSM::write(uint8_t c)
{
  if ((_status == TCPCONNECTEDCLIENT) ||(_status == TCPCONNECTEDSERVER) )
    return write(&c, 1);
  else
    return 0;
}

int GSM::write(const char* str)
{
  if ((_status == TCPCONNECTEDCLIENT) ||(_status == TCPCONNECTEDSERVER) )
      return write((const uint8_t*)str, strlen(str));
  else
      return 0;
}

int GSM::isIP(const char* cadena)
{
    int i;
    for (i=0; i<strlen(cadena); i++)
        if (!(cadena[i]=='.' || ( cadena[i]>=48 && cadena[i] <=57)))
            return 0;
    return 1;
}


int GSM::start(char* pin){return 0;};
int GSM::shutdown(){return 0;};
int GSM::sendSMS(const char* to, const char* msg){return 0;};
int GSM::attachGPRS(char* domain, char* dom1, char* dom2){return 0;};
int GSM::dettachGPRS(){return 0;};
int GSM::connectTCP(const char* server, int port){return 0;};
int GSM::disconnectTCP(){return 0;};
int GSM::connectTCPServer(int port){return 0;};
boolean GSM::connectedClient(){return false;};
int GSM::readCellData(int &mcc, int &mnc, long &lac, long &cellid){return 0;};
int GSM::write(const uint8_t* buffer, size_t sz){return 0;};
int GSM::read(char* result, int resultlength){return 0;};
boolean GSM::availableSMS(){return false;};
boolean GSM::readSMS(char* msg, int msglength, char* number, int nlength){return false;};
boolean GSM::readCall(char* number, int nlength){return false;};
boolean GSM::call(char* number, unsigned int milliseconds){return false;};
int GSM::setPIN(char *pin){return 0;};
int GSM::getCCI(char* cci){return 0;};
int GSM::getIMEI(char* imei){return 0;};
uint8_t GSM::read(){return 0;};







