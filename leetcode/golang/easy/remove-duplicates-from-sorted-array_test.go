package easy

import "testing"

// 26. Remove Duplicates from Sorted Array
//
// Given a sorted array, remove the duplicates in-place such that each element appear only once and return the new length.
//
// Do not allocate extra space for another array, you must do this by modifying the input array in-place with O(1) extra memory.
//
// Example:
//
// Given nums = [1,1,2],
//
// Your function should return length = 2, with the first two elements of nums being 1 and 2 respectively.
// It doesn't matter what you leave beyond the new length.Given a sorted array, remove the duplicates in-place such that each element appear only once and return the new length.
//
// Do not allocate extra space for another array, you must do this by modifying the input array in-place with O(1) extra memory.
//
// Example:
//
// Given nums = [1,1,2],
//
// Your function should return length = 2, with the first two elements of nums being 1 and 2 respectively.
// It doesn't matter what you leave beyond the new length.

func removeDuplicates(nums []int) int {
	if len(nums) == 0 || len(nums) == 1 {
		return len(nums)
	}
	index := 0
	for i := 1; i < len(nums); i++ {
		if nums[i] != nums[index] {
			index++
			nums[index] = nums[i]
		}
	}
	return index + 1
}

func Test_removeDuplicates(t *testing.T) {
	type Case struct {
		nums []int
		exp  int
	}
	cases := []Case{
		{
			nums: []int{1, 1, 2},
			exp:  2,
		},
		{
			nums: []int{1},
			exp:  1,
		},
		{
			nums: []int{1, 1, 1, 1, 1, 1, 1},
			exp:  1,
		},
		{
			nums: []int{1, 2, 3, 4, 5, 6, 7},
			exp:  7,
		},
	}
	for i, c := range cases {
		if act := removeDuplicates(c.nums); act != c.exp {
			t.Errorf("%d: %d(%d)\n", i, act, c.exp)
		}
	}
}
