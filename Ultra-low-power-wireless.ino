// Library
#include <ESP8266WiFi.h>
#include <DHT.h>
// WiFi settings

String apiKey = "50BLPEPFF1942BRQ";
const char ssid[] = "Write your wifi ssid here (wifi name)";
const char password[] = "Write your password here";
// Time to sleep (in seconds):
const int sleepTimeS = (120000000 * 30) ;//1hr
// Host
const char* host = "dweet.io";

const char* server = "api.thingspeak.com";
#define DHTPIN 5 // what pin we’re connected to for the sensor

DHT dht(DHTPIN, DHT11, 15);
WiFiClient client;

void setup() 
{
  // Serial -- You can check what it will print out on the display.
  Serial.begin(115200);
  Serial.println("ESP8266 Booting");
  
  // Connect to WiFi
WiFi.begin(ssid, password);

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi working");


float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Temp sensor cant be read");
    return;
  }

  if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius Humidity: ");
    Serial.print(h);
    Serial.println("% send to Thingspeak");
  }
  client.stop();


  Serial.println("ESP8266 going to deep sleep mode");
  ESP.deepSleep(sleepTimeS);
  
}
void loop() 
{

  
}
