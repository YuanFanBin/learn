package medium

import (
	"reflect"
	"testing"
)

// 22. Generate Parentheses
//
// Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.
//
// For example, given n = 3, a solution set is:
//
// [
//   "((()))",
//   "(()())",
//   "(())()",
//   "()(())",
//   "()()()"
// ]

func generateParenthesis(n int) []string {
	var ans []string
	generate([]byte{}, 0, n<<1, &ans)
	return ans
}

func generate(current []byte, sum, n int, ans *[]string) {
	// fmt.Printf("current: %s, sum: %d, n: %d\n", string(current), sum, n)
	if sum < 0 || n == 0 {
		return
	}
	switch {
	case sum == n:
		for i := 0; i < sum; i++ {
			current = append(current, ')')
		}
		*ans = append(*ans, string(current))
	case sum < n:
		generate(append(current, '('), sum+1, n-1, ans)
		generate(append(current, ')'), sum-1, n-1, ans)
	}
}

func Test_generateParenthesis(t *testing.T) {
	type Case struct {
		n   int
		exp []string
	}
	cases := []Case{
		{
			n:   3,
			exp: []string{"((()))", "(()())", "(())()", "()(())", "()()()"},
		},
	}
	for i, c := range cases {
		if act := generateParenthesis(c.n); !reflect.DeepEqual(act, c.exp) {
			t.Errorf("%d: %v", i, act)
		}
	}
}
