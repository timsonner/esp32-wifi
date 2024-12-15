#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000); // Give time for serial monitor to connect

  Serial.println("Starting WiFi scanner...");

  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from any current connection
  delay(100);
}

void loop() {
  Serial.println("Scanning for networks...");

  // Perform a network scan
  int numNetworks = WiFi.scanNetworks(false, true); // false is passive mode, 2nd param is whether to scan for 'hidden' networks

  if (numNetworks == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.printf("Found %d networks:\n", numNetworks);
    Serial.println("-----------------------------------------------------");
    Serial.println("SSID            BSSID              RSSI  Encryption");
    Serial.println("-----------------------------------------------------");

    for (int i = 0; i < numNetworks; i++) {
      // Get network details
      String ssid = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);
      wifi_auth_mode_t encryptionType = WiFi.encryptionType(i);

      // Convert BSSID (MAC address) to string
      uint8_t *bssid = WiFi.BSSID(i);
      char bssidStr[18];
      sprintf(bssidStr, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

      // If the SSID is empty, it's a hidden network
      if (ssid == "") {
        ssid = "Hidden Network";
      }

      // Print network details
      Serial.printf("%-15s %-17s %-5d %s\n", 
                    ssid.c_str(), 
                    bssidStr, 
                    rssi, 
                    encryptionTypeToString(encryptionType));
    }
  }

  Serial.println("-----------------------------------------------------");
  delay(10000); // Wait 10 seconds before scanning again
}

// Helper function to convert encryption type to a readable string
const char* encryptionTypeToString(wifi_auth_mode_t type) {
  switch (type) {
    case WIFI_AUTH_OPEN: return "Open";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA/PSK";
    case WIFI_AUTH_WPA2_PSK: return "WPA2/PSK";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2/PSK";
    case WIFI_AUTH_WPA3_PSK: return "WPA3/PSK";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2/Enterprise";
    default: return "Unknown";
  }
}
