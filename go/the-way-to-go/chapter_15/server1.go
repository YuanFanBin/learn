package main

import (
	"fmt"
	"net"
	"os"
	"strings"
)

var mapUsers map[string]int

func main() {
	var listener net.Listener
	var error error
	var conn net.Conn
	mapUsers = make(map[string]int)

	fmt.Println("Starting the server ...")

	listener, error = net.Listen("tcp", "localhost:50000")
	checkError(error)
	for {
		conn, error = listener.Accept()
		checkError(error)
		go doServeStuff(conn)
	}
}

func doServeStuff(conn net.Conn) {
	var buf []byte
	var error error

	for {
		buf = make([]byte, 512)
		_, error = conn.Read(buf)
		checkError(error)
		input := string(buf)
		if strings.Contains(input, ": SH") {
			fmt.Println("Server shutting down.")
			os.Exit(0)
		}
		if strings.Contains(input, ": WHO") {
			DisplayList()
		}
		// extract clientname:
		ix := strings.Index(input, "says")
		clName := input[0 : ix-1]
		mapUsers[string(clName)] = 1
		fmt.Printf("Received data: --%v--\n", string(buf))
	}
}

func checkError(error error) {
	if error != nil {
		panic("Error: " + error.Error())
	}
}

func DisplayList() {
	fmt.Println("-------------------------------------------------")
	fmt.Println("This is the client list: 1=active, 0=inactive")
	for key, value := range mapUsers {
		fmt.Printf("User %s is %d\n", key, value)
	}
	fmt.Println("-------------------------------------------------")
}
