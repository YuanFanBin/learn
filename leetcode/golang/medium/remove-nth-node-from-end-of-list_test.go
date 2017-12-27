package medium

import (
	"reflect"
	"testing"
)

// 19. Remove Nth Node From End of List
//
// Given a linked list, remove the nth node from the end of list and return its head.
//
// For example,
//
//    Given linked list: 1->2->3->4->5, and n = 2.
//
//    After removing the second node from the end, the linked list becomes 1->2->3->5.
// Note:
// Given n will always be valid.
// Try to do this in one pass.

func removeNthFromEnd(head *ListNode, n int) *ListNode {
	if head == nil || n == 0 {
		return nil
	}
	var (
		end = head
		i   = 1
	)
	for ; i <= n && end.Next != nil; i++ {
		end = end.Next
	}
	pre := head
	for end.Next != nil {
		end = end.Next
		pre = pre.Next
	}
	if pre.Next != nil && i != n {
		pre.Next = pre.Next.Next
	} else {
		return head.Next
	}
	return head
}

func Test_removeNthFromEnd(t *testing.T) {
	type Case struct {
		head *ListNode
		n    int
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
							Val: 4,
							Next: &ListNode{
								Val: 5}}}}},
			n:   2,
			exp: []int{1, 2, 3, 5},
		},
		{
			head: &ListNode{
				Val: 1,
				Next: &ListNode{
					Val: 2}},
			n:   1,
			exp: []int{1},
		},
		{
			head: &ListNode{
				Val: 1,
				Next: &ListNode{
					Val: 2}},
			n:   2,
			exp: []int{2},
		},
		{
			head: &ListNode{
				Val: 1},
			n:   1,
			exp: []int{},
		},
	}
	for i, c := range cases {
		act := removeNthFromEnd(c.head, c.n)
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
