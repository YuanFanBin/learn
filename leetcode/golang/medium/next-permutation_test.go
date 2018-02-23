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
	}
	for i, c := range cases {
		nextPermutation(c.nums)
		if !reflect.DeepEqual(c.nums, c.exp) {
			t.Errorf("%d: %v(%v)", i, c.nums, c.exp)
		}
	}
}
