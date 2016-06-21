package main

import "fmt"

func main() {
	items := [...]int{10, 20, 30, 40, 50}

	for _, item := range items {
		item *= 2
	}
	fmt.Println(items)
}
