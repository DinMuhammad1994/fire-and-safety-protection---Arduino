

// library import
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial linkSerial(5, 6); // RX, TX
// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include "DHT.h"

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// station1

int flame_1      = A7;
int smoke_1      = A6;
int flame_2      = A3;
int smoke_2      = A2;
int flame_3      = A1;
int smoke_3      = A0;

int f1, f2, f3;
int s1, s2, s3;

#define Motor1 2
#define Fan1   3    

#define Motor2 4
#define Fan2   7

#define Motor3 8
#define Fan3   9

#define buzz1  12
#define buzz2  10
#define buzz3  11


int h;
int t;



int value1;   // temp
int value2;   // humiidity
//---------------------station 1-------------
int value3;  // flame1
int value4;  // smoke1
int value5;  // flame2
int value6;   // smoke2
int value7;  // flame3
char value8;  //  smoke3


void setup ( )
{
  Serial.begin ( 9600 ) ;                                    // setting baud rate at 9600
  linkSerial.begin(4800);                                       // to send data from arduino nano to nod mcu esp9266 serially
  dht.begin();
  pinMode ( flame_1 , INPUT ) ;
  pinMode ( flame_2 , INPUT ) ;
  pinMode ( flame_3 , INPUT ) ;
  pinMode ( smoke_1 , INPUT ) ;
  pinMode ( smoke_2 , INPUT ) ;
  pinMode ( smoke_3 , INPUT ) ;
  pinMode (  Motor1, OUTPUT ) ;
  pinMode (  Motor2, OUTPUT ) ;
  pinMode (  Motor3, OUTPUT ) ;
  pinMode (  Fan1,   OUTPUT ) ;
  pinMode (  Fan2,   OUTPUT ) ;
  pinMode (  Fan3,   OUTPUT ) ;
  pinMode (  buzz1,   OUTPUT ) ;
  pinMode (  buzz2,   OUTPUT ) ;
  pinMode (  buzz3,   OUTPUT ) ;

  // active low relay, that are trun on when LOW, and Trun of When HIGH
  // trun of all relays
  digitalWrite (  Motor1, HIGH ) ;
  digitalWrite (  Motor2, HIGH ) ;
  digitalWrite (  Motor3, HIGH ) ;
  digitalWrite (  Fan1,   HIGH ) ;
  digitalWrite (  Fan2,   HIGH ) ;
  digitalWrite (  Fan3,   HIGH ) ;
  digitalWrite (  buzz1,   LOW ) ;
  digitalWrite (  buzz2,   LOW ) ;
  digitalWrite (  buzz3,   LOW ) ;

  delay(2000);

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  lcd.setCursor(0, 0);             // coulmn number 5, row number 0
  lcd.print("   SECURITY AND     ");
  lcd.setCursor(0, 1);
  lcd.print("    PROTECTION ");
  lcd.setCursor(0, 2);
  lcd.print("INDUSTRIAL CONTROL");
  lcd.setCursor(0, 3);
  lcd.print("     SYSTEM");
  
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("callibrating sensors ....");
  //  delay(3000);
  lcd.clear();


}


void loop ( )
{
  delay(1000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  delay(50);
  value1 = t;   //temperature value
  value2 = h;   //humidity value



  //station 1 sensors read
  s1 = analogRead(smoke_1);              // read value variable = analogread (pin number)
  s1 = map(s1, 0,1023, 0,100);
  delay(50);
  s2 = analogRead(smoke_2);
  s2 = map(s2, 0,1023, 0,100);
  delay(50);
  s3 = analogRead(smoke_3);
  s3 = map(s3, 0,1023, 0,100);
  delay(50);
  f1 = analogRead(flame_1);
  f1 = map(f1, 0,1023, 0,100);
  delay(50);
  f2 = analogRead(flame_2);
  f2 = map(f2, 0,1023, 0,100);
  delay(50);
  f3 = analogRead(flame_3);
  f3 = map(f3, 0,1023, 0,100);
  delay(50);
  //--------------------------------


/*
  Serial.print ("ROOM 1  :  ");
  Serial.print("Smoke Sensor: "); Serial.print(s1);
  Serial.print("  flame Sensor: "); Serial.print(f1);
  Serial.println(" ");

  Serial.print ("ROOM 2  :  ");
  Serial.print("Smoke Sensor: "); Serial.print(s2);
  Serial.print("  flame Sensor: "); Serial.print(f2);
  Serial.println(" ");


  
  Serial.print ("ROOM 3  :  ");
  Serial.print("Smoke Sensor: "); Serial.print(s3);
  Serial.print("  flame Sensor: "); Serial.print(f3);
  Serial.println(" ");


  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println(" ");
*/

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ROOM1  ROOM2  ROOM3");


  lcd.setCursor(0, 1);
  lcd.print("F : ");
  lcd.setCursor(7, 1);
  lcd.print("F : ");
  lcd.setCursor(14, 1);
  lcd.print("F : ");

  lcd.setCursor(0, 2);
  lcd.print("S : ");
  lcd.setCursor(7, 2);
  lcd.print("S : ");
  lcd.setCursor(14, 2);
  lcd.print("S : ");
  lcd.setCursor(0, 3);
  lcd.print("TEMP:");
  lcd.setCursor(5, 3);
  lcd.print(t);
  lcd.setCursor(8, 3);
  lcd.print("'C");
  lcd.setCursor(11, 3);
  lcd.print("HUMI:");
  lcd.setCursor(16, 3);
  lcd.print(h);
  lcd.setCursor(19, 3);
  lcd.print("%");

  sensorization();

  // sending all reading to nod mcu to upload on webserver
  // Print the values on the "debug" serial port
  Serial.print("value1 = ");    //TEMP
  Serial.println(value1);
  Serial.print("value2 = ");    //HIMIDITY
  Serial.println(value2);
  Serial.print("value3 = ");    //F1
  Serial.println(value3);
  Serial.print("value4 = ");    //S1
  Serial.println(value4);
  Serial.print("value5 = ");    //F2
  Serial.println(value5);
  Serial.print("value6 = ");    //S2
  Serial.println(value6);
  Serial.print("value7 = ");    //F3
  Serial.println(value7);
  Serial.print("value8 = ");    //S3
  Serial.println(value8);


  Serial.println("---");


  // Create the JSON document
  StaticJsonDocument<300> doc;
  doc["value1"] = value1;
  doc["value2"] = value2;
  doc["value3"] = value3;
  doc["value4"] = value4;
  doc["value5"] = value5;
  doc["value6"] = value6;
  doc["value7"] = value7;
  doc["value8"] = value8;


  // Send the JSON document over the "link" serial port
  serializeJson(doc, linkSerial);

  // Wait
  delay(3000);

}


void sensorization()
{

  //---------------ROOM1------------------
  if (f1 >  30)
  {
    lcd.setCursor(4, 1);
    lcd.print("OK");
    value3 = 0;
    digitalWrite(Motor1, HIGH ); //OFF
    digitalWrite(buzz1, LOW );   //OFF


  }
  else
  {
    lcd.setCursor(4, 1);
    lcd.print("XX");
    value3 = 1;
    digitalWrite(Motor1, LOW ); //OFF

    digitalWrite(buzz1, HIGH );   //ON
    delay(1000);
    digitalWrite(buzz1, LOW );   //ON


  }

  if (s1 <  40)
  {
    lcd.setCursor(4, 2);
    lcd.print("OK");
    value4 =  0;
    digitalWrite(Fan1, HIGH ); //OFF
    digitalWrite(buzz1, LOW );   //OFF

  }
  else
  {
    lcd.setCursor(4, 2);
    lcd.print("XX");
    value4 = 1;

    digitalWrite(Fan1, LOW ); //OFF
    digitalWrite(buzz1, HIGH );   //ON
    delay(1000);
    digitalWrite(buzz1, LOW );   //ON


  }

  //---------------ROOM2------------------
  if (f2 >  30)
  {
    lcd.setCursor(11, 1);
    lcd.print("OK");
    value5 =  0;
    digitalWrite(Motor2, HIGH ); //OFF
    digitalWrite(buzz2, LOW );   //OFF

  }
  else
  {
    lcd.setCursor(11, 1);
    lcd.print("XX");
    value5 = 1;
    digitalWrite(Motor2, LOW ); //OFF

    digitalWrite(buzz2, HIGH );   //ON
    delay(1000);
    digitalWrite(buzz2, LOW );   //ON


  }

  if (s2 <  40)
  {
    lcd.setCursor(11, 2);
    lcd.print("OK");
    value6 =  0;
    digitalWrite(Fan2, HIGH ); //OFF
    digitalWrite(buzz2, LOW );   //OFF


  }
  else
  {
    lcd.setCursor(11, 2);
    lcd.print("XX");
    value6 = 1;
    digitalWrite(Fan2, LOW ); //OFF
    digitalWrite(buzz2, HIGH );   //ON
    delay(1000);
    digitalWrite(buzz2, LOW );   //ON


  }


  //---------------ROOM3------------------
  if (f3 >  30)
  {
    lcd.setCursor(18, 1);
    lcd.print("OK");
    value7 =  0;
    digitalWrite(Motor3, HIGH ); //OFF
    digitalWrite(buzz3, LOW );   //OFF


  }
  else
  {
    lcd.setCursor(18, 1);
    lcd.print("XX");
    value7 = 1;
    digitalWrite(Motor3, LOW ); //OFF

    digitalWrite(buzz3, HIGH );   //ON
    delay(1000);
    digitalWrite(buzz3, LOW );   //ON


  }

  if (s3 <  40)
  {
    lcd.setCursor(18, 2);
    lcd.print("OK");
    value8 =  0;
    digitalWrite(Fan3, HIGH ); //OFF
    digitalWrite(buzz2, LOW );   //OFF

  }
  else
  {
    lcd.setCursor(18, 2);
    lcd.print("XX");
    value8 = 1;
    digitalWrite(Fan3, LOW ); //OFF
    digitalWrite(buzz3, HIGH );   //ON
    delay(1000);
    digitalWrite(buzz3, LOW );   //ON

  }

  delay(1000);
}
