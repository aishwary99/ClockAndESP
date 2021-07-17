#include<Wire.h>
#include<DS3231.h>
#include<SoftwareSerial.h>
RTClib realTimeClock;
DS3231 clock;
SoftwareSerial wifi(6,7);
char requestBuffer[201];
char response[201];
char x;
int i;
void setup() {
  // put your setup code here, to run once:
  delay(5000);
  pinMode(13,OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Setting up wifi...");
  wifi.begin(9600);
  while(!wifi);
  Serial.println("Wifi module initialized...");
  Serial.println("Wifi testing...");
  wifi.print("AT\r\n");
  delay(2000);
  while(wifi.available()) Serial.print((char)wifi.read());
  Serial.println("Testing complete...");
  Serial.println("Putting wifi module to station mode...");
  wifi.print("AT+CWMODE=1\r\n");
  delay(2000);
  while(wifi.available()) Serial.print((char)wifi.read());
  Serial.println("Wifi module is in station mode...");
  delay(1000);
  Serial.println("Disconnecting from AP");
  wifi.print("AT+CWQAP\r\n");
  delay(2000);
  while(wifi.available()) Serial.print((char)wifi.read());
  Serial.println("Now wifi is not connected to any AP...");
  Serial.println("Connecting to TMIOTLAB");
  wifi.print("AT+CWJAP_CUR=\"Aishu\",\"aishwary\"\r\n");
  x=1;
  while(x<=5)
  {
    delay(2000);
    while(wifi.available()) Serial.print((char)wifi.read());
    x++;  
  }
  Serial.println("Connected to TMIOTLAB");
  Serial.println("Connecting to IOTServer");
  wifi.print("AT+CIPSTART=\"TCP\",\"192.168.43.22\",5050\r\n");
  delay(5000);
  while(wifi.available()) Serial.print((char)wifi.read());
  Serial.println("Connected to IOTServer...");
  Serial.println("Sending clock module data to IOTServer....");
  DateTime today=realTimeClock.now();
  String day(today.day());
  String month(today.month());
  String year(today.year());
  String hour(today.hour());
  String minute(today.minute());
  String second(today.second());
  String request=day+","+month+","+year+","+hour+","+minute+","+second+"#";
  int requestLength=request.length();
  Serial.print("Request  -> ");
  Serial.println(request);
  Serial.print("Request Length -> ");
  Serial.println(requestLength);
  sprintf(requestBuffer,"AT+CIPSEND=%d\r\n",requestLength);
  wifi.print(requestBuffer);
  delay(2000);
  while(wifi.available()) Serial.print((char)wifi.read());
  wifi.print(request);
  Serial.println("Asking for response...");
  delay(3000);
  if(!wifi.find((char *)"+IPD,"))
  {
    while(wifi.available()) wifi.read();
    return; 
  }
  while(wifi.available() && wifi.read()!=':');
  i=0;
  Serial.println("Reading response...");
  while(wifi.available() && i<200)
  {
    x=(char) wifi.read();
    if(x=='#') break;
    response[i]=x;
    i++;
  }
  response[i]='\0';
  Serial.print("Response arrived -> ");
  Serial.println(response);
  if(i<=2)
  {
    //All ok...
    for(int i=0;i<2;i++)
    {
      digitalWrite(13,HIGH);
      delay(2000);
      digitalWrite(13,LOW);
      delay(1000);
    }
  }
  else
  {
    int day=((response[0]-'0')*10)+(response[1]-'0');
    int month=((response[3]-'0')*10)+(response[4]-'0');
    int year=((response[6]-'0')*1000)+(response[7]-'0')+((response[8]-'0')*10)+(response[9]-'0');
    int hour=(response[11]-'0')*10+response[12];
    int minute=(response[14]-'0')*10+response[15];
    int second=(response[17]-'0')*10+response[18];
    clock.setClockMode(true);
    clock.setYear((byte)year);
    clock.setMonth((byte)month);
    clock.setDate((byte)day);
    clock.setHour((byte)hour);
    clock.setMinute((byte)minute);
    clock.setSecond((byte)second);
    Serial.println("Blinking LED for date time updation...");
    for(int i=0;i<4;i++)
    {
      digitalWrite(13,HIGH);
      delay(2000);
      digitalWrite(13,LOW);
      delay(1000);
    }
  }
  /*
  clock.setClockMode(false);
  clock.setYear((byte)20);
  clock.setMonth((byte)12);
  clock.setDate((byte)1);
  clock.setHour((byte)19);
  clock.setMinute((byte)57);
  clock.setSecond((byte)55);
  */
}

void loop() {
}