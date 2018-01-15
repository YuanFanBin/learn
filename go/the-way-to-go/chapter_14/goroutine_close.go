package main

import (
	"fmt"
)

func tel(ch chan int) {
	for i := 0; i < 15; i++ {
		ch <- i
	}
	close(ch)
}

func main() {
	var ok = true
	//var i int
	ch := make(chan int)

	go tel(ch)
	/*
		for ok {
			if i, ok = <-ch; ok {
				fmt.Printf("ok is %t and the counter is at %d\n", ok, i)
			}
		}
	*/
	for i := range ch {
		fmt.Printf("ok is %t and the counter is at %d\n", ok, i)
	}
}
