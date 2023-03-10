void setup() {
    Serial.begin(115200);
  // put your setup code here, to run once:
    String readLine[5];
    String jlineSend;
    String readWord = "";
    int counter = 0;
    String Line;
    String readText="{\"sensor1\":\"10\",\"sensor2\":\"20\",\"voltage\":\"40\",\"current\":\"50\",\"sent\":\"no\"}";//for testing
    byte index=0;
    Serial.println();
    for (int line = 0; line <= readText.length(); line++) {
          Line=readText[line];
          for( int charac=0; charac<= Line.length();charac++){
          char c = Line[charac];
          
          if(c=='{')
          {
          continue;
          }
          else if (c == '"')
          {
            counter++;
            continue;
          }
          else if (counter==2)
          {
            
            //Serial.println("Number  is"+counter);
            readLine[index]=(String)readWord;//losing the memory over here, its not remembering the number i guess
            //Serial.println("current word is"+readWord);
            readWord="";
            counter=0;
            index++;
          }
          else if(c==':'){
            continue;
          }
          else if(c==','){
            continue;
          }
          else{
            readWord = readWord + c;
            //Serial.println(readWord);
          }
                 
          
          
          }
          
     }
     //Serial.println(readLine[0]);
     
     for (int i=0;i<=4;i++)
     {
      Serial.println(readLine[i]);
     }
}
        

void loop() {
  // put your main code here, to run repeatedly:

}
