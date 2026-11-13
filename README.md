# esp32-wifi

Arduino sketches and one Go program for ESP32 Wi-Fi experiments. The sketches scan nearby access points, print frames heard in promiscuous mode, match MAC prefixes to vendor names, and serve a small page from a soft access point. The Go program turns a Wireshark `manuf` file into the map lines the SD card sketches read.

`wireshark-manuf-mapped.txt` is that converted list, already checked in (23285 lines).

## Prerequisites

- Arduino IDE and an ESP32 board package. The sketches include `WiFi.h`. Several also include `esp_wifi.h`, `WebServer.h`, `DNSServer.h`, `SD.h`, or `FS.h` from the Arduino-ESP32 core.
- For the M5Stack ESP32-S3 board index, add this URL under File, Preferences, Additional boards manager URLs:

```
https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
```

- An SD card on the board default SD pins for `match-oui-from-sd.ino` and `wifi-lookup-oui-from-sd.ino` (`SD.begin()` with no arguments).
- Go, only if you regenerate the vendor map. This repository has no `go.mod`. A single-file `go run` is enough. The commands below were run with Go 1.22.2.

## Install, build, run, and test

The repository ships the `.ino` sketches and `go-convert-manuf-to-map.go` only. Open one sketch in the Arduino IDE, select the ESP32 board and serial port, upload, and open the Serial Monitor at the baud rate in that file.

| Sketch | `Serial.begin` baud |
| --- | --- |
| `match-oui-from-sd.ino` | 115200 |
| `wifi-ap-web-server.ino` | 115200 |
| `wifi-ap-web-server-chat.ino` | 115200 |
| `wifi-ap-web-server-dns-redirect.ino` | 115200 |
| `wifi-scan-networks.ino` | 115200 |
| `wifi-sniffer.ino` | 115200 |
| `wifi-sniffer-match-oui.ino` | 115200 |
| `wifi-lookup-oui-from-sd.ino` | 921600 |
| `wifi-sniff-pmkid-to-hc22000.ino` | 921600 |

Regenerate map lines from a Wireshark `manuf` file. Matching lines are printed to stdout as `{"MAC", "description"},`. Empty lines and lines that start with `#` are ignored. Other unmatched lines are printed with the prefix `Skipping line:`.

```bash
go run go-convert-manuf-to-map.go -file path/to/manuf.txt
```

Save the map:

```bash
go run go-convert-manuf-to-map.go -file path/to/manuf.txt > wireshark-manuf-mapped.txt
```

Omit `-file` and the program prints `Please provide a file path using the -file flag` and exits 0.

`wifi-lookup-oui-from-sd.ino` opens these paths on the SD card: `/manuf_partaa`, `/manuf_partab`, `/manuf_partac`, `/manuf_partad`, `/manuf_partae`, `/manuf_partaf`, `/manuf_partag`, `/manuf_partah`, `/manuf_partai`, `/manuf_partaj`. `match-oui-from-sd.ino` opens only `/manuf_partaa`.

This `split` command on the checked-in map produces those ten filenames (`manuf_partaa` through `manuf_partaj`):

```bash
split -l 2500 wireshark-manuf-mapped.txt manuf_part
```

Copy those ten files to the root of the SD card.

## Usage

**Scan.** `wifi-scan-networks.ino` uses station mode, scans (the call passes `show_hidden` as true), and prints index, SSID, BSSID, RSSI, channel, and encryption. It deletes the scan and waits 5 seconds before scanning again. The file header cites the Arduino-ESP32 `WiFiScan` example.

**Soft access point.** `wifi-ap-web-server.ino`, `wifi-ap-web-server-chat.ino`, and `wifi-ap-web-server-dns-redirect.ino` start a soft AP. Join the network named by the `ssid` constant using the `password` constant in that file. Each sketch prints `IP Address:` from `WiFi.softAPIP()`. The HTTP server listens on port 80.

- `wifi-ap-web-server.ino` answers `GET /` with a short HTML welcome page.
- `wifi-ap-web-server-dns-redirect.ino` starts DNS on port 53 and answers queries with the AP address in the sketch. `GET /` picks the HTML from the request Host header. The host strings are constants in the file.
- `wifi-ap-web-server-chat.ino` uses the same DNS setup. For one Host value in the file, `GET /` serves a form and the stored lines. `POST /submit` reads the `chatInput` field, keeps up to 10 messages, and redirects to `/`. Any other host gets the short welcome page.

**SD vendor lookup.** `match-oui-from-sd.ino` loads `/manuf_partaa` and looks up the MAC prefix hardcoded in `setup()`. `wifi-lookup-oui-from-sd.ino` listens in promiscuous mode on channel 1, records unique destination MACs, and prints a line when one of the ten `manuf_part*` files contains that prefix.

**Promiscuous printout.** `wifi-sniffer.ino` listens on channel 1 and prints source and destination MAC, frame type (management, data, or control), RSSI, length, and the first 32 payload bytes. `wifi-sniffer-match-oui.ino` prints the same MAC pair and a vendor name from the map compiled into the sketch (first three octets).

**PMKID lines.** `wifi-sniff-pmkid-to-hc22000.ino` listens in promiscuous mode. For a management frame that contains a PMKID, it prints one hashcat 22000 line on the serial port.

## Layout

| Path | Role |
| --- | --- |
| `go-convert-manuf-to-map.go` | Reads a Wireshark `manuf` file (`-file`) and prints map entries. |
| `wireshark-manuf-mapped.txt` | Checked-in output of that conversion. |
| `match-oui-from-sd.ino` | Loads `/manuf_partaa` and looks up one hardcoded MAC prefix. |
| `wifi-lookup-oui-from-sd.ino` | Promiscuous capture on channel 1; vendor lookup from the ten SD files. |
| `wifi-scan-networks.ino` | Repeated access-point scan printed on serial. |
| `wifi-sniffer.ino` | Promiscuous capture on channel 1; MAC, type, RSSI, and a short payload. |
| `wifi-sniffer-match-oui.ino` | Same capture, plus a compiled-in OUI map. |
| `wifi-ap-web-server.ino` | Soft AP and a one-page HTTP server. |
| `wifi-ap-web-server-dns-redirect.ino` | Soft AP, DNS on port 53, host-based pages. |
| `wifi-ap-web-server-chat.ino` | Soft AP, DNS on port 53, and a 10-line chat form. |
| `wifi-sniff-pmkid-to-hc22000.ino` | Prints PMKID lines in hashcat 22000 form. |
| `README.md` | This file. |

## License

A license file is not included in this repository.
