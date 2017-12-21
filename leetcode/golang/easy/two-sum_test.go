package easy

import "testing"

// 1. Two Sum
//
// Given an array of integers, return indices of the two numbers such that they add up to a specific target.
//
// You may assume that each input would have exactly one solution, and you may not use the same element twice.
//
// Example:
// Given nums = [2, 7, 11, 15], target = 9,
//
// Because nums[0] + nums[1] = 2 + 7 = 9,
// return [0, 1].

func twoSum(nums []int, target int) []int {
	need := map[int]int{}
	for i, num := range nums {
		if index, ok := need[target-num]; ok {
			return []int{index, i}
		}
		need[num] = i
	}
	return nil
}

func Test_twoSum(t *testing.T) {
	t.Logf("%v", twoSum([]int{2, 7, 11, 15}, 9))
}
