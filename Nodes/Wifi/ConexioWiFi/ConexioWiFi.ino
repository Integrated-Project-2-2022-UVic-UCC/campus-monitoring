
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <ESPDateTime.h>

const char* ssid = "PlacidoBravo";
const char* password = "123456789";

const char* serverName = "http://192.168.0.100:8080/api/monitoring";

unsigned long lastTime = 0;

unsigned long timerDelay = 5000;

byte cont = 0;
byte max_intentos = 50;

//Sensor Humitat
int Humitat = A0;
int H;

String data = DateTime.toString();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupDateTime();
  Serial.println("\n");
  Serial.println(data);
  //Conexio WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectant ");
  while (WiFi.status() != WL_CONNECTED and cont < max_intentos){
    cont++; //Conta fins a 50
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  if (cont < max_intentos){
    Serial.println("*****************************************");
    Serial.print("Contectat a la red WiFi: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("macAdress: ");
    Serial.println(WiFi.macAddress());
    Serial.println("*****************************************");
  }
  else {
    Serial.println("-----------------------------------------");
    Serial.print("No s'ha pogut conectar");
    Serial.println("-----------------------------------------");
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - lastTime) > timerDelay){
    if(WiFi.status()==WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");       
      int httpResponseCode = http.POST("{\"timestamp\":\"tPmAT5Ab3j7F9\",\"sensorID\":\"1\",\"var1\":\"24.25\",\"var2\":\"49.54\",\"var3\":\"1005.14\"\"var4\":\"24.25\",\"var5\":\"49.54\",\"var6\":\"1005.14\"\"var7\":\"24.25\"}");
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void setupDateTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout
  // DateTime.setTimeZone("CST-8");
  // DateTime.setServer("asia.pool.ntp.org");
  // DateTime.begin(15 * 1000);
  // from
  /** changed from 0.2.x **/
  DateTime.setTimeZone("CST-8");
  // this method config ntp and wait for time sync
  // default timeout is 10 seconds
  DateTime.begin(/* timeout param */);
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
}
