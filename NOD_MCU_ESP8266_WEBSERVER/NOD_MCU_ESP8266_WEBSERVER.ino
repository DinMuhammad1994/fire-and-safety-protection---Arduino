
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress    apIP(192, 168, 0, 100);  // Defining a static IP address: local & gateway
// Default IP in AP mode is 192.168.4.1

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "INDUSTRIAL CONTROL SYSTEM";
const char *password = "12345678";

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);

#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(D2, D1); // RX, TX

int value1;   // temp
int value2;   // humiidity
int value3;  // flame1
int value4;  // smoke1
int value5;  // flame2
int value6;   //smoke2
int value7;  // flame3
int value8;  // smoke3


String f1;
String s1;
String f2;
String s2;
String f3;
String s3;

String motor1;   //motor1
String fan1;  //fan1
String motor2;  // motot2
String fan2;  // fan2
String motor3;  // motor3
String fan3;  // flame3

String page = "";
double data;
void setup(void) {

  Serial.begin(9600);

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);   // Should be similar as mention in arduino code

  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", []() {
    page = "<blockquote> <h1 style=\"text-align: left;\"><span style=\"color: #993366;\">SECURITY AND PROTECTION INDUSTRIAL CONTROL SYSTEM</span></h1>"
           "<table style=\"width: 528px;\" border=\"1\"> <tbody> <tr> <td style=\"width: 250px;\"> <p>TEMPERATURE:</p> </td> <td style=\"width: 262px;\"> <p>" + String(value1) +
           "</p> </td> </tr> <tr> <td style=\"width: 250px;\"> <p>HUMIDITY:</p> </td> <td style=\"width: 262px;\"> <p>"  +  String(value2) + "</p> </td> </tr> </tbody> </table>"


           "<ul style=\"list-style-type: square;\"> <li> <h3 style=\"text-align: left;\"><strong><span style=\"color: #0000ff;\">&nbsp;<span style=\"color: #800080;\">ROOM 1</span></span></strong></h3> </li> </ul> <table style=\"height: 107px; width: 530px;\" border=\"1\"> <tbody> <tr> <td style=\"width: 248px;\"> <p>FLAME SENSOR:</p> </td> <td style=\"width: 266px;\"> <p> " + String(f1) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>SMOKE SENSOR:&nbsp;</p> </td> <td style=\"width: 266px;\"> <p>" + String(s1) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>FAN STATUS :</p> </td> <td style=\"width: 266px;\"> <p>" + String(fan1) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>MOTOR STATUS :</p> </td> <td style=\"width: 266px;\"> <p>" + String(motor1) + "</p> </td> </tr> </tbody> </table>"

           "<ul style=\"list-style-type: square;\"> <li> <h3 style=\"text-align: left;\"><strong><span style=\"color: #0000ff;\">&nbsp;<span style=\"color: #800080;\">ROOM 2</span></span></strong></h3> </li> </ul> <table style=\"height: 107px; width: 530px;\" border=\"1\"> <tbody> <tr> <td style=\"width: 248px;\"> <p>FLAME SENSOR:</p> </td> <td style=\"width: 266px;\"> <p> " + String(f2) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>SMOKE SENSOR:&nbsp;</p> </td> <td style=\"width: 266px;\"> <p>" + String(s2) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>FAN STATUS :</p> </td> <td style=\"width: 266px;\"> <p>" + String(fan2) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>MOTOR STATUS :</p> </td> <td style=\"width: 266px;\"> <p>" + String(motor2) + "</p> </td> </tr> </tbody> </table>"

           "<ul style=\"list-style-type: square;\"> <li> <h3 style=\"text-align: left;\"><strong><span style=\"color: #0000ff;\">&nbsp;<span style=\"color: #800080;\">ROOM 3</span></span></strong></h3> </li> </ul> <table style=\"height: 107px; width: 530px;\" border=\"1\"> <tbody> <tr> <td style=\"width: 248px;\"> <p>FLAME SENSOR:</p> </td> <td style=\"width: 266px;\"> <p> " + String(f3) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>SMOKE SENSOR:&nbsp;</p> </td> <td style=\"width: 266px;\"> <p>" + String(s3) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>FAN STATUS :</p> </td> <td style=\"width: 266px;\"> <p>" + String(fan3) +
           "</p> </td> </tr> <tr> <td style=\"width: 248px;\"> <p>MOTOR STATUS :</p> </td> <td style=\"width: 266px;\"> <p>" + String(motor3) + "</p> </td> </tr> </tbody> </table>"



           "<meta http-equiv=\"refresh\" content=\"10\" >";


    server.send(200, "text/html", page);
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop(void) {


  if (linkSerial.available())
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<500> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, linkSerial);

    if (err == DeserializationError::Ok)
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.print("value1 = ");
      Serial.println(doc["value1"].as<int>());
      Serial.print("value2 = ");
      Serial.println(doc["value2"].as<int>());
      Serial.print("value3 = ");
      Serial.println(doc["value3"].as<int>());
      Serial.print("value4 = ");
      Serial.println(doc["value4"].as<int>());
      Serial.print("value5 = ");
      Serial.println(doc["value5"].as<int>());
      Serial.print("value6 = ");
      Serial.println(doc["value6"].as<int>());
      Serial.print("value7 = ");
      Serial.println(doc["value7"].as<int>());
      Serial.print("value8 = ");
      Serial.println(doc["value8"].as<int>());

      value1 = (doc["value1"].as<int>());
      value2 = (doc["value2"].as<int>());
      value3 = (doc["value3"].as<int>());
      value4 = (doc["value4"].as<int>());
      value5 = (doc["value5"].as<int>());
      value6 = (doc["value6"].as<int>());
      value7 = (doc["value7"].as<int>());
      value8 = (doc["value8"].as<int>());



      if (value3 == 0)
      {
        f1 = "OK";
        motor1 = "OFF";
      }
      else
      {
        f1 = "ALERT";
        motor1 = "ON";
      }
      if (value4 == 0)
      {
        s1 = "OK";
        fan1 = "OFF";
      }
      else
      {
        s1 = "ALERT";
        fan1 = "ON";
      }


    


      if (value5 == 0)
      {
        f2 = "OK";
        motor2 = "OFF";
      }
      else
      {
        f2 = "ALERT";
        motor2 = "ON";
      }
      if (value6 == 0)
      {
        s2 = "OK";
        fan2 = "OFF";
      }
      else
      {
        s2 = "ALERT";
        fan2 = "ON";
      }



 


      if (value7 == 0)
      {
        f3 = "OK";
        motor3 = "OFF";
      }
      else
      {
        f3 = "ALERT";
        motor3 = "ON";
      }
      if (value8 == 0)
      {
        s3 = "OK";
        fan3 = "OFF";
      }
      else
      {
        s3 = "ALERT";
        fan3 = "ON";
      }

    }
    else
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "link" serial port buffer
      while (linkSerial.available() > 0)
        linkSerial.read();
    }
  }


  server.handleClient();
}
