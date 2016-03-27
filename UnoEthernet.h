// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _UnoEthernet_H_
#define _UnoEthernet_H_
#include "Arduino.h"
//add your includes for the project UnoEthernet here
#include <SPI.h>
#include <Ethernet.h>

#include <DHT.h>

//end of add your includes here
#define DHTPIN 9

//add your function definitions for the project UnoEthernet here
void setupEthernet(byte[]);

void askGoogle(EthernetClient& googleClient, const char question[]);

void sendValues(EthernetClient& blueMixClient, const char bmix[], const char path[], float t, float h);

void readEthernetClient(EthernetClient& client);

//class Out: public HardwareSerial {
//public:
//	Out& print(const char str[]) {HardwareSerial::print(str); return &this;};
//};

//Do not add code below this line
#endif /* _UnoEthernet_H_ */
