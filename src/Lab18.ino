#include "MQTT.h"
#include <blynk.h>
#include "oled-wing-adafruit.h"

SYSTEM_THREAD(ENABLED);

double lat = 0.0;
double longitude = 0.0;

void callback(char *topic, byte *payload, unsigned int length);

OledWingAdafruit display;

SYSTEM_THREAD(ENABLED);

WidgetMap bMap(V0);

MQTT client("lab.thewcl.com", 1883, callback);

// to recieve messages
void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String s = p;
   Serial.println(p);
  // Serial.println(value);
  if ((String) topic == "NODERED2")
    {
     
      lat = s.toFloat();
      Serial.println(lat);
      delay(1000);
    }

  if ((String) topic == "NODERED3")
    {
      longitude = s.toFloat();
      Serial.println(longitude);
      delay(1000);
    }
}

void setup()
{
  Blynk.begin("rPvjevy8R-9N6CXt0GOrIu2meSjpbP1C", IPAddress(167, 172, 234, 162), 9090); // in setup
  Serial.begin(9600);
  display.setup();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void loop()
{
  client.loop();
  display.loop();
  Blynk.run();

  bMap.location(1, lat, longitude, "Point");

  if (client.isConnected())
  {
    client.subscribe("NODERED2"); // lat
    client.subscribe("NODERED3"); // long
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe("NODERED2"); // lat
    client.subscribe("NODERED3"); // long
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(lat);
  display.println(longitude);
  display.display();
