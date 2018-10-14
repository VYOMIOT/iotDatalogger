//////////////////////////////////////////////////////////////////////
//V.I.O.M IOT DATA LOGGER,
// scl pin to D1, sda to D2 (nodeMCU)
// Current load ~120mA
// 03/17 BY Divyanshu Anand 
// 03/18 BY Amit
// Under guidance of Alex Vishwa
///////////////////////////////////////////////////////////////////////////
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#define D 5
#include "FS.h"
float a,b,c,d,e,f,g;
//StaticJsonDocument<512> doc;
DynamicJsonBuffer jsonBuffer(512);
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
 
      WiFiManager wifiManager;
    //reset settings - for testing
    //wifiManager.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setTimeout(120);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    if (!wifiManager.startConfigPortal("DATALOGGER")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
    //  ESP.reset();
      delay(5000);
    }
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  if (client.connect(server, 80))
  {
     File f = SPIFFS.open("/doc", "r");
    if (!f) {
      Serial.println("Count file open failed on read.");
    } else {
      while(f.available()) {
        //Lets read line by line from the file
        String line = f.readStringUntil('\n');
        JsonObject& root = jsonBuffer.parseObject(line);
  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  //root["C1"]=a;
  //root["C2"]=b;
  //root["C3"]=c;
  //root["C4"]=d;
  //root["V1"]=e;
  //root["T1"]=f;
  //root["T2"]=g;
    //break; //if left in, we'll just read the first line then break out of the while.
      } 
      f.close();
    }
  
      lcd.setCursor(0, 0);
      lcd.write("  Reading..");
    
    /*
    double t1 = analogRead(A0);
    Serial.print("analog read is ");
    Serial.println(t1);
    double temp = (t1/1024.0)*33;
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
//double analog_sensor1 = digitalRead(D);
//  analog_sensor1 = analog_sensor1*4.88;
double  sensor2 = analogRead(A0);
  sensor2 = sensor2*4.88;
  lcd.setCursor(0,0); //Serial.print("CO2(mv)");  lcd.print("CO2(mV)");
  //Serial.print(analog_sensor1); lcd.print(analog_sensor1);
   Serial.print("\tCO(mv)"); lcd.print("CO(mV)");
  Serial.println(sensor2); lcd.print(String(sensor2));
  delay(10);
  lcd.print("              ");
//  analog_sensor1= ((analog_sensor1*1996+10)*0.001);
  sensor2=(sensor2)*0.5;
  //lcd.setCursor(0,0); Serial.print("CO2(ppm)");  lcd.print("CO2(ppm)");
  //Serial.print(analog_sensor1); lcd.print(analog_sensor1);
  lcd.setCursor(0,1); Serial.print("\tCO(ppm)"); lcd.print("CO(ppm)");
  Serial.println(sensor2); lcd.print(String(sensor2));
  delay(10);
  lcd.print("              ");
    
    // Construct API request body
     String body = "field6=";
     body += String(sensor2);

     //body += "&field5=";// for multiple fields
     //body += String(sensor2);

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
  else
  {
    String jlineSend;
    File f = SPIFFS.open("/doc", "w"); 
   // JsonObject obj = doc.to<JsonObject>();
   JsonObject& obj=jsonBuffer.createObject();
    obj["C1"]=jsonBuffer.createObject();
    obj["C2"]=jsonBuffer.createObject();
    obj["C3"]=jsonBuffer.createObject();
    obj["C4"]=jsonBuffer.createObject();
    obj["T1"]=jsonBuffer.createObject();
    obj["T2"]=jsonBuffer.createObject();
    obj["V1"]=jsonBuffer.createObject();
    obj["C1"]=a;
  Serial.print(F("Sending: "));
//  serializeJsonPretty(obj, Serial);
  obj.printTo(jlineSend);
  
  Serial.println();

  // Disconnect
  //client.stop();
    if (!f) {
      Serial.println("Count file open failed on update.");
    } else { 
      f.println(jlineSend);
      f.close();
      Serial.println("Adding one");
    }
      f.println(0); 
      f.close();
 
    }
  delay(11000);
}

