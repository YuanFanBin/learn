package main

import "fmt"

func main() {
	fmt.Println("Starting the program")
	panic("A servere error occurred: stopping the program!")
	fmt.Println("Ending the program")
}
