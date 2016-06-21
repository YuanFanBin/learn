package main

import (
	"errors"
	"fmt"
	"math"
)

func main() {
	fmt.Print("First example with -1: ")
	ret1, err1 := MySqrt(-1)
	if err1 != nil {
		fmt.Println("Error! Return values: ", ret1, err1)
	} else {
		fmt.Println("It's ok! Return values are: ", ret1, err1)
	}

	fmt.Print("Second example with 5: ")
	if ret2, err2 := MySqrt(5); err2 != nil {
		fmt.Println("Error! Return values: ", ret2, err2)
	} else {
		fmt.Println("It's ok! Return values are: ", ret2, err2)
	}
	fmt.Println(MySqrt2(5))
}

func MySqrt(f float64) (float64, error) {
	if f < 0 {
		return float64(math.NaN()), errors.New("I won't be able to do a sqrt of negative number!")
	}
	return math.Sqrt(f), nil
}

func MySqrt2(f float64) (ret float64, err error) {
	if f < 0 {
		ret = float64(math.NaN())
		err = errors.New("I won't be able to do a sqrt of negative number!")
	} else {
		ret = math.Sqrt(f)
	}
	return
}
