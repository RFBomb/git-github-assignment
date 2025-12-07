package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	reader := bufio.NewReader(os.Stdin)

	for {
		x := getInt(reader, "Enter the first integer to sum : ")
		y := getInt(reader, "Enter the second integer : ")

		fmt.Printf("%d + %d = %d\n\n", x, y, x+y)
	}
}

func getInt(reader *bufio.Reader, prompt string) int {
	for {
		// print prompt (no newline)
		fmt.Print(prompt)

		// read input
		input, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Error reading input. Try again.")
			continue
		}

		// trim and parse to integer
		value, err := strconv.Atoi(strings.TrimSpace(input))
		if err != nil {
			fmt.Println("Please enter a valid integer.")
			continue
		}

		return value
	}
}
