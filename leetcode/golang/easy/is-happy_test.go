package easy

import "testing"

func isHappy(n int) bool {
	pow2 := map[int]int{0: 0, 1: 1, 2: 4, 3: 9, 4: 16, 5: 25, 6: 36, 7: 49, 8: 64, 9: 81}
	node := map[int]int{}
	for {
		var x int
		for x = 0; n != 0; {
			x += pow2[n%10]
			n /= 10
		}
		if x == 1 {
			return true
		}
		if node[x] == 1 {
			return false
		}
		node[x] = 1
		n = x
	}
}

func Test_isHappy(t *testing.T) {
	type Case struct {
		n   int
		exp bool
	}
	cases := []Case{
		{
			n:   19,
			exp: true,
		},
	}
	for i, c := range cases {
		if act := isHappy(c.n); act != c.exp {
			t.Errorf("%d: '%t'", i, act)
		}
	}
}
