// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 200: Number of Islands
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Array, Matrix
 *
 * Problem Description:
 * Given an `m x n` 2D binary grid `grid` which represents a map of `'1'`s
 * (land)
 * and `'0'`s (water), return _the number of islands_.
 * 
 * An **island** is surrounded by water and is formed by connecting adjacent
 * lands
 * horizontally or vertically. You may assume all four edges of the grid are
 * all
 * surrounded by water.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [
 *       ["1","1","1","1","0"],
 *       ["1","1","0","1","0"],
 *       ["1","1","0","0","0"],
 *       ["0","0","0","0","0"]
 *     ]
 *     **Output:** 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [
 *       ["1","1","0","0","0"],
 *       ["1","1","0","0","0"],
 *       ["0","0","1","0","0"],
 *       ["0","0","0","1","1"]
 *     ]
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= m, n <= 300`
 *   * `grid[i][j]` is `'0'` or `'1'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-islands/
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
    int numIslands(vector<vector<char>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [["1","1","1","1","0"],["1","1","0","1","0"],["1","1","0","0","0"],["0","0","0","0","0"]]
    // Test case 2: [["1","1","0","0","0"],["1","1","0","0","0"],["0","0","1","0","0"],["0","0","0","1","1"]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
