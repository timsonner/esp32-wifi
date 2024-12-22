package main

import (
	"bufio"
	"flag"
	"fmt"
	"os"
	"regexp"
	"strings"
)

// Struct to hold the MAC address and its description
type OUIEntry struct {
	MAC         string
	Description string
}

// Function to parse the content of the file
func parseFile(filename string) ([]OUIEntry, error) {
	// Open the file
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	// Slice to store the parsed OUI entries
	var ouientries []OUIEntry

	// Regular expression to match different MAC address formats (with colons, dashes, or with /XX)
	re := regexp.MustCompile(`([0-9A-Fa-f:-]+(?:/\d{2})?)\s+(.+)`)

	// Scanner to read the file line by line
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		// Trim any leading or trailing whitespace
		line = strings.TrimSpace(line)

		// Skip empty lines and comments
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}

		// Match the line with the regular expression
		match := re.FindStringSubmatch(line)
		if len(match) == 3 {
			// Replace dashes with colons in the MAC address
			mac := strings.Replace(match[1], "-", ":", -1)

			// Add the matched OUI entry to the slice
			ouientries = append(ouientries, OUIEntry{
				MAC:         mac,
				Description: match[2],
			})
		} else {
			// Handle lines that don't match the expected format (optional)
			fmt.Printf("Skipping line: %s\n", line)
		}
	}

	// Check for scanning errors
	if err := scanner.Err(); err != nil {
		return nil, err
	}

	return ouientries, nil
}

func main() {
	// Command-line argument to specify the input file
	filepath := flag.String("file", "", "Path to the file to parse")
	flag.Parse()

	// Ensure the filepath is provided
	if *filepath == "" {
		fmt.Println("Please provide a file path using the -file flag")
		return
	}

	// Parse the file
	ouientries, err := parseFile(*filepath)
	if err != nil {
		fmt.Println("Error reading the file:", err)
		return
	}

	// Output the parsed OUI data, preserving the order of the file
	for _, entry := range ouientries {
		fmt.Printf("{\"%s\", \"%s\"},\n", entry.MAC, entry.Description)
	}
}
