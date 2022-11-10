#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>


#define DHTPIN 26
unsigned long duration, th, tl;
int ppm;
#define Co2PIN 18 
# define MoisturePin 36
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

// Thingspeak stuff go brrr
const char* ssid = "thisis";
const char* pass = "thatis1234";


long writeChannelID =  1904954;
const char* writeAPIKey = "FD9BGYZYGXBALW78";

int port = 1883;


WiFiClient client;



void setup()
{
  Serial.begin(9600);
  dht.begin(); 
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("WiFi Connected!");
  ThingSpeak.begin(client);
}

void loop()
{
  
  
  Serial.print("Moisture Sensor Value:");
  float Moisture=analogRead(MoisturePin);
  Serial.println(Moisture);
  ThingSpeak.setField(2,Moisture);
//  String dataString2 = "field2=" + String(Moisture);
//  mqttClient.publish(topicString.c_str(), dataString2.c_str());
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  ThingSpeak.setField(5,lux);
//  String dataString5 = "field5=" + String(lux);
//  mqttClient.publish(topicString.c_str(), dataString5.c_str());

  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  ThingSpeak.setField(4,h);
//  String dataString4 = "field4=" + String(h);
//  mqttClient.publish(topicString.c_str(), dataString4.c_str());
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  ThingSpeak.setField(3,t);
//  String dataString3 = "field3=" + String(t);
//  mqttClient.publish(topicString.c_str(), dataString3.c_str());
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).

  th = pulseIn(Co2PIN, HIGH, 2008000) / 1000;
  tl = 1004 - th;
  ppm = 2000 * (th - 2) / (th + tl - 4);

  if (ppm > 1000) {
//    Serial.print("Hello");
    Serial.println(ppm);
    ThingSpeak.setField(1,ppm);
    //ThingSpeak.setField(1, ppm);
  }
  else {
    Serial.print(" Co2 Concentration: ");
    Serial.println(ppm);
    ThingSpeak.setField(1, ppm);
        Serial.println(" ppm");
  }
//  String dataString1 = "field1=" + String(ppm);
//  mqttClient.publish(topicString.c_str(), dataString1.c_str());
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  int x = ThingSpeak.writeFields(writeChannelID, writeAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000);
}
