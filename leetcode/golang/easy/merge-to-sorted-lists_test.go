package easy

import (
	"reflect"
	"testing"
)

// 21. Merge Two Sorted Lists
//
// Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.
//
// Example:
//
// Input: 1->2->4, 1->3->4
// Output: 1->1->2->3->4->4

func mergeTwoLists(l1 *ListNode, l2 *ListNode) *ListNode {
	if l1 == nil || l2 == nil {
		if l1 == nil {
			return l2
		}
		return l1
	}
	new := &ListNode{}
	p1 := l1
	p2 := l2
	p3 := new
	for p1 != nil && p2 != nil {
		for p1 != nil && p1.Val <= p2.Val {
			p3.Next = &ListNode{Val: p1.Val}
			p1 = p1.Next
			p3 = p3.Next
		}
		for p1 != nil && p2 != nil && p2.Val <= p1.Val {
			p3.Next = &ListNode{Val: p2.Val}
			p2 = p2.Next
			p3 = p3.Next
		}
	}
	if p1 == nil {
		p1 = p2
	}
	for p1 != nil {
		p3.Next = &ListNode{Val: p1.Val}
		p1 = p1.Next
		p3 = p3.Next
	}
	return new.Next
}

func Test_mergeTwoLists(t *testing.T) {
	type Case struct {
		l1  *ListNode
		l2  *ListNode
		exp []int
	}
	cases := []Case{
		{
			l1: &ListNode{
				Val: 1,
				Next: &ListNode{
					Val: 2,
					Next: &ListNode{
						Val: 4}}},
			l2: &ListNode{
				Val: 1,
				Next: &ListNode{
					Val: 3,
					Next: &ListNode{
						Val: 4}}},
			exp: []int{1, 1, 2, 3, 4, 4},
		},
		{
			l1: &ListNode{
				Val: 5,
				Next: &ListNode{
					Val: 5}},
			l2: &ListNode{
				Val: 1,
				Next: &ListNode{
					Val: 3,
					Next: &ListNode{
						Val: 4}}},
			exp: []int{1, 3, 4, 5, 5},
		},
		{
			l1: &ListNode{
				Val: 5,
				Next: &ListNode{
					Val: 5}},
			l2: &ListNode{
				Val: 1},
			exp: []int{1, 5, 5},
		},
	}
	for i, c := range cases {
		act := mergeTwoLists(c.l1, c.l2)
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
