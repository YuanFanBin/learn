package easy

import "testing"

// 13. Roman to Integer
//
// Given a roman numeral, convert it to an integer.
//
// Input is guaranteed to be within the range from 1 to 3999.

func romanToInt(s string) int {
	oneRoman := map[byte]int{
		'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
	twoRoman := map[string]int{
		"IV": 4, "IX": 9, // IV, IX
		"XL": 40, "XC": 90, // XL, XC
		"CD": 400, "CM": 900} // CD, CM
	res := 0
	for i := 0; i < len(s); i++ {
		switch s[i] {
		case 'I', 'X', 'C':
			if i+1 < len(s) {
				if num, ok := twoRoman[s[i:i+2]]; ok {
					res += num
					i++
				} else {
					res += oneRoman[s[i]]
				}
			} else {
				res += oneRoman[s[i]]
			}
		default:
			res += oneRoman[s[i]]
		}
	}
	return res
}

func Test_romanToInt(t *testing.T) {
	type Case struct {
		s   string
		exp int
	}
	cases := []Case{
		{s: "I", exp: 1},
		{s: "CXCIX", exp: 199},
		{s: "MCDXXXVII", exp: 1437},
		{s: "XVIII", exp: 18},
	}
	for i, c := range cases {
		if act := romanToInt(c.s); act != c.exp {
			t.Errorf("%d: %v", i, act)
		}
	}
}
