#include "WiFiS3.h"
#include "WiFiSSLClient.h"
#include "arduino_secrets.h"
#include <WiFiUdp.h>
#include <Arduino_JSON.h>
#include "RTC.h"
#include <Wire.h>
#include <NTPClient.h>

#define magneticSwitch 2

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0; // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

char server[] = "discord.com";
char path[] = SECRET_webhook;
WiFiSSLClient client;

bool lastState = false;

int counter = 0; // How many times the door has been opened in a day
int lastDay = 0; // Check if the day has changed to reset the counter

void setup()
{
  Serial.begin(9600);
  pinMode(magneticSwitch, INPUT_PULLUP);

  RTC.begin();

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  timeClient.begin();
  timeClient.update();

  RTCTime timeToSet = timeClient.getEpochTime();
  RTC.setTime(timeToSet);

  RTCTime currentTime;
  RTC.getTime(currentTime);

  RTC.getTime(currentTime);
  Serial.println("The RTC was just set to: " + String(currentTime));
}

void loop()
{
  RTCTime currentTime;
  RTC.getTime(currentTime);

  if (client.available())
  {
    Serial.println("client available");
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
  else
  {
    int sensorValue = digitalRead(magneticSwitch);

    if (sensorValue != lastState)
    {
      if (sensorValue == HIGH)
      {
        Serial.println("Door opened");
        if (lastDay != currentTime.getDayOfMonth())
        {
          counter = 0;
          lastDay = currentTime.getDayOfMonth();
        }
        counter++;
        sendWebhook(sensorValue);
      }
      else
      {
        Serial.println("Door closed");
        sendWebhook(sensorValue);
      }
      lastState = sensorValue;

      delay(1000);
    }
  }
}

String listFormats[5] = {"RELATIVE", "DATE", "TIME", "SHORT TIME", "FULL"};
String listUnformatted[5] = {"R", "D", "T", "t", "F"};

String discordTimestamp(int timestamp, String format)
{
  int index = -1;

  for (int i = 0; i < 5; i++)
  {
    if (listFormats[i].equalsIgnoreCase(format))
    {
      index = i;
      break;
    }
  }

  if (index == -1)
  {
    return "Invalid format";
  }

  return "<t:" + String(timestamp) + ':' + listUnformatted[index] + '>';
}

void sendWebhook(int sensorValue)
{
  RTCTime currentTime;
  RTC.getTime(currentTime);

  String content = sensorValue ? "The door was opened!" : "The door was closed!";
  JSONVar body;
  body["content"] = content;
  body["embeds"][0]["title"] = "The door was " + String(sensorValue ? "opened" : "closed") + "!";
  body["embeds"][0]["description"] = "The door was " + String(sensorValue ? "opened" : "closed") + " " + discordTimestamp(currentTime.getUnixTime(), "RELATIVE");
  body["embeds"][0]["color"] = sensorValue ? 3931904 : 16711680;
  body["embeds"][0]["fields"][0]["name"] = "Opened today";
  body["embeds"][0]["fields"][0]["value"] = String(counter) + " times";

  String bodyString = JSON.stringify(body);

  Serial.println(body);
  Serial.println(bodyString);

  Serial.print("Connecting to server: ");
  Serial.println(server);

  if (client.connect(server, 443))
  {
    Serial.println("connected to server");
    client.println("POST " + String(path) + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(bodyString.length()));
    client.println("Connection: close");
    client.println();
    client.println(bodyString);

    while (client.connected())
    {
      String response = client.readStringUntil('\n');
      if (response.length() == 0)
      {
        break;
      }
      Serial.println(response);
    }
    client.stop();
  }
  else
  {
    Serial.println("connection failed");
  }
}

void printWifiStatus()
{
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
