package main

import (
	"fmt"
)

func badCall() {
	a, b := 10, 0
	n := a / b
	fmt.Println(n)
}

func test() {
	defer func() {
		if e := recover(); e != nil {
			fmt.Printf("Panicing %s\n", e)
		}
	}()
	badCall()
	fmt.Printf("After bad caller\n")
}

func main() {
	fmt.Printf("Calling test\n")
	test()
	fmt.Printf("Test completed\n")
}
