package main

import "fmt"

func main() {
	// #1 未发生拷贝
	var a = []int{1, 2, 3}
	b := a
	b[1] = 10
	fmt.Println(a)
	fmt.Println(b)

	// #2 ?????
	var c = new([3]int)
	d := c
	d[1] = 10
	fmt.Println(c)
	fmt.Println(d)

	// #3
	var e = []int{4, 5, 6}
	p(e)
	fmt.Println(e)

	// #4
	var f = new([3]int)
	p2(f)
	fmt.Println(f)
}

func p(arr []int) {
	arr[1] = 10
	fmt.Println(arr)
}

func p2(arr *[3]int) {
	arr[1] = 10
	fmt.Println(arr)
}
