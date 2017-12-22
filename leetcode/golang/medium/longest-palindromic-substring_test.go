package medium

import "testing"

// 5. Longest Palindromic Substring
//
// Given a string s, find the longest palindromic substring in s. You may assume that the maximum length of s is 1000.
//
// Example:
//
// Input: "babad"
//
// Output: "bab"
//
// Note: "aba" is also a valid answer.
// Example:
//
// Input: "cbbd"
//
// Output: "bb"

func longestPalindrome(s string) string {
	n := len(s)
	if len(s) <= 1 {
		return s
	}
	var (
		i   = 0
		max = 1
		end = 1
	)
	for j := i + 1; j < n; j++ {
		len := 0
		for found := 0; found < j; {
			l := found + len
			r := j - len
			if l <= r && s[l] == s[r] {
				len++
				continue
			}
			if l-r == 2 { // aba
				if max < len<<1-1 {
					max = len<<1 - 1
					end = j
				}
			} else if l-r == 1 { // aa
				if max < len<<1 {
					max = len << 1
					end = j
				}
			}
			if j-found < max {
				break
			}
			found++
			len = 0
		}
	}
	if max == 1 {
		return s[0:1]
	}
	return s[end+1-max : end+1]
}

func Test_longestPalindrome(t *testing.T) {
	t.Logf("%v", longestPalindrome("babad") == "bab")
	t.Logf("%v", longestPalindrome("cbbd") == "bb")
	t.Logf("%v", longestPalindrome("aa") == "aa")
	t.Logf("%v", longestPalindrome("aaaa") == "aaaa")
	t.Logf("%v", longestPalindrome("aaaaa") == "aaaaa")
	t.Logf("%v", longestPalindrome("1aabaa4") == "aabaa")
	t.Logf("%v", longestPalindrome("abcdasdfghjkldcba") == "a")
}
