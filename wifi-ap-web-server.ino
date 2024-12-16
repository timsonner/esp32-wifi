#include <WiFi.h>
#include <WebServer.h>  // Web server to serve the landing page

const char *ssid = "ESP32_Hotspot";  // Wi-Fi SSID
const char *password = "123456789";  // Wi-Fi passphrase

WebServer server(80);  // Create a web server that listens on port 80

// HTML content for the landing page
const char* landingPage = "<html><body><h1>Welcome to the ESP32 Hotspot!</h1></body></html>";

void setup() {
  Serial.begin(115200);

  // Start the Wi-Fi in AP mode
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());  // Print the IP of the AP

  // Set up the HTTP server to serve the landing page
  server.on("/", HTTP_GET, [](){
    server.send(200, "text/html", landingPage);  // Send the landing page
  });

  // Start the HTTP server
  server.begin();
}

void loop() {
  server.handleClient();  // Handle incoming client requests
}
