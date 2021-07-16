#include "MQTT.h"
#include "oled-wing-adafruit.h"
#include <blynk.h>

SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;

float longitude;
float latitude;

WidgetMap myMap(V1);

void callback(char* topic, byte* payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);

// to receive a message from mqtt
void callback(char *topic, byte *payload, unsigned int length){
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String s = p;
  double value = s.toFloat();
  String Topic = String(topic);
  if(Topic == "aalongitude"){
    longitude = value;
  }
  if(Topic == "aalatitude"){
		latitude = value;
  }
  display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
  display.print("longitude: ");
  display.println(longitude);
  display.print("latitude: ");
  display.println(latitude);
  display.display();
  int index = 0;
  myMap.location(index, latitude, longitude, "value");
}

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

  Serial.begin(9600);
  display.setup();
	display.clearDisplay();
	display.display();

  Blynk.begin("0jUcpcb8kvBG_QIlVN5fM965ZuXUz7Ag", IPAddress(167, 172, 234, 162), 9090);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  Blynk.run();
  display.loop();
  if (client.isConnected()) {
    client.loop();
  } else {
    client.connect(System.deviceID());
  }
  // subscribe to ksa topic in mqtt
  client.subscribe("aalongitude");
  client.subscribe("aalatitude");
  client.publish("noodered","send request");
  delay(10000);
  // The core of your code will likely live here.

}