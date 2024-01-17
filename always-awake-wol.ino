#include <ESP8266WiFi.h>

#include <WiFiUdp.h>

// https://github.com/a7md0/WakeOnLan
#include <WakeOnLan.h>

WiFiUDP UDP;
WakeOnLan WOL(UDP);

const char* ssid     = "<YOUR_SSID>";
const char* password = "<YOUR_PASSWORD>";

void wakeMyPC() {
    // Your HOST PC's mac address
    const char *MACAddress = "00:00:00:00:00:00";

  WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
}

void setup() {
  WOL.setRepeat(3, 100); // Optional, repeat the packet three times with 100ms between. WARNING delay() is used between send packet function.

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
    delay(250);
    Serial.print(".");
  }

  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask()); // Optional  => To calculate the broadcast address, otherwise 255.255.255.255 is used (which is denied in some networks).

}

void loop() {
  for (int i = 0; i < 360; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    wakeMyPC();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(9900);
    if (WiFi.status() != WL_CONNECTED){
      restart();
    }
  }
  restart();
}

void restart() {

  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
    delay(50);
  }
  Serial.println("Restaring...");
  ESP.restart();
}