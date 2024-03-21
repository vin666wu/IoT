#include "ESP8266WiFi.h"
#include "WiFiClientSecure.h"

void wifiConnect(const char* SSID,const char* PASSWORD);
void sendMessage(const char* token, const char* message);

const char* SSID = "";
const char* PASSWORD = "";
String Linetoken = "";

void setup() {
  Serial.begin(115200);
  wifiConnect(SSID, PASSWORD);
}

void loop() {
  int number = 55688;
  String MESSAGE = "test string"+'\n'+String(number);
  sendMessage(Linetoken, MESSAGE);
  delay(5000);
}

void wifiConnect(const char* SSID,const char* PASSWORD){
  WiFi.mode(WIFI_STA);  //set nodemcu as station(client) mode
  WiFi.disconnect();
  delay(100);

    // Attempt to connect to Wifi network:
    Serial.print("Connecting WiFi: ");
    Serial.println("SSID");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status()!=WL_CONNECTED){
      Serial.print("NodeMCU is trying connecting to WiFi, still attempt connecting...\n");
      delay(500);
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
}

void sendMessage(String token, String message){

  WiFiClientSecure client;
  String host = "notify-api.line.me";

  if(client.connect(host,443)){   //port 443: HTTPS connection
    Serial.print("Host : "); Serial.println(host); Serial.println();
    Serial.print("message="); Serial.println(message);
    int LEN = message.length();
    Serial.print("message.length()="); Serial.println(LEN);

         /* Post url */
    String url = "/api/notify";
    client.println("POST "+url+" HTTP/1.1");

          /* Headers */
    client.print("HOST: "); client.println(host); /* Header (Host) */
    client.println("Connection: close");
    client.println("User-Agent: ESP8266/1.0");    //optional
    client.print("Authorization: Bearer "); client.println(token);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: "); client.println( String((LEN+8)) );
    client.println(); //end of the headers

         /* Body */    
    client.print("message="); client.println(message);
    client.println();

    delay(50);
    String response = "";
    response = client.readString();
    Serial.println(response);
  }
  else{
    Serial.println("connected fail");
    Serial.println(client.status());
  }     
}
