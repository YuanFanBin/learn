package medium

import (
	"fmt"
	"testing"
)

// 2. Add Two Numbers
//
// You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.
//
// You may assume the two numbers do not contain any leading zero, except the number 0 itself.
//
// Example
//
// Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
// Output: 7 -> 0 -> 8
// Explanation: 342 + 465 = 807.

type ListNode struct {
	Val  int
	Next *ListNode
}

func addTwoNumbers(l1 *ListNode, l2 *ListNode) *ListNode {
	if l1 == nil {
		return l2
	}
	if l2 == nil {
		return l1
	}
	res := &ListNode{}
	var (
		t1    = l1
		t2    = l2
		t3    = res
		pre   = res
		carry = 0
	)
	for t1 != nil && t2 != nil {
		if t3 == nil {
			t3 = &ListNode{}
			pre.Next = t3
			pre = t3
		}
		sum := t1.Val + t2.Val + carry
		if sum >= 10 {
			t3.Val = sum % 10
		} else {
			t3.Val = sum
		}
		carry = sum / 10
		t1 = t1.Next
		t2 = t2.Next
		t3 = t3.Next
	}
	if t1 == nil {
		t1 = t2
	}
	for t1 != nil {
		if t3 == nil {
			t3 = &ListNode{}
			pre.Next = t3
			pre = t3
		}
		sum := t1.Val + carry
		if sum >= 10 {
			t3.Val = sum % 10
		} else {
			t3.Val = sum
		}
		carry = sum / 10
		t1 = t1.Next
		t3 = t3.Next
	}
	if carry != 0 {
		t3 = &ListNode{Val: carry}
		pre.Next = t3
	}
	return res
}

type Case struct {
	l1 *ListNode
	l2 *ListNode
}

func Test_addTwoNumbers(t *testing.T) {
	cases := []Case{
		// in: (2 -> 4 -> 3) + (5 -> 6 -> 4)
		// out: (7 -> 0 -> 8)
		{
			l1: &ListNode{
				Val: 2,
				Next: &ListNode{
					Val: 4,
					Next: &ListNode{
						Val: 3}}},
			l2: &ListNode{
				Val: 5,
				Next: &ListNode{
					Val: 6,
					Next: &ListNode{
						Val: 4}}},
		},
		// in: (1 -> 8) + (0)
		// out: (1 -> 8)
		{
			l1: &ListNode{
				Val: 1,
				Next: &ListNode{
					Val: 8}},
			l2: &ListNode{
				Val: 0},
		},
		// in: (5) + (5)
		// out: (0 -> 1)
		{
			l1: &ListNode{
				Val: 5},
			l2: &ListNode{
				Val: 5},
		},
	}
	for i, c := range cases {
		r := addTwoNumbers(c.l1, c.l2)
		fmt.Printf("[case %d]: ", i)
		for r != nil {
			fmt.Printf("%d ", r.Val)
			r = r.Next
		}
		fmt.Println()
	}
}
