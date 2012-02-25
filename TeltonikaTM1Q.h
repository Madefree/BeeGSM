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

#ifndef TELTONIKATM1Q_H
#define TELTONIKATM1Q_H
#include <NewSoftSerial.h>
#include "GSM.h"

class TeltonikaTM1Q : public virtual GSM
{

  private:
    int configandwait(char* pin);
    int setPIN(char *pin);

  public:
    TeltonikaTM1Q();
    ~TeltonikaTM1Q();
    void debug(boolean value);
    int start(char* pin=0);
    int restart(char* pin=0);
    int shutdown();
    int getCCI(char* cci);
    int getIMEI(char* imei);
    int sendSMS(const char* to, const char* msg);
    boolean readSMS(char* msg, int msglength, char* number, int nlength);
    boolean readCall(char* number, int nlength);
    boolean call(char* number, unsigned int milliseconds);
    boolean readCallAuthPhoneBook(char* number, int nlength,boolean &auth);
    int readPhoneBook(int index, char* number, char* text);
    int findPhoneBook(char* findtext, int &index, char* number, char* text);
    int writePhoneBook(char* number, char* text);
    int readCellData(int &mcc, int &mnc, long &lac, long &cellid);
  
};

extern TeltonikaTM1Q gsm;

#endif

