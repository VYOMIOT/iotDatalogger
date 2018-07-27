//////////////////////////////////////////////////////////////////////
//V.I.O.M IOT DATA LOGGER,
// scl pin to D1, sda to D2 (nodeMCU)
// Current load ~120mA
// 03/17 BY Divyanshu Anand 
// 03/18 BY Amit
// Under guidance of Alex Vishwa
///////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "rgb_lcd.h"
#define D 5
// Wi-Fi Settings
const char* ssid = "VIOM_iot"; // your wireless network name (SSID)
const char* password = "p49wtc9kvk";
WiFiClient client;
const int channelID = 547045;
String writeAPIKey = "DKI3JQA21VRQVVEL"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 15000 ; // post data every 15 seconds


rgb_lcd lcd;

const int colorR = 64;
const int colorG = 64;
const int colorB = 196;

void setup()
{   
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB); 
   delay(100);
  pinMode(A0, INPUT);
  pinMode(D, INPUT);
  Serial.begin(115200);
  Serial.flush();
  lcd.setCursor(5, 0);
  lcd.write("V.I.O.M");
  lcd.setCursor(1, 1);
  lcd.write("IOT DATA LOGGER");
  delay(100);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
}
void loop()
{
 
  if (client.connect(server, 80))
  {
      lcd.setCursor(0, 0);
      lcd.write("  Reading..");
    
    /*
    float t1 = analogRead(A0);
    Serial.print("analog read is ");
    Serial.println(t1);
    float temp = (t1/1024.0)*33;
    Serial.print("Temp is ");
    Serial.println(temp); 
     // Print a message to the LCD.
     String lcdtext=" Temp:"+String(temp)+"'C";
     lcd.setCursor(0, 0);
    lcd.print(lcdtext);
    delay(10);
     lcd.setCursor(0, 1);
   lcd.print("                ");
   */
double analog_sensor1 = digitalRead(D);
  analog_sensor1 = analog_sensor1*4.88;
double  sensor2 = analogRead(A0);
  sensor2 = sensor2*4.88;
  lcd.setCursor(0,0); Serial.print("CO2(mv)");  lcd.print("CO2(mV)");
  Serial.print(analog_sensor1); lcd.print(analog_sensor1);
  lcd.setCursor(0,1); Serial.print("\tCO(mv)"); lcd.print("CO(mV)");
  Serial.println(sensor2); lcd.print(sensor2);
  delay(1000);
  lcd.clear();
  analog_sensor1= ((analog_sensor1*1996+10)*0.001);
  sensor2=(sensor2)*0.5;
  lcd.setCursor(0,0); Serial.print("CO2(ppm)");  lcd.print("CO2(ppm)");
  Serial.print(analog_sensor1); lcd.print(analog_sensor1);
  lcd.setCursor(0,1); Serial.print("\tCO(ppm)"); lcd.print("CO(ppm)");
  Serial.println(sensor2); lcd.print(sensor2);
  delay(1000);
  lcd.clear();
    // Construct API request body
     String body = "field6=";
     body += String(analog_sensor1);

     body += "&field5=";// for multiple fields
     body += String(sensor2);

     /*body += "&field3=";// for multiple fields
     body += String(volt1);
     body += "&field4=";// for multiple fields
     body += String(volt2);
     body += "&field5=";// for multiple fields
     body += String(cur1);
     body += "&field6=";// for multiple fields
     body += String(cur2);
     body += "&field7=";// for multiple fields
     body += String(volt3);
     body += "&field8=";// for multiple fields
     body += String(cur3);
     body += "&field9=";// for multiple fields
     body += String(volt4);
     body += "&field10=";// for multiple fields
     body += String(cur4);
*/
//    String tsData = "Zarvis(BOT):Temperature @ V.I.O.M office is "+ String(temp)+"'C";
//      char len = tsData.length();
   
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(body.length());
    client.print("\n\n");
    client.print(body);
    client.print("\n\n");

    client.stop();
    lcd.setCursor(0, 1);
    lcd.print(" Sent to cloud");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("              ");
  }
  delay(11000);
}

