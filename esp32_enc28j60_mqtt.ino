
#include <SPI.h>
#include <EthernetENC.h>
#include "PubSubClient.h"
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char server[] = "www.google.com";    // name address for Google (using DNS)
#define CLIENT_ID       "JEZIEL_78233"
#define INTERVAL        3000 // 3 sec delay between publishing

IPAddress ip(192, 168, 3, 177);
IPAddress myDns(192, 168, 3, 1);

EthernetClient client;
PubSubClient mqttClient;
long previousMillis;

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
   mqttClient.setClient(client);
   mqttClient.setServer("192.168.3.22",1883);
   Serial.println(F("MQTT client configured"));
previousMillis = millis();
  // give the Ethernet shield a second to initialize:
 

  // if you get a connection, report back via serial:

  beginMicros = micros();
}

void loop() {

   auto link = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (link) {
    case Unknown:
      Serial.println("Unknown");
      break;
    case LinkON:
      Serial.println("ON");
      break;
    case LinkOFF:
      Serial.println("OFF");
      break;
  }
  delay(1000);

   if(millis() - previousMillis > INTERVAL) {
    sendData();
    previousMillis = millis();
  }
  mqttClient.loop();
}

void sendData() {
  if(mqttClient.connect(CLIENT_ID)) {
   mqttClient.publish("/MYTEST", "hola_esp32");

 }
}
