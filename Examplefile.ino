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
#define MoisturePin 36
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

#define CSE_IP "esw-onem2m.iiit.ac.in"
#define CSE_PORT 443
#define OM2M_ORGIN "fIsOtR:&LTnM1"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "Team-32"
#define OM2M_DATA_CONT "Node-1/Data"

void createCICO2(String &val)
{
  String server = "https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

  http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  String label = "node1";

  int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

  Serial.println(code);
  if (code == -1)
  {
    Serial.println("Unable to connect to the server");
  }
  http.end();
}

void createCImoisture(String &val)
{
  String server = "https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

  http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  String label = "node1";

  int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

  Serial.println(code);
  if (code == -1)
  {
    Serial.println("Unable to connect to the server");
  }
  http.end();
}

void createCITemp(String &val)
{
  String server = "https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

  http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  String label = "node1";

  int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

  Serial.println(code);
  if (code == -1)
  {
    Serial.println("Unable to connect to the server");
  }
  http.end();
}

void createCIHumidity(String &val)
{
  String server = "https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

  http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  String label = "node1";

  int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

  Serial.println(code);
  if (code == -1)
  {
    Serial.println("Unable to connect to the server");
  }
  http.end();
}

void createCILum(String &val)
{
  String server = "https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

  http.begin(server + String() + OM2M_AE + "/" + OM2M_DATA_CONT + "/");

  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  String label = "node1";

  int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

  Serial.println(code);
  if (code == -1)
  {
    Serial.println("Unable to connect to the server");
  }
  http.end();
}

// Thingspeak stuff go brrr
const char *ssid = "esw-m19@iiith";
const char *pass = "e5W-eMai@3!20hOct";

long writeChannelID = 1904954;
const char *writeAPIKey = "FD9BGYZYGXBALW78";

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
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("WiFi Connected!");
  ThingSpeak.begin(client);
}

void loop()
{
  Serial.print("Moisture Sensor Value:");
  float Moisture = analogRead(MoisturePin);
  float soilmoisturepercent = map(Moisture, 4095, 2400, 0, 100);
  Serial.println(soilmoisturepercent);
  ThingSpeak.setField(2, soilmoisturepercent);
  String mstr = (String)soilmoisturepercent
  createCImoisture(mstr)
  //  String dataString2 = "field2=" + String(Moisture);
  //  mqttClient.publish(topicString.c_str(), dataString2.c_str());
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  ThingSpeak.setField(5, lux);
  String lstr = (String)lux
  createCILum(lstr)
  //  String dataString5 = "field5=" + String(lux);
  //  mqttClient.publish(topicString.c_str(), dataString5.c_str());

  //  delay(10000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  ThingSpeak.setField(4, h);
  String hstr = (String)h
  createCIHumidity(hstr)
  //  String dataString4 = "field4=" + String(h);
  //  mqttClient.publish(topicString.c_str(), dataString4.c_str());
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  ThingSpeak.setField(3, t);
  String tstr = (String)t
  createCITemp(tstr)
  //  String dataString3 = "field3=" + String(t);
  //  mqttClient.publish(topicString.c_str(), dataString3.c_str());
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).

  th = pulseIn(Co2PIN, HIGH, 2008000) / 1000;
  tl = 1004 - th;
  ppm = 2000 * (th - 2) / (th + tl - 4);

  Serial.print("Co2 Concentration: ");
  Serial.print(ppm);
  ThingSpeak.setField(1, ppm);
  String pstr = (String)ppm
  createCICO2(pstr)
  Serial.println("ppm");
  //  String dataString1 = "field1=" + String(ppm);
  //  mqttClient.publish(topicString.c_str(), dataString1.c_str());
  if (isnan(h) || isnan(t) || isnan(f))
  {
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
  if (x == 200)
  {
    Serial.println("Channel update successful\n");
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x) + "\n");
  }
  delay(15000);
}
