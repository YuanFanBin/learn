package medium

import "testing"

// 11. Container With Most Water
//
// Given n non-negative integers a1, a2, ..., an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.
//
// Note: You may not slant the container and n is at least 2.

func maxArea(height []int) int {
	min := func(a, b int) int {
		if a < b {
			return a
		}
		return b
	}
	max := func(a, b int) int {
		if a > b {
			return a
		}
		return b
	}
	area := 0
	for l, r := 0, len(height)-1; l < r; {
		area = max(area, min(height[l], height[r])*(r-l))
		if height[l] < height[r] {
			l++
		} else {
			r--
		}
	}
	return area
}

func Test_maxArea(t *testing.T) {
	t.Logf("1.%v\n", maxArea([]int{}) == 0)
	t.Logf("2.%v\n", maxArea([]int{2, 1, 3, 2, 4, 1}) == 8)
}
