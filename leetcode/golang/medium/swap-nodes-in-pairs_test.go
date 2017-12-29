package medium

import (
	"reflect"
	"testing"
)

// 24. Swap Nodes in Pairs
//
// Given a linked list, swap every two adjacent nodes and return its head.
//
// For example,
// Given 1->2->3->4, you should return the list as 2->1->4->3.
//
// Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.

/**
 * Definition for singly-linked list.
 * type ListNode struct {
 *     Val int
 *     Next *ListNode
 * }
 */

func swapPairs(head *ListNode) *ListNode {
	if head == nil || head.Next == nil {
		return head
	}
	var (
		prev = head
		next = head.Next
	)
	h := &ListNode{Next: head}
	pprev := h
	for {
		// fmt.Printf("I: %d, II: %d, III: %d\n", pprev.Val, prev.Val, next.Val)
		prev.Next = next.Next
		next.Next = prev
		pprev.Next = next
		if prev.Next == nil || prev.Next.Next == nil {
			break
		}
		pprev = prev
		prev = prev.Next
		next = prev.Next
	}
	return h.Next
}

func Test_swapPairs(t *testing.T) {
	type Case struct {
		head *ListNode
		exp  []int
	}
	cases := []Case{
		{
			head: &ListNode{
				Val: 1,
				Next: &ListNode{
					Val: 2,
					Next: &ListNode{
						Val: 3,
						Next: &ListNode{
							Val: 4}}}},
			exp: []int{2, 1, 4, 3},
		},
	}
	for i, c := range cases {
		act := swapPairs(c.head)
		iAct := []int{}
		for act != nil {
			iAct = append(iAct, act.Val)
			act = act.Next
		}
		if !reflect.DeepEqual(iAct, c.exp) {
			t.Errorf("%d: %v(%v)\n", i, iAct, c.exp)
		}
	}
}
