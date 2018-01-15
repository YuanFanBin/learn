package main

import (
	"fmt"
)

func badCall() {
	panic("bad end")
}

func test() {
	defer func() {
		if e := recover(); e != nil {
			fmt.Printf("Panicing %s\n", e)
		}
	}()
	badCall()
	fmt.Printf("After bad call\n")
}

func main() {
	fmt.Printf("Calling test\n")
	test()
	fmt.Printf("Test completed\n")
}
