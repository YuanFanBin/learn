package medium

import (
	"testing"
)

// 6. ZigZag Conversion
//
// The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)
//
// P   A   H   N
// A P L S I I G
// Y   I   R
// And then read line by line: "PAHNAPLSIIGYIR"
// Write the code that will take a string and make this conversion given a number of rows:
//
// string convert(string text, int nRows);
// convert("PAYPALISHIRING", 3) should return "PAHNAPLSIIGYIR".

func convert(s string, numRows int) string {
	bucket := map[int][]byte{}
	for i := 0; i < len(s); {
		for k := 1; i < len(s) && k <= numRows; k++ {
			bucket[k] = append(bucket[k], byte(s[i]))
			i++
		}
		for k := numRows - 1; i < len(s) && k > 1; k-- {
			bucket[k] = append(bucket[k], byte(s[i]))
			i++
		}
	}
	res := []byte{}
	for i := 1; i <= numRows; i++ {
		res = append(res, bucket[i]...)
	}
	return string(res)
}

func Test_convert(t *testing.T) {
	t.Logf("%v\n", convert("PAYPALISHIRING", 3) == "PAHNAPLSIIGYIR")
	t.Logf("%v\n", convert("ABC", 3) == "ABC")
	t.Logf("%v\n", convert("AB", 3) == "AB")
}
