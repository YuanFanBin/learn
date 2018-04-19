package easy

import "testing"

// 38. Count and Say
//
// The count-and-say sequence is the sequence of integers with the first five terms as following:
//
// 1.     1
// 2.     11
// 3.     21
// 4.     1211
// 5.     111221
// 1 is read off as "one 1" or 11.
// 11 is read off as "two 1s" or 21.
// 21 is read off as "one 2, then one 1" or 1211.
// Given an integer n, generate the nth term of the count-and-say sequence.
//
// Note: Each term of the sequence of integers will be represented as a string.
//
// Example 1:
//
// Input: 1
// Output: "1"
//
// Example 2:
//
// Input: 4
// Output: "1211"

func countAndSay(n int) string {
	if n <= 0 {
		return ""
	}
	var (
		current  = []byte{'1'}
		next     = []byte{}
		int2byte = []byte("0123456789")
	)
	for n--; n > 0; n-- {
		for i := 0; i < len(current); {
			k := 0
			for ; i+k < len(current) && current[i+k] == current[i]; k++ {
			}
			next = append(next, int2byte[k], current[i])
			// next += strconv.Itoa(k) + string(current[i])
			i = i + k
		}
		current = next
		// next = ""
		next = []byte{}
	}
	return string(current)
}

func Test_countAndSay(t *testing.T) {
	type Case struct {
		n   int
		exp string
	}
	cases := []Case{
		{
			n:   1,
			exp: "1",
		},
		{
			n:   4,
			exp: "1211",
		},
		{
			n:   5,
			exp: "111221",
		},
		{
			n:   6,
			exp: "312211",
		},
		{
			n:   7,
			exp: "13112221",
		},
	}
	for i, c := range cases {
		if act := countAndSay(c.n); act != c.exp {
			t.Errorf("case%d: %v(%v)", i, act, c.exp)
		}
	}
}
