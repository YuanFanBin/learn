package medium

import "testing"

// 3. Longest Substring Without Repeating Characters
//
// Given a string, find the length of the longest substring without repeating characters.
//
// Examples:
//
// Given "abcabcbb", the answer is "abc", which the length is 3.
//
// Given "bbbbb", the answer is "b", with the length of 1.
//
// Given "pwwkew", the answer is "wke", with the length of 3. Note that the answer must be a substring, "pwke" is a subsequence and not a substring.

func lengthOfLongestSubstring(s string) int {
	n := len(s)
	if n <= 1 {
		return n
	}
	var (
		i   = 0
		j   = 0
		max = 1
	)
	for j = i + 1; j < n; j++ {
		found := i
		for ; found < j; found++ {
			if s[found] == s[j] {
				break
			}
		}
		if found != j {
			if j-i > max {
				max = j - i
			}
			i = found + 1
		}
	}
	if j-i > max {
		return j - i
	}
	return max
}

func Test_lengthOfLongestSubstring(t *testing.T) {
	// - 0  1  2  3  4  5  6  7
	// - a  b  c  a  b  c  b  b
	// 1 i  j  .  .  .  .  .  .    -> (s[0] != s[1])
	//      .  .  .  .  .  .  .    -> max = 1, found = 1, i = 0
	// 2 i  .  j  .  .  .  .  .    -> (s[0] != s[2] -> s[1] != s[2])
	//      .     .  .  .  .  .    -> max = 1, found = 2, i = 0
	// 3 i  .     j  .  .  .  .    -> (s[0] != s[3] -> s[1] != s[3] -> s[2] != s[3])
	//      .     .  .  .  .  .    -> max = 1, found = 3, i = 0
	// 4 i  .     .  j  .  .  .    -> (s[0] == s[4])
	//      .     .     .  .  .    -> max = 3, found = 1, i = found + 1 => 1
	// 5    i     .     j  .  .    -> (s[1] != s[5], s[2] == s[5])
	//            .     .  .  .    -> max = 3, found = 2, i = found + 1 => 3
	// 6          i     .  j  .    -> (s[3] != s[6], s[4] == s[6])
	//                  .     .    -> max = 3, found = 4, i = found + 1 => 5
	// 7                i     j    -> (s[5] != s[7], s[6] == s[7])
	//                             -> max = 3, found = 6, i = found + 1 => 7
	t.Logf("%d", lengthOfLongestSubstring("abcabcbb"))
}
