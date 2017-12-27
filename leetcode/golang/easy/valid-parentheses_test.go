package easy

import "testing"

// 20. Valid Parentheses
//
// Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
//
// The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not.

func isValid(s string) bool {
	if len(s)%2 == 1 {
		return false
	}
	stack := make([]byte, len(s)) // len(s)/2
	top := -1
	need := map[byte]byte{')': '(', '}': '{', ']': '['}
	for i := 0; i < len(s); i++ {
		switch s[i] {
		case '(', '{', '[':
			top++
			stack[top] = s[i] // if top > len(s) / 2 return false
		default:
			if top == -1 || stack[top] != need[s[i]] {
				return false
			}
			top--
		}
	}
	return top == -1
}

func Test_isValid(t *testing.T) {
	type Case struct {
		s   string
		exp bool
	}
	cases := []Case{
		{s: "([])", exp: true},
		{s: "()[]{}", exp: true},
		{s: "(])", exp: false},
		{s: "([)]", exp: false},
		{s: "((", exp: false},
	}
	for i, c := range cases {
		if act := isValid(c.s); act != c.exp {
			t.Errorf("%d: %v", i, act)
		}
	}
}
