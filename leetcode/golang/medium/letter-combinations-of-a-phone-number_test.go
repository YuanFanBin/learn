package medium

import (
	"bytes"
	"container/list"
	"reflect"
	"testing"
)

// 17. Letter Combinations of a Phone Number
//
// Given a digit string, return all possible letter combinations that the number could represent.
//
// A mapping of digit to letters (just like on the telephone buttons) is given below.
//
// 	1     2     3
//       abc   def
//  4     5     6
// ghi   jkl   mno
//  7     8     9
// pqrs  tuv   wxyz
//
// Input:Digit string "23"
// Output: ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].
// Note:
// Although the above answer is in lexicographical order, your answer could be in any order you want.

func letterCombinations(digits string) []string {
	if len(digits) == 0 {
		return nil
	}
	digits2map := map[byte][]byte{
		'2': []byte{'a', 'b', 'c'},
		'3': []byte{'d', 'e', 'f'},
		'4': []byte{'g', 'h', 'i'},
		'5': []byte{'j', 'k', 'l'},
		'6': []byte{'m', 'n', 'o'},
		'7': []byte{'p', 'q', 'r', 's'},
		'8': []byte{'t', 'u', 'v'},
		'9': []byte{'w', 'x', 'y', 'z'}}
	fifo := list.New()
	for i := 0; i < len(digits); i++ {
		l := fifo.Len()
		switch {
		case l == 0:
			for _, c := range digits2map[digits[i]] {
				fifo.PushBack([]byte{c})
			}
		default:
			for j := 0; j < l; j++ {
				front := fifo.Front()
				prefix := front.Value.([]byte)
				for _, c := range digits2map[digits[i]] {
					p := bytes.NewBuffer(nil)
					p.Write(prefix)
					p.WriteByte(c)
					fifo.PushBack(p.Bytes())
				}
				fifo.Remove(front)
			}
		}
	}
	ans := []string{}
	for s := fifo.Front(); s != nil; s = fifo.Front() {
		ans = append(ans, string(s.Value.([]byte)))
		fifo.Remove(s)
	}
	return ans
}

func Test_letterCombinations(t *testing.T) {
	type Case struct {
		digits string
		exp    []string
	}
	cases := []Case{
		{
			digits: "23",
			exp:    []string{"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"},
		},
		{
			digits: "2",
			exp:    []string{"a", "b", "c"},
		},
		{
			digits: "234",
			exp:    []string{"adg", "adh", "adi", "aeg", "aeh", "aei", "afg", "afh", "afi", "bdg", "bdh", "bdi", "beg", "beh", "bei", "bfg", "bfh", "bfi", "cdg", "cdh", "cdi", "ceg", "ceh", "cei", "cfg", "cfh", "cfi"},
		},
	}
	for i, c := range cases {
		if act := letterCombinations(c.digits); !reflect.DeepEqual(act, c.exp) {
			t.Errorf("%d: %v(%v)", i, act, c.exp)
		}
	}
}
