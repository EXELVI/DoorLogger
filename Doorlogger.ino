#include "WiFiS3.h"
#include "WiFiSSLClient.h"

#define magneticSwitch 2

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "canary.discord.com";
char path[] = ""
WiFiSSLClient client;

bool lastState = false;

void setup() {
  Serial.begin(9600);
    pinMode(magneticSwitch, INPUT);
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {
  int sensorValue = digitalRead(magneticSwitch);

    if (sensorValue != lastState) {
        if (sensorValue == HIGH) {
        Serial.println("Door opened");
        sendWebhook("Door opened");
        } else {
        Serial.println("Door closed");
        sendWebhook("Door closed");
        }
        lastState = sensorValue;
    }


  delay(1000);
}

void sendWebhook(String message) {
    
    String body = '{"embeds":[{"title":"The door was open!","description":"The door was open!","color":3931904}]}'

    
}