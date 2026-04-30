// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 79: Word Search
 * Difficulty: Medium
 * Tags: Depth-First Search, Array, String, Backtracking, Matrix
 *
 * Problem Description:
 * Given an `m x n` grid of characters `board` and a string `word`, return
 * `true`
 * _if_ `word` _exists in the grid_.
 * 
 * The word can be constructed from letters of sequentially adjacent cells,
 * where
 * adjacent cells are horizontally or vertically neighboring. The same letter
 * cell
 * may not be used more than once.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]],
 * word = "ABCCED"
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]],
 * word = "SEE"
 *     **Output:** true
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 * **Input:** board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]],
 * word = "ABCB"
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == board.length`
 *   * `n = board[i].length`
 *   * `1 <= m, n <= 6`
 *   * `1 <= word.length <= 15`
 * * `board` and `word` consists of only lowercase and uppercase English
 * letters.
 * 
 * 
 * 
 * **Follow up:** Could you use search pruning to make your solution faster
 * with a
 * larger `board`?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/word-search/
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
    bool exist(vector<vector<char>>& board, string word) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]]
    // Test case 2: "ABCCED"
    // Test case 3: [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]]
    // Test case 4: "SEE"
    // Test case 5: [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]]
    // Test case 6: "ABCB"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
