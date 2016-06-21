package main

import (
	"fmt"
)

type Log struct {
	msg string
}

type Customer struct {
	Name string
	Log
}

func main() {
	c := &Customer{"Barak Obama", Log{"1 - Yes we can!"}}
	c.Add("2 - After me the world will be a better place!")
	//c.Log.Add("2 - After me the world will be a better place!")
	fmt.Println(c)
}

func (l *Log) Add(s string) {
	l.msg += "\n" + s
}

func (l *Log) String() string {
	return l.msg
}

func (c *Customer) String() string {
	return c.Name + "\nLog:" + fmt.Sprintln(c.Log)
}
