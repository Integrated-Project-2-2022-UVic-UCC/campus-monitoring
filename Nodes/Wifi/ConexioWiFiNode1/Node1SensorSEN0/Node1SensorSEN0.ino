#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "DFRobot_BME280.h"
#include "Wire.h"
#include "DFRobot_CCS811.h"

//Sensor CO2
DFRobot_CCS811 CCS811;
typedef DFRobot_BME280_IIC    BME;
BME   bme(&Wire, 0x76);
#define SEA_LEVEL_PRESSURE    1015.0f

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.0.100");
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

//Define ESP to Server
const char* ssid = "PlacidoBravo";
const char* password = "123456789";
const char* serverName = "http://192.168.0.100:8080/api/monitoring";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
byte cont = 0;
byte max_intentos = 50;

/*void ConexioServer(){
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
}*/
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
  // put your setup code here, to run once:
  Serial.begin(115200);
  //ConexioServer();
  //Sensor BME (Temp,Humit,Altitud,pression)
  bme.reset();
  Serial.println("bme read data test");
  while(bme.begin() != BME::eStatusOK) {
    Serial.println("bme begin faild");
    printLastOperateStatus(bme.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bme begin success");
  delay(100);
  //Sensor CCS811 CO2
  while(CCS811.begin() != 0){
        Serial.println("failed to init chip, please check if the chip connection is fine");
        delay(1000);

  timeClient.begin();
  timeClient.setTimeOffset(0);
}
}

void loop() {
  /*   //Codi per agafar la data del servidor-------------------
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
  //--------------------------------------------------------------------*/
  float   temp = bme.getTemperature();
  uint32_t    press = bme.getPressure();
  float   alti = bme.calAltitude(SEA_LEVEL_PRESSURE, press);
  float   humi = bme.getHumidity();
  int CO2 = CCS811.getCO2PPM();
  
  Serial.println();
  Serial.println("======== start print ========");
  Serial.print("temperature (unit Celsius): "); Serial.println(temp);
  Serial.print("pressure (unit pa):         "); Serial.println(press);
  Serial.print("altitude (unit meter):      "); Serial.println(alti);
  Serial.print("humidity (unit percent):    "); Serial.println(humi);
  Serial.println("========  end print  ========");

    if(CCS811.checkDataReady() == true){
        Serial.print("CO2: ");
        Serial.print(CO2);
        Serial.print("ppm");

    } else {
        Serial.println("Data is not ready!");
    }
  CCS811.writeBaseLine(0x447B);
    /*
  //--------------------------------------------------------------------
  
  if ((millis() - lastTime) > timerDelay){
    Serial.println("--------------------");
    if(WiFi.status()==WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);
      
      
      http.addHeader("Content-Type", "application/json");
      String httpRequestData = "{\"timestamp\":\"" + currentDate + "\",\"sensorID\":1,\"var1\":12,\"var2\":"+ temp +",\"var3\":"+ press +",\"var4\":"+ alti +",\"var5\":"+ humi +",\"var6\":"+ CO2 +",\"var7\":24.25}";
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
  }*/
  delay(1000);
}
