#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "DFRobot_BME280.h"
#include "Wire.h"
#include "DFRobot_CCS811.h"
//Sensor bme280 and ccs
DFRobot_CCS811 CCS811;
typedef DFRobot_BME280_IIC    BME;    // ******** use abbreviations instead of full names ********
BME   bme(&Wire, 0x76);   // select TwoWire peripheral and set sensor address
#define SEA_LEVEL_PRESSURE    1015.0f
//Sensor Humitat ground
int Humitat = A0;
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
//-----------------------------------------------------------------
void printLastOperateStatus(BME::eStatus_t eStatus)
{
  switch(eStatus) {
  case BME::eStatusOK:    Serial.println("everything ok"); break;
  case BME::eStatusErr:   Serial.println("unknow error"); break;
  case BME::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
  case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
  default: Serial.println("unknow status"); break;
  }
}
void setup() {
  
  Serial.begin(115200);
  Serial.println("-----------------------------");
  //Initialize Conexion to Server
  ConexioServer();
  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(0);
  //Serial begin sensor C02
  bme.reset();
  Serial.println("bme read data test");
  while(bme.begin() != BME::eStatusOK) {
    Serial.println("bme begin faild");
    printLastOperateStatus(bme.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bme begin success");
  delay(100);

}

void loop() {
  //Void loop for ground Humidity Sensor---------------------
  Serial.print("Valor humitat del sol: ");
  Serial.println(analogRead(Humitat));
  //Get variables of sensor Co2 (bme280 and ccs)
  float   temp = bme.getTemperature();
  uint32_t    press = bme.getPressure();
  float   alti = bme.calAltitude(SEA_LEVEL_PRESSURE, press);
  float   humi = bme.getHumidity();
  float   Co2 = CCS811.getCO2PPM();
  
  Serial.println();
  Serial.println("======== start print ========");
  Serial.print("temperature (unit Celsius): "); Serial.println(temp);
  Serial.print("pressure (unit pa):         "); Serial.println(press);
  Serial.print("altitude (unit meter):      "); Serial.println(alti);
  Serial.print("humidity (unit percent):    "); Serial.println(humi);
  Serial.println("========  end print  ========");

    if(CCS811.checkDataReady() == true){
        Serial.print("CO2: ");
        Serial.print(Co2);
        Serial.print("ppm");

    } else {
        Serial.println("Data is not ready!");
    }
  CCS811.writeBaseLine(0x447B);
  
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
      String httpRequestData = "{\"timestamp\":\"" + currentDate + "\",\"sensorID\":2,\"var1\":" + temp + ",\"var2\":" + humi + ",\"var3\":"+ Co2/1000press/1000 +",\"var4\":"+ alti +",\"var5\":"+ press/1000 +"\,\"var6\":"+ Humitat +",\"var7\":0}";
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
  
  delay(60000);
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
