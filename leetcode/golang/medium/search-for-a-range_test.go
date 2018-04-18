package medium

import (
	"reflect"
	"testing"
)

// 34. Search for a Range
//
// Given an array of integers nums sorted in ascending order, find the starting and ending position of a given target value.
//
// Your algorithm's runtime complexity must be in the order of O(log n).
//
// If the target is not found in the array, return [-1, -1].
//
// Example 1:
//
// Input: nums = [5,7,7,8,8,10], target = 8
// Output: [3,4]
// Example 2:
//
// Input: nums = [5,7,7,8,8,10], target = 6
// Output: [-1,-1]

func searchRange(nums []int, target int) []int {
	l := 0
	r := len(nums) - 1
	for l <= r {
		if nums[l] > target {
			return []int{-1, -1}
		}
		if nums[l] == target {
			i := l
			// 可能部分target在右边，i < len(nums)
			for ; i < len(nums) && nums[i] == target; i++ {
			}
			if i == len(nums)-1 && nums[i] == target {
				return []int{l, r}
			}
			return []int{l, i - 1}
		}
		m := (l + r) >> 1
		if nums[m] < target { // target在右边
			l = m + 1
		} else { // target在左边，可能部分target在右边
			r = m
		}
	}
	return []int{-1, -1}
}

func Test_searchRange(t *testing.T) {
	type Case struct {
		nums   []int
		target int
		exp    []int
	}
	cases := []Case{
		{
			[]int{5, 7, 7, 8, 8, 10},
			8,
			[]int{3, 4},
		},
		{
			[]int{5, 7, 7, 8, 8, 10},
			6,
			[]int{-1, -1},
		},
		{
			[]int{},
			1,
			[]int{-1, -1},
		},
		{
			[]int{1, 1, 1, 1, 1},
			1,
			[]int{0, 4},
		},
		{
			[]int{1},
			1,
			[]int{0, 0},
		},
		{
			[]int{1, 2, 2},
			2,
			[]int{1, 2},
		},
	}
	for i, c := range cases {
		if act := searchRange(c.nums, c.target); !reflect.DeepEqual(act, c.exp) {
			t.Errorf("case%d: %v(%v)\n", i, act, c.exp)
		}
	}
}
