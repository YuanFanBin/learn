package main

import (
	"./person"
	"fmt"
)

func main() {
	p := new(person.Person)
	// p.firstName undefined
	// (cannot refer to unexported field or method firstName)
	// p.firstname = "Eric"
	p.SetFirstName("Eric")
	fmt.Println(p.FirstName()) // Output: Eric
	fmt.Println(p)
}
