package easy

import "testing"

// 9. Palindrome Number
//
// Determine whether an integer is a palindrome. Do this without extra space.
//
// click to show spoilers.
//
// Some hints:
// Could negative integers be palindromes? (ie, -1)
//
// If you are thinking of converting the integer to string, note the restriction of using extra space.
//
// You could also try reversing an integer. However, if you have solved the problem "Reverse Integer", you know that the reversed integer might overflow. How would you handle such case?
//
// There is a more generic way of solving this problem.Determine whether an integer is a palindrome. Do this without extra space.

func isPalindrome(x int) bool {
	if x < 0 {
		return false
	}
	if x < 10 {
		return true
	}
	if x%10 == 0 {
		return false
	}
	r := 0
	for x != 0 {
		if x == r || (x >= 10 && x/10 == r) {
			return true
		}
		r = r*10 + x%10
		x /= 10
	}
	return false
}

func Test_isPalindrome(t *testing.T) {
	t.Logf("%v", isPalindrome(0) == true)
	t.Logf("%v", isPalindrome(123) == false)
	t.Logf("%v", isPalindrome(12321) == true)
	t.Logf("%v", isPalindrome(111) == true)
	t.Logf("%v", isPalindrome(1111) == true)
	t.Logf("%v", isPalindrome(10) == false)
	t.Logf("%v", isPalindrome(11) == true)
	t.Logf("%v", isPalindrome(100) == false)
	t.Logf("%v", isPalindrome(1000) == false)
	t.Logf("%v", isPalindrome(101) == true)
	t.Logf("%v", isPalindrome(21120) == false)
}
