#include <ESPDateTime.h>
String h = DateTime.toString();
int Humitat = A0;


void setup() {
  delay(1000);
  Serial.begin(115200);
  setupDateTime();
  //Serial.println(DateTime.now());
  Serial.println("--------------------");
  Serial.println(DateTime.toString());
  Serial.println(h);
  //Serial.println(DateTime.toISOString());
  //Serial.println(DateTime.toUTCString());
  //Serial.println(DateTime.format(DateFormatter::COMPAT));
  //Serial.println(DateTime.format(DateFormatter::DATE_ONLY));
  //Serial.println(DateTime.format(DateFormatter::TIME_ONLY));
  //DateTimeParts p = DateTime.getParts();

}

void loop() {
  Serial.print("Valor humitat del sol: ");
  Serial.println(analogRead(Humitat));

  Serial.println("--------------------");
  Serial.println("{\"timestamp\":\"2020-04-26T08:45:12\",\"sensorID\":2,\"var1\":24.25,\"var2\":49.54,\"var3\":84.14,\"var4\":24.25,\"var5\":49.54\,\"var6\":145.14,\"var7\":24.25}");
  Serial.println("--------------------");
  Serial.println("{\"timestamp\":\"" +h+"\",\"sensorID\":2\,\"var1\":\Humitat\}");
  delay(5000);
}
void setupDateTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout
  // DateTime.setTimeZone(-4);
  //   DateTime.setServer("asia.pool.ntp.org");
  //   DateTime.begin(15 * 1000);
  DateTime.setTimeZone("CST-8");
  DateTime.begin();
  /*if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  } else {
    Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
    Serial.printf("Timestamp is %ld\n", DateTime.now());
  }*/
}
