  #include <iostream>
  #include "globals.h"

  #include <WiFi.h>
  #include <Wire.h>
  #include <Adafruit_BME280.h>
  #include <Adafruit_Sensor.h>

  // 77m npm = 1007.60 hpa
  // (11,3 hPa/100m)
  #define SEALEVELPRESSURE_HPA (1013.25) 

  Adafruit_BME280 bme; 

  WiFiServer server(80);

  String header;

  unsigned long currentTime = millis();
  unsigned long previousTime = 0; 
  const long timeoutTime = 2000;

  void setup() {
    Serial.begin(115200);
    bool status;

    if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }

    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("");
      Serial.print("Connecting to ssid: ");
      Serial.println(ssid);
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
  }

  void loop(){
    WiFiClient client = server.available();

    if (client) {                          
      currentTime = millis();
      previousTime = currentTime;
      Serial.println("New Client.");       
      String currentLine = "";                
      while (client.connected() && currentTime - previousTime <= timeoutTime) { 
        currentTime = millis();
        if (client.available()) {             
          char c = client.read();            
          Serial.write(c);                   
          header += c;
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE html><html><meta charset=\"utf-8\"><title>ESP32 - BME280</title>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<meta http-equiv=\"refresh\" content=\"2\">");  // Dodaj automatyczne odświeżanie
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
              client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
              client.println("th { padding: 12px; background-color: #0043af; color: white; }");
              client.println("tr { border: 1px solid #ddd; padding: 12px; }");
              client.println("tr:hover { background-color: #bcbcbc; }");
              client.println("td { border: none; padding: 12px; }");
              client.println(".sensor {font-weight: bold; padding: 1px; }");            
              client.println("</style></head><body><h1>ESP32 - BME280</h1>");
              client.println("<table><tr><th>Pomiar</th><th>Wartość</th></tr>");
              client.println("<tr><td>Temperatura</td><td><span class=\"sensor\">");
              client.println(bme.readTemperature());
              client.println(" *C</span></td></tr>");  
              client.println("<tr><td>Ciśnienie</td><td><span class=\"sensor\">");
              client.println(bme.readPressure() / 100.0F);
              client.println(" hPa</span></td></tr>");
              client.println("<tr><td>Przybliżona wysokość</td><td><span class=\"sensor\">");
              client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
              client.println(" m</span></td></tr>"); 
              client.println("<tr><td>Wilgotność</td><td><span class=\"sensor\">");
              client.println(bme.readHumidity());
              client.println(" %</span></td></tr>"); 
              client.println("</body></html>");            
              client.println();
              break;
            } else {
              currentLine = "";
            }
          } else if (c != '\r') { 
            currentLine += c;
          }
        }
      }
      header = "";
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }