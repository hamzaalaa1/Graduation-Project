
#include <DFRobot_SIM808.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
StaticJsonBuffer<200> jsonBuffer; 
SoftwareSerial myserial(10, 11); // RX: 10, TX:11
DFRobot_SIM808 sim808(&Serial);

 
#define DHTPIN A1
#define DHTTYPE DHT11
 
  
char t[32];
char deviceID[12] = "MYTEST56";
float Lat ; 
float Long ;
StaticJsonBuffer<200> jsonBuffer;
void setup()
{
  myserial.begin(9600);        // the GPRS baud rate
  Serial.begin(9600);
  Serial.println("Initializing..........");
  DynamicJsonBuffer jsonBuffer;
  delay(5000);
    //******** Initialize sim808 module *************
  while(!sim808.init()) { 
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }

  //************* Turn on the GPS power************
  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");
  
}
 
void loop()
{
  Serial.println("");
  Serial.println("************************************************************");
  
  
   
 
 
 /********************GSM Communication Starts********************/
 
  if (myserial.available())
  Serial.write(myserial.read());
 
  myserial.println("AT");
  delay(3000);
 
  myserial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");//APN
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=1,1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=2,1");
  delay(6000);
  ShowSerialData();
 
 
  myserial.println("AT+HTTPINIT");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPPARA=\"CID\",1");	
  delay(6000);
  ShowSerialData();
 
  //StaticJsonBuffer<200> jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  Lat  = sim808.GPSdata.lat ;
  Long = sim808.GPSdata.lon ;
  
  object.set("Lat",Lat);
  object.set("Long",Long);
 
  object.printTo(Serial);
  Serial.println(" ");  
  String sendtoserver;
  object.prettyPrintTo(sendtoserver);
  delay(4000);
 
  myserial.println("AT+HTTPPARA=\"URL\",\"ww.test.com\""); //Server address
  delay(4000);
  ShowSerialData();
 
  myserial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(4000);
  ShowSerialData();
 
 
  myserial.println("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
  Serial.println(sendtoserver);
  delay(6000);
  ShowSerialData();
 
  myserial.println(sendtoserver);
  delay(6000);
  ShowSerialData;
 
  myserial.println("AT+HTTPACTION=1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPREAD");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPTERM");
  delay(10000);
  ShowSerialData;
 
  /********************GSM Communication Stops********************/
 
}
 
 
void ShowSerialData()
{
  while (myserial.available() != 0)
    Serial.write(myserial.read());
  delay(1000);
 
}
