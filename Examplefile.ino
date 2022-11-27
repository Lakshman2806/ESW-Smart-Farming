const int trigPin = 25;
const int echoPin = 27;
const int fill = 4;
const int filltrigPin = 26;
const int fillechoPin = 33;

#define BLYNK_TEMPLATE_ID "TMPLc4LGbm05"
#define BLYNK_DEVICE_NAME "Fully Automatic Water Dipsenser"
#define BLYNK_AUTH_TOKEN "nnTJU1iHOd74OJoh5OCeuTtNs6acnMbJ"
#define SOUND_SPEED 0.034
#define FILL_SERVOPIN 32
#define DISP_SERVOPIN 2
//  #0define ONE_WIRE_BUS 23

#include <BlynkSimpleEsp32.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "time.h"
#include "ArduinoJson.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "lab@test";
char pass[] = "lab12345";
int flag = 0;
int dispflag=0;
int nodisp=0;
int fillflag=0;
String cse_ip = "192.168.15.76"; 
String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";
String ae="Water_Sensing";
// String cnt1="temperature";
String cnt2="Water_level";
String cnt3="dispcount";
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature TempSensor(&oneWire);
HTTPClient http;
BlynkTimer timer;
BLYNK_WRITE(V0) // Function for button
{
  int value = param.asInt(); // Takes the value of V0

  if (value == 1)
  {
    flag = 1;
    digitalWrite(fill, HIGH); // sets the fill button high
  }
  else
  {
    flag = 0;
    digitalWrite(fill, LOW); // Set digital pin 2 LOW
  }
}

float HandDistance = 5;
float FillMinThreshold = 14;
float FillMaxThreshold = 8;
double duration;
float distanceCm;
double fillduration;
float filldistanceCm;
float temperature;

Servo autoDisp_servo;
Servo autoFill_servo;

void myTimerEvent()
{
  double temp = 26 - filldistanceCm;
  Blynk.virtualWrite(V2, temp);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V3, nodisp);
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(fill, OUTPUT);
  pinMode(filltrigPin, OUTPUT);
  pinMode(fillechoPin, INPUT);
//  pinMode(ONE_WIRE_BUS, INPUT);
//
//  TempSensor.begin();

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, myTimerEvent);

  autoFill_servo.attach(FILL_SERVOPIN);
  autoFill_servo.write(0);

  autoDisp_servo.attach(DISP_SERVOPIN);
  autoDisp_servo.write(0);

  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }
}

void loop() {
  Blynk.run();
  timer.run();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration * SOUND_SPEED / 2;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  if (distanceCm > HandDistance)
  {
    int temp;
    temp = dispflag;
    Serial.println("dispensing OFF");
    autoDisp_servo.write(150);
    dispflag=0;
    nodisp+=dispflag^temp;
  }
  else
  {
    Serial.println("dispensing ON");
    autoDisp_servo.write(0);
    dispflag=1;
  }

  digitalWrite(filltrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(filltrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(filltrigPin, LOW);

  fillduration = pulseIn(fillechoPin, HIGH);

  filldistanceCm = fillduration * SOUND_SPEED / 2;

  Serial.print("fillDistance (cm): ");
  Serial.println(filldistanceCm);

  Serial.println(flag);

  if (flag == 0)
  {
    if ((filldistanceCm >= FillMinThreshold || fillflag==1)&& filldistanceCm > FillMaxThreshold )
    {
      Serial.println("filling ON");
      fillflag = 1;
      autoFill_servo.write(150);
    }
    else
    {
      Serial.println("filling OFF");
      fillflag = 0;
      autoFill_servo.write(0);
    }
  }

  else
  {
    if (filldistanceCm >= FillMaxThreshold)
    {
      Serial.println("isWorking");
      Serial.println("filling ON");
//      autoFill_servo.write(150);
//      delay(100);
      autoFill_servo.write(150);
    }
    else
    {
      Serial.println("filling OFF");
      autoFill_servo.write(0);
      Blynk.virtualWrite(V0, 0);
      flag = 0;
    }
  }

  
  Serial.println(fillflag);
  Serial.println();
 
  String y=(String)(26-filldistanceCm);
  createCIdist(y);
  String z=(String)nodisp;
  createCIdisp(z);
 
  delay(50);
}
void createCIdist(String& val){
http.begin(server + String() + ae + "/" + cnt2 + "/");

http.addHeader("X-M2M-Origin", "admin:admin");
http.addHeader("Content-Type", "application/json;ty=4");
http.addHeader("Content-Length", "100");

String label = "node1";

int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

Serial.println(code);
if(code == -1) {
  Serial.println("Unable to connect to the server");
}
http.end();
}


void createCIdisp(String& val){
http.begin(server + String() + ae + "/" + cnt3 + "/");

http.addHeader("X-M2M-Origin", "admin:admin");
http.addHeader("Content-Type", "application/json;ty=4");
http.addHeader("Content-Length", "100");

String label = "node1";

int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

Serial.println(code);
if(code == -1) {
  Serial.println("Unable to connect to the server");
}
http.end();
}
