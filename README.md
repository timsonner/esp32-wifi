# Testing repo for m5stack cardputer (ESP32-S3)  

# Arduino IDE setup notes for Linux  
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

