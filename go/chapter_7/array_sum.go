package main

import "fmt"

func main() {
	array := [3]float64{7.0, 8.5, 9.1}
	change(array) /* copy */
	fmt.Println(array)

	x := Sum(&array)
	fmt.Printf("The sum of the array is: %f\n", x)

	// #2
	array2 := new([3]float64)
	y := Sum(array2)
	fmt.Printf("The sum of the array is: %f\n", y)
}

func Sum(a *[3]float64) (sum float64) {
	for _, v := range a {
		sum += v
	}
	return sum
}

func change(a [3]float64) {
	a[1] = 100.0
	fmt.Println(a)
}
