void setup() {
    Serial.begin(115200);
  // put your setup code here, to run once:
    String readLine[5];
    String jlineSend;
    String readWord = "";
    int counter = 0;
    String Line;
    String readText="{\"sensor1\":\"10\",\"sensor2\":\"20\",\"voltage\":\"40\",\"current\":\"50\",\"sent\":\"no\"}";//for testing
    Serial.println();
    for (int line = 0; line <= readText.length(); line++) {
          Line=readText[line];
          for( int charac=0; charac<= readText.length();charac++){
          char c = Line[charac];
          
          if(c=='{')
          {
          ++charac;
          }
          
          if (c == '"')
          {
            counter++;
            continue;
          }
          else if (counter==2)
          {
            
            //Serial.println("Number  is"+counter);
            readLine[2]=readWord;
            Serial.println("current word is"+readWord);
            readWord="";
            counter=0;
          }
          else if(c==':'){
            continue;
          }
          else{
            readWord = readWord + c;
            //Serial.println(readWord);
          }
                 
          
          
          }
          
     }
     for (int i=0;i<=readLine->length();i++)
     {
      Serial.println("New Array:"+readLine[i]);
     }
}
        

void loop() {
  // put your main code here, to run repeatedly:

}
