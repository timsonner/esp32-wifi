#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char *ssid = "ESP32_Hotspot";
const char *password = "123456789";

WebServer server(80);  // Web server listening on port 80
DNSServer dnsServer;   // DNS server for specific domain redirection

const IPAddress apIP(192, 168, 4, 1);  // IP of the ESP32 Access Point

// Store chat messages in an array (limited size for simplicity)
String chatMessages[10];  
int messageIndex = 0;

// Define custom landing page for zzz.com with a chat input and chat history
const char* landingPageZzz = R"(
<html>
  <head>
  </head>
  <body>
    <h1>Welcome to ZZZ.com!</h1>
    <h2>Chat:</h2>
    <div id="chatBox">
      <!-- Chat messages will be displayed here -->
      %CHAT_HISTORY%
    </div>
    <form action="/submit" method="POST">
      <label for="chatInput">Say something:</label>
      <input type="text" id="chatInput" name="chatInput" required>
      <input type="submit" value="Submit">
    </form>
  </body>
</html>
)";

// Set up DNS to redirect only specific domains to the ESP32 IP address
void setupDNS() {
  dnsServer.start(53, "*", apIP);  // Start DNS server on port 53, redirect all queries to ESP32's IP
}

// Send chat messages as HTML
String getChatHistory() {
  String chatHistory = "";
  
  // Generate the chat history from the stored messages
  for (int i = 0; i < messageIndex; i++) {
    chatHistory += "<p>" + chatMessages[i] + "</p>";
  }

  return chatHistory;
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

  // Set up HTTP server to serve the landing page for zzz.com
  server.on("/", HTTP_GET, [](){
    String host = server.hostHeader();  // Get the 'Host' header from the request
    
    if (host == "zzz.com") {
      String chatHistory = getChatHistory();
      
      // Replace the placeholder %CHAT_HISTORY% with the actual chat history
      String page = landingPageZzz;
      page.replace("%CHAT_HISTORY%", chatHistory);
      
      server.send(200, "text/html", page);
    } else {
      server.send(200, "text/html", "<html><body><h1>Welcome to the ESP32 Hotspot!</h1></body></html>");
    }
  });

  // Handle form submission (POST request)
  server.on("/submit", HTTP_POST, [](){
    if (server.hasArg("chatInput")) {
      String chatInput = server.arg("chatInput");  // Get the user input from the form
      Serial.println("User input: " + chatInput);  // Print the input to the serial monitor

      // Store the message in the chatMessages array
      if (messageIndex < 10) {  // Limit to 10 messages for simplicity
        chatMessages[messageIndex] = chatInput;
        messageIndex++;
      } else {
        // If the array is full, shift the messages up and add the new one
        for (int i = 1; i < 10; i++) {
          chatMessages[i-1] = chatMessages[i];
        }
        chatMessages[9] = chatInput;
      }

      // Send a redirect response to refresh the page
      server.sendHeader("Location", "/");
      server.send(303);  // HTTP status code 303 for redirect
    } else {
      server.send(400, "text/html", "<html><body><h1>Error: No input received.</h1></body></html>");
    }
  });

  // Start the HTTP server
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();  // Handle DNS queries
  server.handleClient();           // Handle HTTP requests
}
