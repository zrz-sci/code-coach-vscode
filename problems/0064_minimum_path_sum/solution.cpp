// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 64: Minimum Path Sum
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming, Matrix
 *
 * Problem Description:
 * Given a `m x n` `grid` filled with non-negative numbers, find a path from
 * top
 * left to bottom right, which minimizes the sum of all numbers along its path.
 * 
 * **Note:** You can only move either down or right at any point in time.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[1,3,1],[1,5,1],[4,2,1]]
 *     **Output:** 7
 * **Explanation:** Because the path 1 -> 3 -> 1 -> 1 -> 1 minimizes the sum.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[1,2,3],[4,5,6]]
 *     **Output:** 12
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= m, n <= 200`
 *   * `0 <= grid[i][j] <= 200`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-path-sum/
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
    int minPathSum(vector<vector<int>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,3,1],[1,5,1],[4,2,1]]
    // Test case 2: [[1,2,3],[4,5,6]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
