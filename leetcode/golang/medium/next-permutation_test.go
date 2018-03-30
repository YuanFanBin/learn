package medium

import (
	"reflect"
	"testing"
)

// 31. Next Permutation
//
// Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.
//
// If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).
//
// The replacement must be in-place, do not allocate extra memory.
//
// Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.
// 1,2,3 → 1,3,2
// 3,2,1 → 1,2,3
// 1,1,5 → 1,5,1

func nextPermutation(nums []int) {
	i := 0
	if len(nums) > 2 {
		i = len(nums) - 1
		for ; i > 1 && nums[i-1] >= nums[i]; i-- {
		}
		j := len(nums) - 1
		for ; j >= i && nums[i] >= nums[j]; j-- {
		}
		if i != 0 {
			nums[i-1], nums[j] = nums[j], nums[i-1]
		} else {
			if nums[0] < nums[1] {
				nums[0], nums[j] = nums[j], nums[0]
			}
			i++
		}
	}
	reverse(nums, i, len(nums)-1)
	return
}

func reverse(nums []int, l, r int) {
	for ; l < r; l, r = l+1, r-1 {
		nums[l], nums[r] = nums[r], nums[l]
	}
}

func Test_nextPermutation(t *testing.T) {
	type Case struct {
		nums []int
		exp  []int
	}
	cases := []Case{
		{
			nums: []int{1, 2, 3},
			exp:  []int{1, 3, 2},
		},
		{
			nums: []int{3, 2, 1},
			exp:  []int{1, 2, 3},
		},
		{
			nums: []int{1, 1, 5},
			exp:  []int{1, 5, 1},
		},
		{
			nums: []int{1, 3, 2},
			exp:  []int{2, 1, 3},
		},
		{
			nums: []int{2, 3, 1},
			exp:  []int{3, 1, 2},
		},
	}
	for i, c := range cases {
		if nextPermutation(c.nums); !reflect.DeepEqual(c.nums, c.exp) {
			t.Errorf("%d: %v\n", i, c.nums)
		}
	}
}
