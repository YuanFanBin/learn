package easy

import (
	"testing"
)

// 27. Remove Element
//
// Given an array and a value, remove all instances of that value in-place and return the new length.
//
// Do not allocate extra space for another array, you must do this by modifying the input array in-place with O(1) extra memory.
//
// The order of elements can be changed. It doesn't matter what you leave beyond the new length.
//
// Example:
//
// Given nums = [3,2,2,3], val = 3,
//
// Your function should return length = 2, with the first two elements of nums being 2.

func removeElement(nums []int, val int) int {
	var n int
	for i := 0; i < len(nums); i++ {
		if nums[i] != val {
			nums[n] = nums[i]
			n++
		}
	}
	return n
}

func Test_removeElement(t *testing.T) {
	type Case struct {
		nums []int
		val  int
		exp  int
	}
	cases := []Case{
		{
			nums: []int{3, 2, 2, 3},
			val:  3,
			exp:  2,
		},
	}
	for i, c := range cases {
		if act := removeElement(c.nums, c.val); act != c.exp {
			t.Errorf("%d: %v(%v)", i, act, c.exp)
		}
	}
}
