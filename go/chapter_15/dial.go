package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	conn, err := net.Dial("tcp", "localhost:80")
	checkConnection(conn, err)
	conn, err = net.Dial("udp", "localhost:80")
	checkConnection(conn, err)
}

func checkConnection(conn net.Conn, err error) {
	if err != nil {
		fmt.Printf("error %v connecting!", conn)
		os.Exit(1)
	}
	fmt.Println("Connection is made with %v", conn)
}
