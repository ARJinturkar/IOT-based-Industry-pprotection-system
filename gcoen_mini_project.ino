#include <SoftwareSerial.h>
#include<LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimerOne.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
#define RX 2
#define TX 3
#include<LiquidCrystal_I2C.h>
int buzzer =4;
int threshold = 25;
int led = 5;
String AP = "Akash";       // AP NAME
String PASS = ""; // AP PASSWORD
String API = "G16FM62DEIS33GIR";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
String field2 = "field2";
String field3 = "field3";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int temp = 1;
int gas =1;
int fire = 1;
SoftwareSerial esp8266(RX,TX); 


  
void sendFire()
{
   
 
  if(digitalRead(8))
  {
      
  }
  else {
      digitalWrite(led,HIGH);
    delay(1000);
    digitalWrite(led,LOW);
    delay(1000);
  }
}
void sendtemp()
{
 
  // Serial.println(analogRead(A2)/2);
 
  if(analogRead(A0)>threshold)
  {
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(1000);
  }

}
void setup() {
    lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  esp8266.begin(115200);
  pinMode(4,OUTPUT);
pinMode(5,OUTPUT);


  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

int getTemperatureValue()
{return (analogRead(A2)/2);
}

int getGas()
{ return analogRead(A0);
}
 int getFire()
 {
  return digitalRead(8);
 }
void loop() {
 temp = getTemperatureValue();
 gas=getGas();
 fire=getFire();
Serial.print(gas);
   lcd.clear();
  
 
 // timer_mq2.run();
 
 lcd.setCursor(1,0);
 lcd.print("Temp:" );
 lcd.print(temp);
 //Serial.println(temp);
 delay(1000);
 lcd.setCursor(0,1);
 if(gas>threshold)
 {
  lcd. print("smoke detected");
  
 }
 else 
 {
  lcd.print("smoke not detected");
 }

 
 //Serial.println(gas);
 //delay(1000);
 if(fire){
  lcd.setCursor(1,0);
  lcd.print("fire");
   delay(1000);
 }
 else{
  lcd.setCursor(1,0);
  lcd.print("No fire");
   delay(1000);}
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(temp)+"&"+ field2 +"="+String(gas)+"&"+ field3 +"="+String(fire);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
//printlcd();
//sendtemp();

}

void printlcd()
{
   lcd.clear();
  
 
 // timer_mq2.run();
 
 lcd.setCursor(1,0);
 lcd.print("Temp:" );
 lcd.print(temp);
 //Serial.println(temp);
 delay(1000);
 lcd.setCursor(0,1);
 lcd.print("Gas:");
 lcd.print(gas);
 
 //Serial.println(gas);
 //delay(1000);
 if(fire){
  lcd.setCursor(1,0);
  lcd.print("fire");
 
   delay(1000);
 }
 else{
  lcd.setCursor(1,0);
  lcd.print(" No fire"); 
   delay(1000);}
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
