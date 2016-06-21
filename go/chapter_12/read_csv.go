package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
	"strings"
)

type Book struct {
	title    string
	price    float64
	quantity int
}

func main() {
	bks := make([]Book, 1)
	file, err := os.Open("products.txt")
	if err != nil {
		log.Fatalf("Error %s opening file products.txt: ", err)
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	for {
		// read one line from the file:
		line, err := reader.ReadString('\n')
		if err == io.EOF {
			break
		}
		// remove \n
		line = string(line[:len(line)-1])
		//fmt.Printf("The input was: -%s-", line)

		strS1 := strings.Split(line, ";")
		book := new(Book)
		book.title = strS1[0]
		book.price, err = strconv.ParseFloat(strS1[1], 32)
		if err != nil {
			fmt.Printf("Error in file: %v", err)
		}
		//fmt.Printf("The quan was:-%s-", strS1[2])
		book.quantity, err = strconv.Atoi(strS1[2])
		if err != nil {
			fmt.Printf("Error in file: %v", err)
		}
		if bks[0].title == "" {
			bks[0] = *book
		} else {
			bks = append(bks, *book)
		}
	}
	fmt.Println("We have read the following books from the file: ")
	for _, bk := range bks {
		fmt.Println(bk)
	}
}
