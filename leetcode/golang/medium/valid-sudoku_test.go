package medium

import "testing"

// 36. Valid Sudoku
//
// Determine if a 9x9 Sudoku board is valid. Only the filled cells need to be validated according to the following rules:
//
// Each row must contain the digits 1-9 without repetition.
// Each column must contain the digits 1-9 without repetition.
// Each of the 9 3x3 sub-boxes of the grid must contain the digits 1-9 without repetition.
//
// +-----------------------------+
// | 5  3  . | .  7  . | .  .  . |
// | 6  .  . | 1  9  5 | .  .  . |
// | .  9  8 | .  .  . | .  6  . |
// +-----------------------------+
// | 8  .  . | .  6  . | .  .  3 |
// | 4  .  . | 8  .  3 | .  .  1 |
// | 7  .  . | .  3  . | .  .  6 |
// +-----------------------------+
// | .  6  . | .  .  . | 2  8  . |
// | .  .  . | 4  1  9 | .  .  5 |
// | .  .  . | .  8  . | .  .  9 |
// +-----------------------------+
// A partially filled sudoku which is valid.
//
// The Sudoku board could be partially filled, where empty cells are filled with the character '.'.
//
// Example 1:
//
// Input:
// [
// ["5","3",".",".","7",".",".",".","."],
// ["6",".",".","1","9","5",".",".","."],
// [".","9","8",".",".",".",".","6","."],
// ["8",".",".",".","6",".",".",".","3"],
// ["4",".",".","8",".","3",".",".","1"],
// ["7",".",".",".","2",".",".",".","6"],
// [".","6",".",".",".",".","2","8","."],
// [".",".",".","4","1","9",".",".","5"],
// [".",".",".",".","8",".",".","7","9"]
//
// ]
// Output: true
// Example 2:
//
// Input:
// [
// ["8","3",".",".","7",".",".",".","."],
// ["6",".",".","1","9","5",".",".","."],
// [".","9","8",".",".",".",".","6","."],
// ["8",".",".",".","6",".",".",".","3"],
// ["4",".",".","8",".","3",".",".","1"],
// ["7",".",".",".","2",".",".",".","6"],
// [".","6",".",".",".",".","2","8","."],
// [".",".",".","4","1","9",".",".","5"],
// [".",".",".",".","8",".",".","7","9"]
//
// ]
// Output: false
// Explanation: Same as Example 1, except with the 5 in the top left corner being
// modified to 8. Since there are two 8's in the top left 3x3 sub-box, it is invalid.
// Note:
//
// A Sudoku board (partially filled) could be valid but is not necessarily solvable.
// Only the filled cells need to be validated according to the mentioned rules.
// The given board contain only digits 1-9 and the character '.'.
// The given board size is always 9x9.

func isValidSudoku(board [][]byte) bool {
	rows := [9][9]bool{} // 每一行的数字是否满足
	cols := [9][9]bool{} // 每一列的数字是否满足
	cube := [9][9]bool{} // 每一个3x3的数字是否满足
	cubeIndex := [][]int{
		[]int{0, 0, 0, 1, 1, 1, 2, 2, 2},
		[]int{0, 0, 0, 1, 1, 1, 2, 2, 2},
		[]int{0, 0, 0, 1, 1, 1, 2, 2, 2},
		[]int{3, 3, 3, 4, 4, 4, 5, 5, 5},
		[]int{3, 3, 3, 4, 4, 4, 5, 5, 5},
		[]int{3, 3, 3, 4, 4, 4, 5, 5, 5},
		[]int{6, 6, 6, 7, 7, 7, 8, 8, 8},
		[]int{6, 6, 6, 7, 7, 7, 8, 8, 8},
		[]int{6, 6, 6, 7, 7, 7, 8, 8, 8},
	}
	for i := 0; i < 9; i++ {
		for j := 0; j < 9; j++ {
			if board[i][j] != '.' {
				num := board[i][j] - '0' - 1
				if rows[i][num] || cols[j][num] || cube[cubeIndex[i][j]][num] {
					return false
				}
				rows[i][num] = true
				cols[j][num] = true
				cube[cubeIndex[i][j]][num] = true
			}
		}
	}
	return true
}

func Test_isValidSudoku(t *testing.T) {
	type Case struct {
		board [][]byte
		exp   bool
	}
	cases := []Case{
		{
			board: [][]byte{
				[]byte{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
				[]byte{'6', '.', '.', '1', '9', '5', '.', '.', '.'},
				[]byte{'.', '9', '8', '.', '.', '.', '.', '6', '.'},
				[]byte{'8', '.', '.', '.', '6', '.', '.', '.', '3'},
				[]byte{'4', '.', '.', '8', '.', '3', '.', '.', '1'},
				[]byte{'7', '.', '.', '.', '2', '.', '.', '.', '6'},
				[]byte{'.', '6', '.', '.', '.', '.', '2', '8', '.'},
				[]byte{'.', '.', '.', '4', '1', '9', '.', '.', '5'},
				[]byte{'.', '.', '.', '.', '8', '.', '.', '7', '9'},
			},
			exp: true,
		},
		{
			board: [][]byte{
				[]byte{'8', '3', '.', '.', '7', '.', '.', '.', '.'},
				[]byte{'6', '.', '.', '1', '9', '5', '.', '.', '.'},
				[]byte{'.', '9', '8', '.', '.', '.', '.', '6', '.'},
				[]byte{'8', '.', '.', '.', '6', '.', '.', '.', '3'},
				[]byte{'4', '.', '.', '8', '.', '3', '.', '.', '1'},
				[]byte{'7', '.', '.', '.', '2', '.', '.', '.', '6'},
				[]byte{'.', '6', '.', '.', '.', '.', '2', '8', '.'},
				[]byte{'.', '.', '.', '4', '1', '9', '.', '.', '5'},
				[]byte{'.', '.', '.', '.', '8', '.', '.', '7', '9'},
			},
			exp: false,
		},
	}
	for i, c := range cases {
		if act := isValidSudoku(c.board); act != c.exp {
			t.Errorf("case%d: %v(%v)", i, act, c.exp)
		}
	}
}
