#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <ESPDateTime.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.0.100");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("-----------------------------");
  ConexioServer();
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(0);
}

void loop() {
  //Codi per agafar la data del servidor-------------------
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  String currentMonthName = months[currentMonth-1];
  int currentYear = ptm->tm_year+1900;

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "T" + String(formattedTime);
  Serial.print("Current date: ");
  Serial.println(currentDate);
  
  //--------------------------------------------------------------------
  
  if ((millis() - lastTime) > timerDelay){
    Serial.println("--------------------");
    if(WiFi.status()==WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);
      
      
      http.addHeader("Content-Type", "application/json");
      String httpRequestData = "{\"timestamp\":\"" + currentDate + "\",\"sensorID\":2,\"var1\":24.25,\"var2\":49.54,\"var3\":84.14,\"var4\":24.25,\"var5\":49.54\,\"var6\":145.14,\"var7\":24.25}";
      int httpResponseCode = http.POST(httpRequestData);
      Serial.println(httpRequestData);
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
  delay(1000);
}

void ConexioServer(){
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
