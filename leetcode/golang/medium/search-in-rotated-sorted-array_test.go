package medium

import "testing"

// 33. Search in Rotated Sorted Array
//
// Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.
//
// (i.e., [0,1,2,4,5,6,7] might become [4,5,6,7,0,1,2]).
//
// You are given a target value to search. If found in the array return its index, otherwise return -1.
//
// You may assume no duplicate exists in the array.
//
// Your algorithm's runtime complexity must be in the order of O(log n).
//
// Example 1:
//
// Input: nums = [4,5,6,7,0,1,2], target = 0
// Output: 4
// Example 2:
//
// Input: nums = [4,5,6,7,0,1,2], target = 3
// Output: -1

func search(nums []int, target int) int {
	if len(nums) == 0 {
		return -1
	}
	var _search func(int, int) int
	_search = func(l, r int) int {
		medium := (l + r) >> 1
		if nums[l] <= nums[r] { // 单调递增
			switch {
			case l == r:
				if target == nums[l] {
					return l
				}
				return -1
			case nums[l] < target:
				if index := _search(l, medium); index != -1 {
					return index
				}
				return _search(medium+1, r)
			case nums[l] > target:
				return -1
			case nums[l] == target:
				return l
			}
		}
		// 混合
		if index := _search(l, medium); index != -1 {
			return index
		}
		return _search(medium+1, r)
	}
	return _search(0, len(nums)-1)
}

func Test_search(t *testing.T) {
	type Case struct {
		nums   []int
		target int
		exp    int
	}
	cases := []Case{
		{
			[]int{4, 5, 6, 7, 0, 1, 2},
			0,
			4,
		},
		{
			[]int{4, 5, 6, 7, 0, 1, 2},
			3,
			-1,
		},
		{
			[]int{},
			5,
			-1,
		},
	}
	for i, c := range cases {
		if act := search(c.nums, c.target); act != c.exp {
			t.Errorf("case%d: %v(%v)\n", i, act, c.exp)
		}
	}
}
