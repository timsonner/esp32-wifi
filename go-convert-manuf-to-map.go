package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func main() {
	// Ensure a file path is provided
	if len(os.Args) < 2 {
		fmt.Println("Usage: go run main.go <file-path>")
		return
	}

	filePath := os.Args[1]

	// Open the specified file
	file, err := os.Open(filePath)
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		return
	}
	defer file.Close()

	// Initialize a scanner to read the file line by line
	scanner := bufio.NewScanner(file)
	var result []string

	for scanner.Scan() {
		// Read each line
		line := scanner.Text()

		// Remove comments (anything after '#') and trim spaces
		if idx := strings.Index(line, "#"); idx != -1 {
			line = line[:idx]
		}
		line = strings.TrimSpace(line)

		// Skip empty lines
		if line == "" {
			continue
		}

		// Split the line into fields (OUI and manufacturer)
		fields := strings.Fields(line)
		if len(fields) < 2 {
			continue
		}

		// The OUI is the first field, and the manufacturer is the rest
		oui := fields[0]
		manufacturer := strings.Join(fields[1:], " ")

		// Format the output
		result = append(result, fmt.Sprintf("{\"%s\", \"%s\"},", oui, manufacturer))
	}

	// Check for errors while reading the file
	if err := scanner.Err(); err != nil {
		fmt.Printf("Error reading file: %v\n", err)
		return
	}

	// Print the result
	fmt.Println("Output:")
	for _, line := range result {
		fmt.Println(line)
	}
}
