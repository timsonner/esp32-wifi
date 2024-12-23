#include "WiFi.h"
#include "esp_wifi.h"
#include "FS.h"
#include "SD.h"
#include <map>
#include <string>
#include <set>

// Set to store unique MAC addresses
std::set<std::string> uniqueMacs;

// Map to store OUI vendor information
std::map<std::string, std::string> ouiMap;

// Function to list files on the SD card
void listFiles() {
    File root = SD.open("/");
    if (!root) {
        Serial.println("Failed to open directory.");
        return;
    }

    while (true) {
        File entry = root.openNextFile();
        if (!entry) {
            break; // No more files
        }
        String fileName = entry.name();
        // Print only manuf* files for clarity
        if (fileName.startsWith("manuf")) {
            Serial.println("Found OUI file: " + fileName);
        }
        entry.close();
    }
}

bool loadOUIData(const char *files[], int numFiles, const uint8_t *mac, String &vendor) {
    // Iterate through each file one by one
    for (int i = 0; i < numFiles; ++i) {
        Serial.printf("Opening file: %s\n", files[i]);
        File file = SD.open(files[i], FILE_READ);
        if (!file) {
            Serial.printf("Failed to open file: %s\n", files[i]);
            continue; // Skip if file cannot be opened
        }

        // Read each line in the current file
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0) continue; // Skip empty lines

            // Feed the watchdog
            yield();  // Allow time for other tasks
            delay(1); // Add small delay for the watchdog timer

            int spaceIndex = line.indexOf(' ');
            if (spaceIndex > 0) {
                String oui = line.substring(0, spaceIndex);
                String vendorName = line.substring(spaceIndex + 1);

                // Check if the OUI from the line matches the first 3 bytes of the MAC address
                char ouiStr[9]; // "XX:XX:XX\0"
                snprintf(ouiStr, sizeof(ouiStr), "%02X:%02X:%02X", mac[0], mac[1], mac[2]);

                if (oui == ouiStr) {
                    vendor = vendorName;  // If a match is found, return the vendor
                    
                    file.close();  // Close the file once the match is found
                    return true;
                }
            }
        }

        file.close(); // Close the current file if no match was found
    }

    // If no match was found in any file
    return false;
}

void promiscuousCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
    wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *)buf;
    const uint8_t *dst_mac = packet->payload + 4; // Destination MAC (4th byte for 802.11 frame)

    char macStr[18]; // "XX:XX:XX:XX:XX:XX\0"
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             dst_mac[0], dst_mac[1], dst_mac[2],
             dst_mac[3], dst_mac[4], dst_mac[5]);

    // Add MAC address to the set if not already present
    if (uniqueMacs.find(macStr) == uniqueMacs.end()) {
        uniqueMacs.insert(macStr);

        // Use OUI matching function to check the destination MAC
        String vendor = "Unknown";
        
        // Define the file paths as an array
        const char* files[] = {"/manuf_partaa", "/manuf_partab", "/manuf_partac", "/manuf_partad", "/manuf_partae", "/manuf_partaf", "/manuf_partag", "/manuf_partah", "/manuf_partai", "/manuf_partaj"};
        int numFiles = sizeof(files) / sizeof(files[0]);

        bool matchFound = false;
        
        // Check each file
        for (int i = 0; i < numFiles; ++i) {
            if (loadOUIData(&files[i], 1, dst_mac, vendor)) {
                Serial.printf("MAC: %s - File: %s - Vendor: %s\n", macStr, files[i], vendor.c_str());
                matchFound = true;
                break; // Exit loop as soon as a match is found
            } else {
                // Optional: Print the file being checked even if no match is found
                // Serial.printf("Checking file: %s\n", files[i]);
            }
        }

        if (!matchFound) {
            Serial.printf("MAC: %s - Vendor: Unknown\n", macStr);
        }
    }
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
    // Initialize SD card
    if (!SD.begin()) {
        Serial.println("[-] SD card initialization failed!");
        return;
    }
    Serial.println("[+] SD card initialized.");

    // List files on the SD card to verify contents
    listFiles();
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay in milliseconds, allows watchdog reset

    delay(1000);
}
