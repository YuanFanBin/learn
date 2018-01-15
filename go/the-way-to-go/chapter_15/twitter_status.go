package main

import (
	"encoding/xml"
	"fmt"
	"net/http"
)

type Status struct {
	Text string
}

type User struct {
	XMLName xml.Name
	Status  Status
}

func main() {
	response, _ := http.Get("http://twitter.com/users/Googland.xml")
	user := User{xml.Name{"", "user"}, Status{""}}
	xml.Unmarshal(response.Body, &user)
	fmt.Printf("status: %s", user.Status.Text)
}
