package easy

import (
	"math"
	"testing"
)

// 7. Reverse Integer
//
// Given a 32-bit signed integer, reverse digits of an integer.
//
// Example 1:
//
// Input: 123
// Output:  321
// Example 2:
//
// Input: -123
// Output: -321
// Example 3:
//
// Input: 120
// Output: 21
// Note:
// Assume we are dealing with an environment which could only hold integers within the 32-bit signed integer range. For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.

func reverse(x int) int {
	sign := 1
	if x < 0 {
		x = -x
		sign = -1
	}
	res := 0
	for x != 0 {
		res = res*10 + x%10
		if res > math.MaxInt32 || res*sign < math.MinInt32 {
			return 0
		}
		x /= 10
	}
	return res * sign
}

func Test_reverse(t *testing.T) {
	t.Logf("%v", reverse(123))
	t.Logf("%v", reverse(-123))
	t.Logf("%v", reverse(120))
	t.Logf("%v", reverse(0))
	t.Logf("%v", reverse(1))
	t.Logf("%v", reverse(-1))
	t.Logf("%v", reverse(1534236469))
	t.Logf("%v", reverse(math.MaxInt32))
	t.Logf("%v", reverse(math.MinInt32))
}
