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
#include "WiFiUdp.h"
#include "NTP.h"
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);
DynamicJsonDocument doc(512);
DynamicJsonDocument doc2(1024);
// Wi-Fi Settings
const char* ssid = "vivo-1901"; //your wireless network name (SSID)
const char* password = "alex7210";
WiFiClient client;
const int channelID = 2049316;
String writeAPIKey = "LOX7D0GNL4PI64FW"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 15000 ; // post data every 15 seconds
void setup()
{ 
  pinMode(A0, INPUT);
  pinMode(D, INPUT);
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(50);
  WiFiManager wifiManager;
//    wifiManager.setSaveConfigCallback(saveConfigCallback); add persistent to false
  ntp.ruleDST("IST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
  //ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60); // last sunday in october 3:00, timezone +60min (+1 GMT)
  ntp.begin();
  }
}
void loop()
{
    int count=0;
    ntp.update();
  //Serial.println(ntp.formattedTime("%d %B %Y %T")); // dd. Mmm yyyy
    String timestamp = ntp.formattedTime("%d %B %Y %A %T");
    if (client.connect(server, 80)) //Online mode
    {
      Serial.println("connected...yeey :)");
      //TODO:write to memory
      
      if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
      }
      File f = SPIFFS.open("doc", "w");
    /*
    doc["C1"]=20;
    doc["C2"]=30;
    doc["C3"]=40;
    doc["C4"]=10;
    doc["V1"]=90;
    doc["T1"]=80;
    doc["T2"]=70;*/
    //break; //if left in, we'll just read the first line then break out of the while.
    double adcVol = 3.33/256 ; 
    double reading = analogRead(A0);
    double sensor1= adcVol * reading;
    double sensor2= 2* sensor1;
    double volt1= 3* sensor1;
    double volt2= 4* sensor1;
    double cur1= 5* sensor1;
    double cur2= 6* sensor1;
      Serial.println("Reading is "+String(reading));
      doc[count]["C1"]=cur1;
      doc[count]["C2"]=cur2;
      doc[count]["T1"]=sensor1;
      doc[count]["T2"]=sensor2;
      doc[count]["V1"]=volt1;
      doc[count]["V2"]=volt2;
      doc[count]["timestamp"]=timestamp;
      doc[count]["status"]="sent";
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
     f.print(body);
     f.close();
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
    File f2 = SPIFFS.open("doc", "r");//read and send the values which were not sent by checking timestamp
      if (!f2) {
      Serial.println("Count file open failed on read.");
      } else {
      //while(f2.available()) {
        //Lets read line by line from the file
        for(int i=0;i<f2.size();i++)
          {
          //String line = f2.readStringUntil('\n');
          //Serial.prinln(line);
          Serial.print((char)f2.read());
          }
        
        //Serial.println(line);
    // Test if parsing succeeds.
    /*
    DeserializationError error = deserializeJson(doc2, f2);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      */
      double sensor1= doc["T1"];
      double sensor2= doc["T2"];
      double volt1= doc["V1"];
      double volt2= doc["V2"];
      double cur1= doc["C1"];
      double cur2= doc["C2"];
      serializeJson(doc,Serial);
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
    
  
  }
    }
  else //Offline mode
  {
    String jlineSend="xyz";
    if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
    File f = SPIFFS.open("doc", "w"); 
    
    //TODO:: Loop through json document(check for values which are not sent to server) and for every cycle convert values and store them
      Serial.println("Adding to file");
      //Serial.print(F("Sending: "));
    //json.printTo(jlineSend);
      double adcVol = 3.33/256 ; 
      double reading = analogRead(A0);
      double sensor1= adcVol * reading;
      double sensor2= 2* sensor1;
      double volt1= 3* sensor1;
      double volt2= 4* sensor1;
      double cur1= 5* sensor1;
      double cur2= 6* sensor1;
      //TODO::send data to cloud if its not sent, need to do the same on the server
      doc[count]["C1"]=cur1;
      doc[count]["C2"]=cur2;
      doc[count]["T1"]=sensor1;
      doc[count]["T2"]=sensor2;
      doc[count]["V1"]=volt1;
      doc[count]["V2"]=volt2;
      doc[count]["timestamp"]=timestamp;
      doc[count]["status"]="not sent";
      if (!f) {
      Serial.println("Count file open failed on update.");
        } else { 
          f.close();
        }
          //f.println(0); 
          f.close(); 
    }
  delay(11000);
  count++;
}
