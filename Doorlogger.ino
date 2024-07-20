#include "WiFiS3.h"
#include "WiFiSSLClient.h"
#include "arduino_secrets.h"

#define magneticSwitch 2

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;  // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "google.com";
char path[] = SECRET_webhook;
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
  if (client.available()) {

    Serial.println("client available");

    String line = client.readStringUntil('\r');
    Serial.println(line);
  } else {

    int sensorValue = digitalRead(magneticSwitch);

    if (sensorValue != lastState) {
      if (sensorValue == HIGH) {
        Serial.println("Door opened");
        sendWebhook(sensorValue);
      } else {
        Serial.println("Door closed");
        sendWebhook(sensorValue);
      }
      lastState = sensorValue;

      delay(1000);
    }
  }
}

void sendWebhook(int sensorValue) {

  String body = "{\"embeds\":[{\"title\":\"The door was ";
  body.concat(sensorValue ? "opened" : "closed");
  body.concat("!\",\"description\":\"The door was ");
  body.concat(sensorValue ? "opened" : "closed");
  body.concat("\",\"color\": ");
  body.concat(String(sensorValue ? 3931904 : 16711680));
  body.concat("}]}");

  Serial.println(body);

  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    client.println("POST " + String(path) + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + body.length());
    client.println();
    client.println(body);
    client.stop();
  } else {
    Serial.println("connection failed");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
