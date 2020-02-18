#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "sps30.h"

#define SP30_COMMS I2C_COMMS

const char* ssid = "kku-wifi@true";
const char* pass = "";
String urlPost = "http://202.28.95.210:1880/pm2p3/";
String nodeName = "41";

SPS30 sps30;
HTTPClient http;

uint8_t ret;
struct sps_values val;
String payload;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  sps30.begin(SP30_COMMS);
  sps30.start();
  sps30.I2C_expect();
}

void loop() {
  ret = sps30.GetValues(&val);

  Serial.print(F("HTTP-POST = "));
  Serial.print(F("PM 1.0 : "));
  Serial.print(val.MassPM1);
  Serial.print(F(" ,PM 2.5 : "));
  Serial.print(val.MassPM2);
  Serial.print(F(" ,PM 10 : "));
  Serial.print(val.MassPM10);
  Serial.print(F("\n"));
  delay(5000);

  payload = "[{\"id\":"+nodeName+",\"pm1\":"+val.MassPM1+",\"pm2\":"+val.MassPM2+",\"pm10\":"+val.MassPM10+"}]";
  http.begin(urlPost);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  http.end();
}
