#include "WiFi.h"
#include "esp_wifi.h"

// Callback function for promiscuous mode
void promiscuousCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
  // Print raw packet details
  wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *)buf;
  uint8_t *payload = packet->payload;

  // Packet length
  uint16_t length = packet->rx_ctrl.sig_len;

   // Extract MAC addresses
    const uint8_t *src_mac = packet->payload + 10; // Source MAC
    const uint8_t *dst_mac = packet->payload + 4;  // Destination MAC

    Serial.printf("Packet: Source MAC: %02x:%02x:%02x:%02x:%02x:%02x -> Dest MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                  src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5],
                  dst_mac[0], dst_mac[1], dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5]);

  // Print the packet type
  switch (type) {
    case WIFI_PKT_MGMT:
      Serial.print("[MGMT] ");
      break;
    case WIFI_PKT_DATA:
      Serial.print("[DATA] ");
      break;
    case WIFI_PKT_CTRL:
      Serial.print("[CTRL] ");
      break;
    default:
      Serial.print("[UNKNOWN] ");
      break;
  }

  // Print the RSSI (signal strength)
  Serial.printf("RSSI: %d | Length: %d\n", packet->rx_ctrl.rssi, length);

  // Print raw packet payload (first 32 bytes for brevity)
  Serial.print("Payload: ");
  for (int i = 0; i < length && i < 32; i++) {
    Serial.printf("%02X ", payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_APSTA); // Disable Wi-Fi to allow promiscuous mode

  // Set the channel to listen on
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  // Enable promiscuous mode
  esp_wifi_set_promiscuous(true);

  // Set promiscuous mode callback function
  esp_wifi_set_promiscuous_rx_cb(promiscuousCallback);

  // Set filter to capture packet types
  wifi_promiscuous_filter_t filter = { .filter_mask = WIFI_PROMIS_FILTER_MASK_ALL };
  esp_wifi_set_promiscuous_filter(&filter);

  Serial.println("[+] Promiscuous mode test started. Listening on channel 1...");
}

void loop() {
  // Nothing in the loop; all work is done in the callback
  delay(1000);
}
