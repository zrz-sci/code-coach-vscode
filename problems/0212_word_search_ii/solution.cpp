// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 212: Word Search II
 * Difficulty: Hard
 * Tags: Trie, Array, String, Backtracking, Matrix
 *
 * Problem Description:
 * Given an `m x n` `board` of characters and a list of strings `words`, return
 * _all words on the board_.
 * 
 * Each word must be constructed from letters of sequentially adjacent cells,
 * where
 * **adjacent cells** are horizontally or vertically neighboring. The same
 * letter
 * cell may not be used more than once in a word.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** board =
 * [["o","a","a","n"],["e","t","a","e"],["i","h","k","r"],["i","f","l","v"]],
 * words = ["oath","pea","eat","rain"]
 *     **Output:** ["eat","oath"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** board = [["a","b"],["c","d"]], words = ["abcb"]
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == board.length`
 *   * `n == board[i].length`
 *   * `1 <= m, n <= 12`
 *   * `board[i][j]` is a lowercase English letter.
 *   * `1 <= words.length <= 3 * 104`
 *   * `1 <= words[i].length <= 10`
 *   * `words[i]` consists of lowercase English letters.
 *   * All the strings of `words` are unique.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/word-search-ii/
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
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [["o","a","a","n"],["e","t","a","e"],["i","h","k","r"],["i","f","l","v"]]
    // Test case 2: ["oath","pea","eat","rain"]
    // Test case 3: [["a","b"],["c","d"]]
    // Test case 4: ["abcb"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
