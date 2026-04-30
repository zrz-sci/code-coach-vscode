// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 562: Longest Line of Consecutive One in Matrix
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming, Matrix
 *
 * Problem Description:
 * Given an `m x n` binary matrix `mat`, return _the length of the longest
 * line of
 * consecutive one in the matrix_.
 * 
 * The line could be horizontal, vertical, diagonal, or anti-diagonal.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** mat = [[0,1,1,0],[0,1,1,0],[0,0,0,1]]
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** mat = [[1,1,1,1],[0,1,1,0],[0,0,0,1]]
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == mat.length`
 *   * `n == mat[i].length`
 *   * `1 <= m, n <= 104`
 *   * `1 <= m * n <= 104`
 *   * `mat[i][j]` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-line-of-consecutive-one-in-matrix/
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
    int longestLine(vector<vector<int>>& mat) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,1,1,0],[0,1,1,0],[0,0,0,1]]
    // Test case 2: [[1,1,1,1],[0,1,1,0],[0,0,0,1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
