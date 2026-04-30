// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 419: Battleships in a Board
 * Difficulty: Medium
 * Tags: Depth-First Search, Array, Matrix
 *
 * Problem Description:
 * Given an `m x n` matrix `board` where each cell is a battleship `'X'` or
 * empty
 * `'.'`, return _the number of the**battleships** on_ `board`.
 * 
 * **Battleships** can only be placed horizontally or vertically on `board`. In
 * other words, they can only be made of the shape `1 x k` (`1` row, `k`
 * columns)
 * or `k x 1` (`k` rows, `1` column), where `k` can be of any size. At least
 * one
 * horizontal or vertical cell separates between two battleships (i.e., there
 * are
 * no adjacent battleships).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** board = [["X",".",".","X"],[".",".",".","X"],[".",".",".","X"]]
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** board = [["."]]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == board.length`
 *   * `n == board[i].length`
 *   * `1 <= m, n <= 200`
 *   * `board[i][j]` is either `'.'` or `'X'`.
 * 
 * 
 * 
 * **Follow up:** Could you do it in one-pass, using only `O(1)` extra memory
 * and
 * without modifying the values `board`?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/battleships-in-a-board/
 */


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

class Solution {
public:
    int countBattleships(vector<vector<char>>& board) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [["X",".",".","X"],[".",".",".","X"],[".",".",".","X"]]
    // Test case 2: [["."]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
