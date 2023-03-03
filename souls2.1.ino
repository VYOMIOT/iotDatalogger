///////////////////////////////////////////////////////////////////////////
//V.I.O.M IOT DATA LOGGER,
// scl pin to D1, sda to D2 (nodeMCU)
// Current load ~120mA
// 03/17 BY Divyanshu Anand 
// 03/18 BY Amit
// 06/18 BY Alex Vishwa
// 03/23 BY Alex
///////////////////////////////////////////////////////////////////////////
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DNSServer.h> 
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#define D 5
#include "FS.h" //FS was added from before
//#include <FSTools.h>
//#include <LittleFS.h>

#define TARGET_FS_LAYOUT FST::layout_4m3m
FSTools fstools;
float a,b,c,d,e,f,g;
//StaticJsonDocument<512> doc;
DynamicJsonDocument doc(512);
// Wi-Fi Settings
const char* ssid = "vivo-1901"; //your wireless network name (SSID)
const char* password = "alex7210";
WiFiClient client;
const int channelID = 2049316;
String writeAPIKey = "LOX7D0GNL4PI64FW"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 15000 ; // post data every 15 seconds
bool migrateFS() {
  if (!fstools.attemptToMountFS(LittleFS)) {  //  Attempts to mount LittleFS without autoformat...
    Serial.println(F("Default FS not found"));
    if (fstools.mountAlternativeFS(FST::SPIFFS /* FST::LITTLEFS */, TARGET_FS_LAYOUT, true)) {
      Serial.println(F("Alternative found"));
      if (fstools.moveFS(LittleFS)) {
        Serial.println(F("FileSystem Moved New FS contents:"));
        fstools.fileListIterator(LittleFS, "/" , [](File& f) {
          Serial.printf_P(PSTR(" File: %-30s [%8uB]\n"), f.fullName(), f.size());
        });
        return true;
      }
    }
  }
  return false;
}
void setup()
{ 
  migrateFS();
  pinMode(A0, INPUT);
  pinMode(D, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
}
void loop()
{
 //Starting Wifi in STA** mode
    WiFiManager wifiManager;
    wifiManager.setSaveConfigCallback(saveConfigCallback);
     if (!wifiManager.autoConnect("Vivo-1901","alex7210")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
    /*
    if (!wifiManager.startConfigPortal("DATALOGGER")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
    //  ESP.reset();
      delay(5000);
    }
    */
    Serial.println("connected...yeey :)");
    if (client.connect(server, 80)) //Online mode
    {
      /*
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
  // store values in SPIFFS in form of JSON
  */
    doc["C1"]=a;
  //root["C2"]=b;
  //root["C3"]=c;
  //root["C4"]=d;
  //root["V1"]=e;
  //root["T1"]=f;
  //root["T2"]=g;
    //break; //if left in, we'll just read the first line then break out of the while.
      serializeJson(doc, Serial);
      } 
      f.close();
    }
  
    
    double adcVol = 3.33/256 ; 
    double reading = analogRead(A0);
    double sensor1= adcVol * reading;
    double sensor2= 2* sensor1;
    double volt1= 3* sensor1;
    double volt2= 4* sensor1;
    double cur1= 5* sensor1;
    double cur2= 6* sensor1;
     String body = "field1=";
     body += String(sensor1);
     body += "&field2=";// for multiple fields
     body += String(sensor2);
     body += "&field3=";// for multiple fields
     body += String(volt1);
     body += "&field4=";// for multiple fields
     body += String(volt2);
     body += "&field5=";// for multiple fields
     body += String(cur1);
     body += "&field6=";// for multiple fields
     body += String(cur2);
     //body += "&field7=";// for multiple fields
     //body += String(volt3);
     //body += "&field8=";// for multiple fields
     //body += String(cur3);
     //body += "&field9=";// for multiple fields
     //body += String(volt4);
     //body += "&field10=";// for multiple fields
     //body += String(cur4);
/**/
   
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
  }
  else //Online mode
  {
    String jlineSend;
    File f = SPIFFS.open("/doc", "w"); 
    deserializeJson(doc, json);
    //TODO:: Loop through json document and for every cycle convert values and store them
    double c1=doc["C1"];
    double c2=doc["C2"];
    double t1=doc["T1"];
    double t2=doc["T2"];
    double v1=doc["V1"];
    double v2=doc["V2"];
    
    //Serial.print(F("Sending: "));
    obj.printTo(jlineSend);
  
  Serial.println();
    if (!f) {
      Serial.println("Count file open failed on update.");
    } else { 
      f.println(jlineSend);
      f.close();
      Serial.println("Adding one");
    }
      f.println(0); 
      f.close();
       double adcVol = 3.33/256 ; 
    double reading = analogRead(A0);
    double sensor1= adcVol * reading;
    double sensor2= 2* sensor1;
    double volt1= 3* sensor1;
    double volt2= 4* sensor1;
    double cur1= 5* sensor1;
    double cur2= 6* sensor1;
     String body = "field1=";
     body += String(sensor1);
     body += "&field2=";// for multiple fields
     body += String(sensor2);
     body += "&field3=";// for multiple fields
     body += String(volt1);
     body += "&field4=";// for multiple fields
     body += String(volt2);
     body += "&field5=";// for multiple fields
     body += String(cur1);
     body += "&field6=";// for multiple fields
     body += String(cur2);
     //body += "&field7=";// for multiple fields
     //body += String(volt3);
     //body += "&field8=";// for multiple fields
     //body += String(cur3);
     //body += "&field9=";// for multiple fields
     //body += String(volt4);
     //body += "&field10=";// for multiple fields
     //body += String(cur4);
/**/
   
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
    }
  delay(11000);
}
