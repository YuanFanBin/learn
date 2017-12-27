package medium

import "testing"

// 12. Integer to Roman
//
// Given an integer, convert it to a roman numeral.
//
// Input is guaranteed to be within the range from 1 to 3999.

func intToRoman(num int) string {
	romans := [4][10]string{
		{"", "M", "MM", "MMM"},
		{"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
		{"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
		{"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
	}
	return romans[0][num/1000] + romans[1][(num%1000)/100] + romans[2][(num%100)/10] + romans[3][num%10]
}

func Test_intToRoman(t *testing.T) {
	type Case struct {
		num int
		exp string
	}
	cases := []Case{
		{num: 1, exp: "I"},
		{num: 99, exp: "XCIX"},
		{num: 18, exp: "XVIII"},
	}
	for i, c := range cases {
		if act := intToRoman(c.num); act != c.exp {
			t.Errorf("%d: %v", i, act)
		}
	}
}

// ref: https://en.wikipedia.org/wiki/Roman_numerals
