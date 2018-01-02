package hard

// 146. LRU Cache
//
// Design and implement a data structure for Least Recently Used (LRU) cache. It should support the following operations: get and put.
//
// get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
// put(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.
//
// Follow up:
// Could you do both operations in O(1) time complexity?
//
// Example:
//
// LRUCache cache = new LRUCache( 2 /* capacity */ );
//
// cache.put(1, 1);
// cache.put(2, 2);
// cache.get(1);       // returns 1
// cache.put(3, 3);    // evicts key 2
// cache.get(2);       // returns -1 (not found)
// cache.put(4, 4);    // evicts key 1
// cache.get(1);       // returns -1 (not found)
// cache.get(3);       // returns 3
// cache.get(4);       // returns 4

import (
	"container/list"
	"testing"
)

type LRUCache struct {
	capacity int
	list     *list.List
}

func Constructor(capacity int) LRUCache {
	return LRUCache{capacity: capacity, list: list.New()}
}

// Get from lru cache
func (this *LRUCache) Get(key int) int {
	if this.list.Len() == 0 {
		return -1
	}
	var ans int
	for head := this.list.Front(); head != nil; head = head.Next() {
		find := head.Value.(map[int]int)
		if f, ok := find[key]; ok {
			ans = f
			this.list.Remove(head)
			this.list.PushFront(map[int]int{key: ans})
			return ans
		}
	}
	return -1
}

// Put to lru cache
func (this *LRUCache) Put(key int, value int) {
	exist := false
	for head := this.list.Front(); head != nil; head = head.Next() {
		find := head.Value.(map[int]int)
		if _, ok := find[key]; ok {
			this.list.Remove(head)
			this.list.PushFront(map[int]int{key: value})
			exist = true
			break
		}
	}
	if exist {
		return
	} else {
		if this.list.Len() == this.capacity {
			this.list.Remove(this.list.Back())
		}
		this.list.PushFront(map[int]int{key: value})
	}
}

/**
 * Your LRUCache object will be instantiated and called as such:
 * obj := Constructor(capacity);
 * param_1 := obj.Get(key);
 * obj.Put(key,value);
 */

func Test_LRUCache(t *testing.T) {
	type operator struct {
		in  []int
		out int
	}
	type Case struct {
		cache     LRUCache
		operators []operator
	}
	cases := []Case{
		{
			cache: Constructor(2),
			operators: []operator{
				{in: []int{1, 1}},
				{in: []int{2, 2}},
				{in: []int{1}, out: 1},
				{in: []int{3, 3}},
				{in: []int{2}, out: -1},
				{in: []int{4, 4}},
				{in: []int{1}, out: -1},
				{in: []int{3}, out: 3},
				{in: []int{4}, out: 4},
			},
		},
		{
			cache: Constructor(2),
			operators: []operator{
				{in: []int{2}, out: -1},
				{in: []int{2, 6}},
				{in: []int{1}, out: -1},
				{in: []int{1, 5}},
				{in: []int{1, 2}},
				{in: []int{1}, out: 2},
				{in: []int{2}, out: 6},
			},
		},
	}
	for i, c := range cases {
		for _, o := range c.operators {
			if len(o.in) == 2 {
				c.cache.Put(o.in[0], o.in[1])
			} else {
				act := c.cache.Get(o.in[0])
				t.Logf("[D]%d: %d(%d)\n", i, act, o.out)
				if act != o.out {
					t.Errorf("[ERROR]%d: %d(%d)\n", i, act, o.out)
				}
			}
		}
	}
}
