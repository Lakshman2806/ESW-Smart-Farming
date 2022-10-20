#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#include <SPI.h>

#define DHTPIN 26
unsigned long duration, th, tl;
int ppm;
#define Co2PIN 18 
# define MoisturePin 25
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

void setup()
{
  Serial.begin(9600);
  dht.begin(); 
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));
}

void loop()
{
  Serial.print("Moisture Sensor Value:");
  Serial.println(analogRead(MoisturePin));
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");


  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).

  th = pulseIn(Co2PIN, HIGH, 2008000) / 1000;
  tl = 1004 - th;
  ppm = 2000 * (th - 2) / (th + tl - 4);

  if (ppm > 1000) {
//    Serial.print("Hello");
    Serial.println(ppm);
    //ThingSpeak.setField(1, ppm);
  }
  else {
    Serial.print(" Co2 Concentration: ");
    Serial.println(ppm);
    //ThingSpeak.setField(1, ppm);
    //    Serial.println(" ppm");
  }
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
  
}