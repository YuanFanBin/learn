package medium

import (
	"sort"
	"testing"
)

// 16. 3Sum Closest
//
// Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target. Return the sum of the three integers. You may assume that each input would have exactly one solution.
//
//     For example, given array S = {-1 2 1 -4}, and target = 1.
//
//     The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).

func threeSumClosest(nums []int, target int) int {
	switch len(nums) {
	case 0:
		return 0
	case 1:
		return nums[0]
	case 2:
		return nums[0] + nums[1]
	case 3:
		return nums[0] + nums[1] + nums[2]
	}
	sort.Ints(nums)
	abs := func(a int) int {
		if a < 0 {
			return -a
		}
		return a
	}
	closest := nums[0] + nums[1] + nums[2]
	for i := 0; i < len(nums); i++ {
		head := i + 1
		tail := len(nums) - 1
		for head < tail {
			sum := nums[i] + nums[head] + nums[tail]
			if sum == target {
				return sum
			}
			if abs(sum-target) < abs(closest-target) {
				closest = sum
			}
			if sum < target {
				for head++; head < tail && nums[head-1] == nums[head]; head++ {
				}
			} else {
				for tail--; head < tail && nums[tail] == nums[tail+1]; tail-- {
				}
			}
		}
	}
	return closest
}

func Test_thresSumClosest(t *testing.T) {
	type Case struct {
		nums   []int
		target int
		exp    int
	}
	cases := []Case{
		{
			nums:   []int{-1, 2, 1, -4},
			target: 1,
			exp:    2,
		},
		{
			nums:   []int{1, 1, 1, 1},
			target: 0,
			exp:    3,
		},
		{
			nums:   []int{1, 1, 1, 0},
			target: 100,
			exp:    3,
		},
		{
			nums:   []int{0, 2, 1, -3},
			target: 1,
			exp:    0,
		},
	}
	for i, c := range cases {
		if act := threeSumClosest(c.nums, c.target); act != c.exp {
			t.Errorf("%d: %d", i, act)
		}
	}
}
