package easy

import "testing"

// 35. Search Insert Position
//
// Given a sorted array and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.
//
// You may assume no duplicates in the array.
//
// Example 1:
//
// Input: [1,3,5,6], 5
// Output: 2
// Example 2:
//
// Input: [1,3,5,6], 2
// Output: 1
// Example 3:
//
// Input: [1,3,5,6], 7
// Output: 4
// Example 4:
//
// Input: [1,3,5,6], 0
// Output: 0

func searchInsert(nums []int, target int) int {
	l := 0
	r := len(nums) - 1
	for l <= r {
		if l == r {
			if nums[l] == target { // 存在，返回原位置
				return l
			}
			break
		}
		m := (l + r) >> 1
		if nums[m] < target {
			l = m + 1
		} else {
			r = m
		}
	}
	if nums[l] < target { // 比找到的位置大，插入当前的下一个位置
		return l + 1
	}
	return l // 比找到的位置小，插入当前位置
}

func Test_searchInsert(t *testing.T) {
	type Case struct {
		nums   []int
		target int
		exp    int
	}
	cases := []Case{
		{
			[]int{1, 3, 5, 6},
			5,
			2,
		},
		{
			[]int{1, 3, 5, 6},
			2,
			1,
		},
		{
			[]int{1, 3, 5, 6},
			7,
			4,
		},
		{
			[]int{1, 3, 5, 6},
			0,
			0,
		},
	}
	for i, c := range cases {
		if act := searchInsert(c.nums, c.target); act != c.exp {
			t.Errorf("case%d: %v(%v)\n", i, act, c.exp)
		}
	}
}
