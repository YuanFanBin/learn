package medium

import (
	"reflect"
	"sort"
	"testing"
)

// 18. 4Sum
//
// Given an array S of n integers, are there elements a, b, c, and d in S such that a + b + c + d = target? Find all unique quadruplets in the array which gives the sum of target.
//
// Note: The solution set must not contain duplicate quadruplets.
//
// For example, given array S = [1, 0, -1, 0, -2, 2], and target = 0.
//
// A solution set is:
// [
//   [-1,  0, 0, 1],
//   [-2, -1, 1, 2],
//   [-2,  0, 0, 2]
// ]

func fourSum(nums []int, target int) [][]int {
	sort.Ints(nums)
	res := [][]int{}
	len := len(nums)
	for i := 0; i < len-3; i++ {
		for j := i + 1; j < len-2; j++ {
			m := j + 1
			n := len - 1
			for m < n {
				sum := nums[i] + nums[j] + nums[m] + nums[n]
				switch {
				case sum < target:
					m++
				case sum > target:
					n--
				default:
					res = append(res, []int{nums[i], nums[j], nums[m], nums[n]})
					for m < n && nums[m] == nums[m+1] {
						m++
					}
					m++
					for n < n && nums[n-1] == nums[n] {
						n--
					}
					n--
				}
			}
			for j < len-2 && nums[j] == nums[j+1] {
				j++
			}
		}
		for i < len-3 && nums[i] == nums[i+1] {
			i++
		}
	}
	return res
}

func nSum(nums []int, n, target int, prefix []int, ans *[][]int) {
	if len(nums) < n {
		return
	}
	if n == 2 {
		for head, tail := 0, len(nums)-1; head < tail; {
			sum := nums[head] + nums[tail]
			switch {
			case sum < target:
				head++
			case sum > target:
				tail--
			default:
				*ans = append(*ans, append(prefix, nums[head], nums[tail]))
				for head < tail && nums[head] == nums[head+1] {
					head++
				}
				head++
				for head < tail && nums[tail-1] == nums[tail] {
					tail--
				}
				tail--
			}
		}
		return
	}
	for i := 0; i < len(nums)-n+1; i++ {
		if i == 0 || nums[i] != nums[i-1] {
			nSum(nums[i+1:], n-1, target-nums[i], append(prefix, nums[i]), ans)
		}
	}
}

func fourSum2(nums []int, target int) [][]int {
	sort.Ints(nums)
	ans := [][]int{}
	if len(nums) < 4 {
		return ans
	}
	nSum(nums, 4, target, []int{}, &ans)
	return ans
}

func Test_fourSum(t *testing.T) {
	type Case struct {
		nums   []int
		target int
		exp    [][]int
	}
	cases := []Case{
		{
			nums:   []int{1, 0, -1, 0, -2, 2},
			target: 0,
			exp: [][]int{
				{-1, 0, 0, 1},
				{-2, -1, 1, 2},
				{-2, 0, 0, 2},
			},
		},
		{
			nums:   []int{0, 0, 0, 0},
			target: 0,
			exp: [][]int{
				{0, 0, 0, 0},
			},
		},
		{
			nums:   []int{-2, 0, 0, 3, 3, -1},
			target: 5,
			exp: [][]int{
				{-1, 0, 3, 3},
			},
		},
	}
	for i, c := range cases {
		if act := fourSum(c.nums, c.target); !reflect.DeepEqual(act, c.exp) {
			t.Errorf("%d: %v\n", i, act)
		}
	}
}
