package medium

import (
	"reflect"
	"sort"
	"testing"
)

// 15. 3Sum
//
// Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.
//
// Note: The solution set must not contain duplicate triplets.
//
// For example, given array S = [-1, 0, 1, 2, -1, -4],
//
// A solution set is:
// [
//   [-1, 0, 1],
//   [-1, -1, 2]
// ]

func threeSum(nums []int) [][]int {
	if len(nums) < 3 {
		return nil
	}
	sort.Ints(nums)
	res := [][]int{}
	twoSum := func(head, tail, target int) {
		for head < tail {
			sum := nums[head] + nums[tail]
			switch {
			case sum < target:
				head++
			case sum > target:
				tail--
			default:
				res = append(res, []int{-target, nums[head], nums[tail]})
				skip := 1
				for head+skip < tail && nums[head] == nums[head+skip] {
					skip++
				}
				head += skip
				skip = 1
				for tail-skip > head && nums[tail] == nums[tail-skip] {
					skip++
				}
				tail -= skip
			}
		}
		return
	}
	for i := 1; i < len(nums); i++ {
		twoSum(i, len(nums)-1, -nums[i-1])
		for ; i < len(nums) && nums[i-1] == nums[i]; i++ {
		}
	}
	return res
}

func Test_threeSum(t *testing.T) {
	type Case struct {
		nums []int
		exp  [][]int
	}
	cases := []Case{
		{
			nums: []int{-1, 0, 1, 2, -1, -4},
			exp: [][]int{
				{-1, -1, 2}, {-1, 0, 1},
			},
		},
		{
			nums: []int{-1, 0, 1},
			exp: [][]int{
				{-1, 0, 1},
			},
		},
		{
			nums: []int{0, 0, 0, 0},
			exp: [][]int{
				{0, 0, 0},
			},
		},
		{
			nums: []int{-4, -2, -2, -2, 0, 1, 2, 2, 2, 3, 3, 4, 4, 6, 6},
			exp: [][]int{
				{-4, -2, 6}, {-4, 0, 4}, {-4, 1, 3}, {-4, 2, 2}, {-2, -2, 4}, {-2, 0, 2},
			},
		},
	}
	for i, c := range cases {
		if act := threeSum(c.nums); !reflect.DeepEqual(act, c.exp) {
			t.Errorf("%d: %v", i, act)
		}
	}
}
