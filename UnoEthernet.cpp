// Do not remove the include below
#include "UnoEthernet.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char GOOGLE_SERVER[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient googleClient;
EthernetClient bmixClient;

DHT dht(DHTPIN, DHT11);

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	Serial.println("*** Beginning ***");

	setupEthernet(mac);

	dht.begin();

	askGoogle(googleClient,"arduino");
}

void loop() {
	// if the server's disconnected, read temp and resend
	if (bmixClient.connected()) {
		readEthernetClient(bmixClient);
	} else {
		// Reading temperature or humidity takes about 250 milliseconds!
		// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
		float h = dht.readHumidity();
		// Read temperature as Celsius (the default)
		float t = dht.readTemperature();
		Serial.print("Temp=");
		Serial.print(t);
		Serial.print(" H%=");
		Serial.println(h);

		sendValues(bmixClient,"antired.mybluemix.net","/arduino",t,h);
		delay(1000);
	}

//	if(googleClient) {
//		readEthernetClient(googleClient);
//	}
}


void setupEthernet(byte mac[]) {
	// start the Ethernet connection:
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to configure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	} else {
		ip=Ethernet.localIP();
		Serial.print("Connected as IP=");
		Serial.print(ip[0]);Serial.print('.');
		Serial.print(ip[1]);Serial.print('.');
		Serial.print(ip[2]);Serial.print('.');
		Serial.print(ip[3]);Serial.println();
	}
	return;
}

void askGoogle(EthernetClient& googleClient, const char question[]) {
	// if you get a connection, report back via serial:
	if (googleClient.connect(GOOGLE_SERVER, 80)) {
		Serial.println("connected");
		// Make a HTTP request:
		googleClient.print("GET /search?q=");
		googleClient.println(question);
		googleClient.println(" HTTP/1.1");
		googleClient.print("Host: ");
		googleClient.println(GOOGLE_SERVER);
		googleClient.println("Connection: close");
		googleClient.println();
	} else {
		// if you didn't get a connection to the server:
		Serial.println("connection failed");
	}
}

void sendValues(EthernetClient& blueMixClient, const char bmixHost[], const char path[], float t, float h) {
	// make sure we close any opened connection
	blueMixClient.stop();

	// got a connection, report back via serial:
	if (blueMixClient.connect(bmixHost, 80)) {
		Serial.print("connected to ");
		Serial.println(bmixHost);

		// Make a HTTP request:
		blueMixClient.print("GET ");
		blueMixClient.print(path);
		blueMixClient.print("?t=");
		blueMixClient.print(t);
		blueMixClient.print("&h=");
		blueMixClient.print(h);
		blueMixClient.println(" HTTP/1.1");
		blueMixClient.print("Host: ");
		blueMixClient.println(bmixHost);
		blueMixClient.println("Connection: close");
		blueMixClient.println();
	} else {
		// didn't get a connection to the server:
		Serial.print("connection failed to ");
		Serial.println(bmixHost);
	}
}

void readEthernetClient(EthernetClient& client) {
	// read returned data
	// if there are incoming bytes available
	// from the server, read them and print them:
	if (client.available()) {
		char c = client.read();
		Serial.print(c);
	}
}

// The loop function is called in an endless loop
