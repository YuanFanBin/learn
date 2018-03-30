package medium

import "testing"
import "math"

// 29. Divide Two Integers
//
// Divide two integers without using multiplication, division and mod operator.
//
// If it is overflow, return MAX_INT.

func divide(dividend int, divisor int) int {
	if divisor == 0 {
		return math.MaxInt32
	}
	if dividend == math.MinInt32 && divisor == -1 {
		return math.MaxInt32
	}
	sign := 1
	if dividend < 0 {
		sign = -1
		dividend = -dividend
	}
	if divisor < 0 {
		sign *= -1
		divisor = -divisor
	}
	ans := 0
	for {
		dividend -= divisor
		switch {
		case dividend < 0:
			return ans * sign
		case dividend == 0:
			return (ans + 1) * sign
		default:
			ans++
		}
	}
}

func Test_divide(t *testing.T) {
	type Case struct {
		dividend int
		divisor  int
		exp      int
	}
	cases := []Case{
		{
			dividend: 10,
			divisor:  3,
			exp:      3,
		},
		{
			dividend: -5,
			divisor:  3,
			exp:      -1,
		},
		{
			dividend: 1,
			divisor:  1,
			exp:      1,
		},
		{
			dividend: 2147483647,
			divisor:  2,
			exp:      1073741823,
		},
	}
	for i, c := range cases {
		if act := divide(c.dividend, c.divisor); act != c.exp {
			t.Errorf("%d: %d(%d)", i, act, c.exp)
		}
	}
}
