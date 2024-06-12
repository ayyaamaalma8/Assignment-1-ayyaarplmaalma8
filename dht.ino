#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// WiFi Configuration
const char* ssid = "HUAWEI-AD08";
const char* password = "90311938";

// Sensor configuration
const int sensorPin = 4;
const String sensorName = "suhu";

// RestAPI Configuration
const char* baseRoute = "http://192.168.8.100:5000/sensor/";
const int requestDelay = 1000;

DHT dht(sensorPin, DHT11);

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi network");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(requestDelay);
  int c = dht.readTemperature();
  int f = dht.readTemperature(true);

  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, baseRoute+sensorName);
    http.addHeader("Content-Type", "application/json");
    int statusCode = http.POST("{\"celcius\": "+String(c)+", \"fahrenheit\": "+String(f)+"}");

    http.end();
  }else{
    Serial.println("WiFi Disconnected");
  }
}