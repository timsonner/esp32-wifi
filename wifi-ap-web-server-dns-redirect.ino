#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char *ssid = "ESP32_Hotspot";
const char *password = "123456789";

WebServer server(80);  // Create a web server that listens on port 80
DNSServer dnsServer;   // DNS server for specific domain redirection

const IPAddress apIP(192, 168, 4, 1);  // IP of the ESP32 Access Point

// Define custom landing pages for specific domains
const char* landingPageZzz = "<html><body><h1>Welcome to ZZZ.com!</h1></body></html>";
const char* landingPageExample = "<html><body><h1>Welcome to Example.com!</h1></body></html>";

// Set up DNS to redirect only specific domains to the ESP32 IP address
void setupDNS() {
  dnsServer.start(53, "*", apIP);  // Start DNS server on port 53, redirect all queries to ESP32's IP
}

void setup() {
  Serial.begin(115200);

  // Start Wi-Fi in AP mode
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());  // Print the IP of the AP

  // Start the DNS server
  setupDNS();

  // Set up HTTP server to serve different landing pages based on the domain
  server.on("/", HTTP_GET, [](){
    String host = server.hostHeader();  // Get the 'Host' header from the request
    
    // Serve different landing pages based on the domain
    if (host == "zzz.com") {
      server.send(200, "text/html", landingPageZzz);
    } else if (host == "example.com") {
      server.send(200, "text/html", landingPageExample);
    } else {
      server.send(200, "text/html", "<html><body><h1>Welcome to the ESP32 Hotspot!</h1></body></html>");
    }
  });

  // Start the HTTP server
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();  // Handle DNS queries
  server.handleClient();           // Handle HTTP requests
}
