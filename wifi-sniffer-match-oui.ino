#include "WiFi.h"
#include "esp_wifi.h"
#include <map>
#include <string>

// OUI Vendor map
std::map<std::string, std::string> ouiMap = {
{
{"FF:FF:FF", "ETHERNET BROADCAST"},
{"04:BC:9F", "Calix Inc."},
{"14:7D:05", "Sercomm Philippines Inc"},
{"A8:9A:93", "Sagemcom Broadband SAS"},
{"D0:21:F9", "Ubiquiti Inc"},
{"70:2C:1F", "Wisol"},
{"34:98:7A", "Espressif Inc."},
{"7C:87:CE", "Espressif Inc."},
{"1C:69:20", "Espressif Inc."},
{"C4:1C:FF", "Vizio, Inc"},
{"14:13:0B", "Garmin International"},
{"2C:DC:D7", "AzureWave Technology Inc."},
{"08:37:3D", "Samsung Electronics Co.,Ltd"},
{"B8:BB:AF", "Samsung Electronics Co.,Ltd"},
{"F0:D4:15", "Intel Corporate"},
{"9C:EF:D5", "Panda Wireless, Inc."},
{"FC:84:A7", "Murata Manufacturing Co., Ltd."},
{"60:AB:14", "LG Innotek"},
{"C8:94:02", "Chongqing Fugui Electronics Co.,Ltd."},
{"64:DB:A0", "Select Comfort"},
{"5C:EA:1D", "Hon Hai Precision Ind. Co.,Ltd."},
{"58:9B:4A", "DWnet Technologies(Suzhou) Corporation"},
{"50:DC:E7", "Amazon Technologies Inc."},
{"B4:B7:42", "Amazon Technologies Inc."},
{"FC:84:A7", "Murata Manufacturing Co., Ltd."},
{"3C:EF:42", "TCT mobile ltd"}
}};

// Function to get the vendor name from MAC address
std::string getVendor(const uint8_t *mac) {
    char macStr[9]; // "XX:XX:XX\0"
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X", mac[0], mac[1], mac[2]);
    auto it = ouiMap.find(macStr);
    return (it != ouiMap.end()) ? it->second : "Unknown Vendor";
}

void promiscuousCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
    wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *src_mac = packet->payload + 10; // Source MAC
    const uint8_t *dst_mac = packet->payload + 4;  // Destination MAC

    // Debug: print the raw MAC address in the format XX:XX:XX
    char macStr[18]; // "XX:XX:XX:XX:XX:XX\0"
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X", src_mac[0], src_mac[1], src_mac[2]);
    // Serial.printf("Source MAC: %s\n", macStr);

    std::string src_vendor = getVendor(src_mac);
    std::string dst_vendor = getVendor(dst_mac);

    Serial.printf("Packet: Source MAC: %02X:%02X:%02X:%02X:%02X:%02X (%s) -> Dest MAC: %02X:%02X:%02X:%02X:%02X:%02X (%s)\n",
                  src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5], src_vendor.c_str(),
                  dst_mac[0], dst_mac[1], dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5], dst_vendor.c_str());

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

    Serial.printf("RSSI: %d\n", packet->rx_ctrl.rssi);
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_MODE_APSTA);
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(promiscuousCallback);
    wifi_promiscuous_filter_t filter = {.filter_mask = WIFI_PROMIS_FILTER_MASK_ALL};
    esp_wifi_set_promiscuous_filter(&filter);
    Serial.println("[+] Promiscuous mode test started. Listening on channel 1...");
}

void loop() {
    delay(1000);
}
