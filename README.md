# Testing repo for m5stack cardputer (ESP32-S3)  


A brief description of your project, its purpose, and what it achieves.

## Table of Contents
1. [Overview](#overview)
2. [File Descriptions](#file-descriptions)
3. [Getting Started](#getting-started)
4. [Usage](#usage)
---

## Overview

Researching ESP-32 S3 devices and ESP-IDF library by Espressif. Learning about my M5 Stack Carputer and getting betterrer at C++. Using Arduino IDE... All 2.4 GHz stuff...

---

## File Descriptions

| **File/Folder**       | **Description**                                                                 |
|------------------------|---------------------------------------------------------------------------------|
| `go-convert-manuf-to-map.go`            | Converts the Wireshark manuf.txt file to a map.                                      |
| `wifi-ap-web-server-chat.ino`            | Creates an AP, serves page at 192.168.4.1 where clients can chat.                                      |
| `wifi-ap-web-server-dns-redirect.ino`                 | Creates an AP, serves page at zzz.com where clients can chat.                                          |
| `wifi-ap-web-server.ino`                | Creates an AP, serves page at 192.168.4.1 basic web server.                                      |
| `README.md`            | The README file you're currently reading.                                     |
| `wifi-scan-networks.ino`          | Scans APs, displays RSSI, BSSID, SSID, Encryption type, etc.                       |
| `wifi-sniffer-match-oui.ino.py`   | Captures packets, matching OUI of source and destination using a hard coded map.                                             |
| `wifi-sniffer.ino`   | Captures packets, displays first 32 bits of packet, source and destination MAC.                                             |
| `wireshark-manuf-mapped.txt`   | Wireshark manuf.txt OUI list converted to a map.                                             |

---

## Getting Started

Download Arduino IDE as zip  
https://www.arduino.cc/en/software  

Unzip file, cd into the directory...  
Fix chromesandbox perms (general fix for Electron apps)
```  
sudo chown root:root chrome-sandbox
sudo chmod 4755 chrome-sandbox
```  

Installe Python package for writing firmware to device via serial port    
```  
sudo apt install python3-serial
```  

Maybe this fixes the port not found error, maybe not...    
```  
sudo usermod -a -G dialout $USER
sudo apt remove modemmanager
```  
Reboot system

Install board management in Arduino IDE    
https://docs.m5stack.com/en/arduino/arduino_board  

File > Preferences  
```  
https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
```  

---

## Usage

Get an ESP-32 device, flash a .ino file to it with Arduino IDE.

---


