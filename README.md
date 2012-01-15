# GSM Library for Bee Board (an Arduino compatible) by Madefree Electronics, in collaboration with Acme Systems.

More info: http://www.madefree.eu
           http://www.acmesystems.it

## Features:

* Work to W4 in Software Serial mode
* Receive/Transmit SMS
* Receive/Transmit Call
* Read/Write/Find SIM PhoneBook

## Functions:

### int begin()

Initialize modem and return 1 if is associated with gsm cell, 0 otherwise.

### boolean readCall(char* number, int nlength)

Return *true* if call arrived from **number**, *false* otherwise.

**nlength** is number length

### boolean call(char* number, unsigned int milliseconds)

Call **number** for **milliseconds** timeout.

Return *true* at the end.

### boolean readSMS(char* msg, int msglength, char* number, int nlength)

Return *true* if **msg** arrived from **number**

**msglength** is the message length

**nlength** is the number length

### int sendSMS(const char* to, const char* msg)

Return 1 if have sent **msg** **to** number.

### int readAllPhoneBook(char* phonebook)

Return the number of tuple in the SIM Phone Book.

**phonebook** is a list of tuple

### int readPhoneBook(int index, char* number, char* text)

Write in **number** and **text** parameters entry in **index** location from the phonebook

Return 1 if matched, 0 otherwise.

### int findPhoneBook(char* findtext, int index, char* number, char* text)

Find **findtext** in SIM Phone Book. Write in **index** **number** **text** parameters phonebook entry.

Return 1 if matched, 0 otherwise.

### int writePhoneBook(char* number, char* text)

Store **number** and **text** for contact in Phone Book

Return 1 if OK, 0 otherwise

### int getCCI(char* cci)

Write ICCID of the SIM-card in **cci** parameter.

Return 1 if OK, 0 otherwise

### int getIMEI(char* imei)

Write IMEI of the modem in **imei** parameter.

Return 1 if OK, 0 otherwise


