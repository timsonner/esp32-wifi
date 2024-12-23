#include <FS.h>
#include <SD.h>
#include <map>

// Define a map to store MAC address prefixes and vendor names
std::map<String, String> macVendorMap;

// Helper function to sanitize strings (remove unwanted characters)
String sanitizeString(String input) {
  input.trim(); // Remove leading/trailing whitespace
  input.replace("{", "");
  input.replace("}", "");
  input.replace("\"", "");
  return input;
}

// Function to load the data from the file into the map
void loadMacVendorData() {
  // Open the file on the SD card
  File file = SD.open("/manuf_partaa");

  // Check if the file was successfully opened
  if (!file) {
    Serial.println("Failed to open the file.");
    return;
  }

  Serial.println("Loading MAC prefix and vendor data...");

  // Loop through each line in the file and populate the map
  while (file.available()) {
    String line = file.readStringUntil('\n');
    
    // Print the current line being processed
    Serial.println("Processing line: " + line);

    // Split the line by the comma (assuming the format is {"mac", "vendor"})
    int commaIndex = line.indexOf(',');
    if (commaIndex == -1) {
      Serial.println("Malformed line skipped.");
      continue; // Skip malformed lines
    }

    // Extract the MAC address prefix and the vendor
    String macPrefix = line.substring(0, commaIndex);
    String vendor = line.substring(commaIndex + 1);

    // Sanitize the extracted parts
    macPrefix = sanitizeString(macPrefix);
    vendor = sanitizeString(vendor);

    // Add to the map
    macVendorMap[macPrefix] = vendor;

    // Print what was added to the map
    Serial.println("Added: [" + macPrefix + "] -> [" + vendor + "]");
  }

  file.close(); // Close the file
  Serial.println("Data loading complete.");
}

// Function to find vendor by MAC prefix
String findVendorByMAC(String macPrefix) {
  // Print the prefix being searched for
  Serial.println("Searching for MAC prefix: " + macPrefix);

  // Check if the prefix is in the map
  if (macVendorMap.find(macPrefix) != macVendorMap.end()) {
    Serial.println("Match found: " + macPrefix + " -> " + macVendorMap[macPrefix]);
    return macVendorMap[macPrefix]; // Return the vendor name if found
  }

  Serial.println("No match found for: " + macPrefix);
  return ""; // Return an empty string if not found
}

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Initialize SD card
  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Load MAC address and vendor data into the map
  loadMacVendorData();

  // MAC address to search for (only the first 3 octets)
  String mac = "00:09:DB:0B:AB:AC";
  String macPrefix = mac.substring(0, 8); // Extract the first 3 octets (00:09:CB)

  // Call the function to find the vendor by MAC prefix
  String vendor = findVendorByMAC(macPrefix);
  
  // Output the result
  if (vendor != "") {
    Serial.println("Vendor found: " + vendor);
  } else {
    Serial.println("Vendor not found for the given MAC address.");
  }
}

void loop() {

}
