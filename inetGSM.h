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

#ifndef _INETGSM_H_
#define _INETGSM_H_

#define BUFFERSIZE 200

#include "TeltonikaTM1Q.h"

class InetGSM
{
  private:
    char _buffer[BUFFERSIZE];
    
  public:
    int httpGET(const char* server, int port, const char* path, char* result);
    int httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength);
    
    // Fast and dirty solution. Should make a "mail" object. And by the moment it does not run.
    int openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj);
    int closemail();
    
    // This runs, yes
    int tweet(const char* token, const char* msg);

};

#endif
