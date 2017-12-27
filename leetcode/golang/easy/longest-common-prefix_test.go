package easy

import "testing"

// 14. Longest Common Prefix
//
// Write a function to find the longest common prefix string amongst an array of strings.

func longestCommonPrefix(strs []string) string {
	if len(strs) == 0 {
		return ""
	}
	if len(strs) == 1 {
		return strs[0]
	}
	var (
		i   = 0
		end = false
	)
	for !end {
		if i >= len(strs[0]) {
			i++
			break
		}
		for j := 1; j < len(strs); j++ {
			if i >= len(strs[j]) || strs[j][i] != strs[0][i] {
				end = true
				break
			}
		}
		i++
	}
	return strs[0][0 : i-1]
}

func Test_longestCommonPrefix(t *testing.T) {
	type Case struct {
		strs []string
		exp  string
	}
	cases := []Case{
		{
			strs: []string{},
			exp:  "",
		},
		{
			strs: []string{"12345"},
			exp:  "12345",
		},
		{
			strs: []string{"12345", "12321"},
			exp:  "123",
		},
		{
			strs: []string{"12345", ""},
			exp:  "",
		},
	}
	for i, c := range cases {
		if act := longestCommonPrefix(c.strs); act != c.exp {
			t.Errorf("%d: '%s'", i, act)
		}
	}
}
