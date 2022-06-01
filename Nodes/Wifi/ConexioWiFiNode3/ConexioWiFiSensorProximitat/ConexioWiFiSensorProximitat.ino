#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

// Define NTP Client to get time ------------------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.0.100");
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
// -------------------------------------------------------------
// Define Server data ------------------------------------------
const char* ssid = "PlacidoBravo";
const char* password = "123456789";
const char* serverName = "http://192.168.0.100:8080/api/monitoring";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
byte cont = 0;
byte max_intentos = 50;
//---------------------------------------------------------------
//Define var HC-SR04---------------------------------------------
const int trigPin = D6;
const int echoPin = D5;
//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
long duration;
float distanceCm;
//-----------------------------------------------------------------
void setup() {
  
  Serial.begin(115200);
  Serial.println("-----------------------------");
  //Initialize Conexion to Server
  ConexioServer();
  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(0);

  //Void setup for HC-SR04
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  //Void loop for HC-SR04 proximity sensor-------------------------
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  // Prints the distance on the Serial Monitor
  Serial.print(duration);
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  
  //Get time and data from server-------------------
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
  int currentYear = ptm->tm_year+1900;
  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "T" + String(formattedTime); 
  Serial.print("Current date: ");
  Serial.println(currentDate);
  
  //--------------------------------------------------------------------
  // Post the data to the server in Json.--------------------
  if ((millis() - lastTime) > timerDelay){
    Serial.println("--------------------");
    if(WiFi.status()==WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);
      
      
      http.addHeader("Content-Type", "application/json");
      String httpRequestData = "{\"timestamp\":\"" + currentDate + "\",\"sensorID\":4,\"var1\":" + distanceCm + ",\"var2\":49.54,\"var3\":84.14,\"var4\":24.25,\"var5\":49.54\,\"var6\":145.14,\"var7\":24.25}";
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
