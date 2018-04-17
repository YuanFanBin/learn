package medium

import "testing"
import "math"

// 29. Divide Two Integers
//
// Divide two integers without using multiplication, division and mod operator.
//
// If it is overflow, return MAX_INT.

func divide(dividend, divisor int) int {
	if divisor == 0 || (dividend == math.MinInt32 && divisor == -1) {
		return math.MaxInt32
	}
	sign := 1
	if dividend < 0 {
		sign *= -1
		dividend = -dividend
	}
	if divisor < 0 {
		sign *= -1
		divisor = -divisor
	}
	res := 0
	for dividend >= divisor {
		multi := 1
		maxShift := divisor
		for dividend >= (maxShift << 1) { // 被减数 - divisor * {max}, max = 2 ** n ==> multi
			maxShift <<= 1
			multi <<= 1
		}
		dividend -= maxShift
		res += multi
	}
	return res * sign
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
