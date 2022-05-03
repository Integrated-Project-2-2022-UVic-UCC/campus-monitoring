#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

const char *ssid     = "PlacidoBravo";
const char *password = "123456789";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

byte cont = 0;
byte max_intentos = 50;


void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Conectant ");
  while ( WiFi.status() != WL_CONNECTED and cont < max_intentos) {
    cont++;
    delay ( 500 );
    Serial.print ( "." );
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
  timeClient.begin();
}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());

  delay(1000);
}
