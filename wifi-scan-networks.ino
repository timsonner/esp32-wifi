// this sketch is based on: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiScan/WiFiScan.ino

#include "WiFi.h"

void setup() {
  Serial.begin(115200);

  // Set Wi-Fi to station mode 
  WiFi.mode(WIFI_STA);
  // Disconnect Wi-Fi interface from AP
  WiFi.disconnect();
  delay(100);
  Serial.println("");
  Serial.println("[+] Setup done");
}

void loop() {
  Serial.println("[+] Scan start");
  // REFERENCE: int16_t scanNetworks(bool async = false, bool show_hidden = false, bool passive = false, uint32_t max_ms_per_chan = 300, uint8_t channel = 0, const char * ssid=nullptr, const uint8_t * bssid=nullptr)
  int n = WiFi.scanNetworks(false, true); // First param is 'async' mode, second param is scan for 'hidden' networks. Returns number of networks found.
  Serial.println("[+] Scan end");
  if (n == 0) {
    Serial.println("[!] No networks found");
  } else {
    Serial.printf("[+] %2d networks found \n", n);
    Serial.println("--------------------------------------------------------------------------------------");
    Serial.println("#    | SSID                             | BSSID             | RSSI | CH   | Encryption");
    for (int i = 0; i < n; ++i) {
      // Print network number
      Serial.printf("%4d", i + 1);
      Serial.print(" | ");
      // Print SSID 
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      Serial.print(" | ");
      // Print BSSID
      Serial.print(WiFi.BSSIDstr(i));
      Serial.print(" | ");
      // Print RSSI
      Serial.printf("%4d", WiFi.RSSI(i));
      Serial.print(" | ");
      // Print Channel
      Serial.printf("%4d", WiFi.channel(i));
      Serial.print(" | ");
      // Print Encryption type
      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:            Serial.print("open"); break;
        case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
        default:                        Serial.print("unknown");
      }
      Serial.println();
      delay(10);
    }
  }
  Serial.println("--------------------------------------------------------------------------------------");
  Serial.println("");

  // Delete scan result to free memory
  WiFi.scanDelete();

  // Wait before re-scan
  delay(5000);
}
