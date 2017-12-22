package medium

import (
	"math"
	"testing"
)

// 8. String to Integer (atoi)
//
// Implement atoi to convert a string to an integer.
//
// Hint: Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.
//
// Notes: It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front.
//
// Update (2015-02-10):
// The signature of the C++ function had been updated. If you still see your function signature accepts a const char * argument, please click the reload button  to reset your code definition.
//
// spoilers alert... click to show requirements for atoi.
//
// Requirements for atoi:
// The function first discards as many whitespace characters as necessary until the first non-whitespace character is found. Then, starting from this character, takes an optional initial plus or minus sign followed by as many numerical digits as possible, and interprets them as a numerical value.
//
// The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.
//
// If the first sequence of non-whitespace characters in str is not a valid integral number, or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.
//
// If no valid conversion could be performed, a zero value is returned. If the correct value is out of the range of representable values, INT_MAX (2147483647) or INT_MIN (-2147483648) is returned.Implement atoi to convert a string to an integer.

func myAtoi(str string) int {
	switch str {
	case "", "-", "+":
		return 0
	case "0", "1", "2", "3", "4", "5", "6", "7", "8", "9":
		return int(str[0] - '0')
	}
	var (
		res  = 0
		sign = 1
		i    = 0
		j    = 0
	)
	for ; str[i] == ' ' || str[i] == '\t'; i++ { // skip prefix ' ', '\t'
	}
	switch str[i] { // sign
	case '-':
		sign = -1
		i++
	case '+':
		i++
	}
	for ; str[i] == '0'; i++ { // skip '0'
	}
	for j = i; j < len(str); j++ {
		if '0' <= str[j] && str[j] <= '9' {
			continue
		}
		break
	}
	for ; i < j; i++ {
		res = res*10 + int(str[i]-'0')
		if res > math.MaxInt32 {
			if sign == 1 {
				return math.MaxInt32
			} else if res > math.MaxInt32+1 {
				return math.MinInt32
			}
		}
	}
	return res * sign
}

func Test_myAtoi(t *testing.T) {
	t.Logf("0.%v\n", myAtoi("  -0012a42") == -12)
	t.Logf("1.%v\n", myAtoi("") == 0)
	t.Logf("2.%v\n", myAtoi("-") == 0)
	t.Logf("3.%v\n", myAtoi("----") == 0)
	t.Logf("4.%v\n", myAtoi("+") == 0)
	t.Logf("5.%v\n", myAtoi("++++") == 0)
	t.Logf("6.%v\n", myAtoi("1") == 1)
	t.Logf("7.%v\n", myAtoi("+1") == 1)
	t.Logf("8.%v\n", myAtoi("++1") == 0)
	t.Logf("9.%v\n", myAtoi("-1") == -1)
	t.Logf("10.%v\n", myAtoi("--1") == 0)
	t.Logf("11.%v\n", myAtoi("2147483647") == math.MaxInt32)
	t.Logf("12.%v\n", myAtoi("2147483648") == math.MaxInt32)
	t.Logf("13.%v\n", myAtoi("-2147483648") == math.MinInt32)
	t.Logf("14.%v\n", myAtoi("-2147483649") == math.MinInt32)
	t.Logf("15.%v\n", myAtoi("12345678901") == math.MaxInt32)
}
