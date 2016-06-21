package main

import "fmt"

func Mutiply(a, b int, reply *int) {
	*reply = a * b
}

func main() {
	n := 0
	reply := &n
	Mutiply(10, 5, reply)
	fmt.Println("Mutiply:", *reply)
}
