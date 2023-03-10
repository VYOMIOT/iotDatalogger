void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  String body[] = {"sensor1","sensor1","sensor2","sensor2","voltage","volt1","current","cur1"};
  String cur= (String) findValue("current",body);
  Serial.print(cur);
}

void loop() {
  // put your main code here, to run repeatedly:

}
String findValue(String key, String line[]) {
  //Serial.println(line[]);
  for (int i = 0; i <= line->length(); i++)
  {
    //Serial.println(line[i]);
    if (line[i].equals(key))
    {
      return line[i + 1];
    }
  }
  return "none";
}
