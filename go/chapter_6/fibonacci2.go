package main

import "fmt"

func main() {
	result := 0
	pos := 0
	for i := 0; i <= 10; i++ {
		result, pos = fibonacci(i)
		fmt.Printf("fibonacci(%d) is: %d, %d\n", i, result, pos)
	}
}

func fibonacci(n int) (res int, pos int) {
	if n <= 1 {
		res = 1
		pos = 1
	} else {
		res_1, pos_1 := fibonacci(n - 1)
		res_2, _ := fibonacci(n - 2)
		//res, pos = fibonacci(n-1) + fibonacci(n-2)
		res = res_1 + res_2
		pos = pos_1 + 1
	}
	return
}
