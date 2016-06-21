package main

import "fmt"

type Shaper interface {
	Area() float32
}

// Square
type Square struct {
	side float32
}

func (sq *Square) Area() float32 {
	return sq.side * sq.side
}

// Rectangle
type Rectangle struct {
	length, width float32
}

func (r Rectangle) Area() float32 {
	return r.length * r.width
}

func main() {
	r := Rectangle{5, 3} // Area() of Rectangle needs a value
	q := &Square{5}      // Area() of Square need a pointer
	// shapes := []Shaper{Shaper(r), Shaper(q)}
	// or shoter
	shapes := []Shaper{r, q}
	fmt.Println("Looping through shapes for area ...")
	for n, _ := range shapes {
		fmt.Println("Shape details: ", shapes[n])
		fmt.Println("Area of this shape is: ", shapes[n].Area())
	}
}
