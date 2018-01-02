package easy

import (
	"testing"
)

// 28. Implement strStr()
//
// Implement strStr().
//
// Return the index of the first occurrence of needle in haystack, or -1 if needle is not part of haystack.
//
// Example 1:
//
// Input: haystack = "hello", needle = "ll"
// Output: 2
// Example 2:
//
// Input: haystack = "aaaaa", needle = "bba"
// Output: -1

func strStr(haystack string, needle string) int {
	if len(needle) == 0 {
		return 0
	}
	for i := 0; i < len(haystack); i++ {
		if len(haystack)-i < len(needle) {
			return -1
		}
		j := 0
		for j < len(needle) && haystack[i+j] == needle[j] {
			j++
		}
		if j == len(needle) {
			return i
		}
	}
	return -1
}

func Test_strStr(t *testing.T) {
	type Case struct {
		haystack string
		needle   string
		exp      int
	}
	cases := []Case{
		{
			haystack: "hello",
			needle:   "ll",
			exp:      2,
		},
		{
			haystack: "aaaaa",
			needle:   "bba",
			exp:      -1,
		},
		{
			haystack: "mississippi",
			needle:   "issip",
			exp:      4,
		},
	}
	for i, c := range cases {
		if act := strStr(c.haystack, c.needle); act != c.exp {
			t.Errorf("%d: %v(%v)", i, act, c.exp)
		}
	}
}
