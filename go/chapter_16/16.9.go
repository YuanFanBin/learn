package main

import (
	"fmt"
	"time"
)

var values = [5]int{10, 11, 12, 13, 14}

func main() {
	for ix := range values {
		func() {
			fmt.Print(ix, " ")
		}()
	}
	fmt.Println()

	for ix := range values {
		go func() {
			fmt.Print(ix, " ")
		}()
	}
	fmt.Println()
	time.Sleep(5e9)

	for ix := range values {
		go func(ix interface{}) {
			fmt.Print(ix, " ")
		}(ix)
	}
	fmt.Println()
	time.Sleep(5e9)

	for ix := range values {
		val := values[ix]
		go func() {
			fmt.Print(val, " ")
		}()
	}
	time.Sleep(1e9)
}
