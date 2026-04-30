// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1293: Shortest Path in a Grid with Obstacles Elimination
 * Difficulty: Hard
 * Tags: Breadth-First Search, Array, Matrix
 *
 * Problem Description:
 * You are given an `m x n` integer matrix `grid` where each cell is either `0`
 * (empty) or `1` (obstacle). You can move up, down, left, or right from and
 * to an
 * empty cell in **one step**.
 * 
 * Return _the minimum number of**steps** to walk from the upper left corner
 * _`(0,
 * 0)`_to the lower right corner_`(m - 1, n - 1)`_given that you can
 * eliminate**at
 * most** _`k` _obstacles_. If it is not possible to find such walk return
 * `-1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,0,0],[1,1,0],[0,0,0],[0,1,1],[0,0,0]], k = 1
 *     **Output:** 6
 *     **Explanation:** 
 *     The shortest path without eliminating any obstacle is 10.
 * The shortest path with one obstacle elimination at position (3,2) is 6.
 * Such path is (0,0) -> (0,1) -> (0,2) -> (1,2) -> (2,2) -> **(3,2)** ->
 * (4,2).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,1,1],[1,1,1],[1,0,0]], k = 1
 *     **Output:** -1
 * **Explanation:** We need to eliminate at least two obstacles to find such a
 * walk.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= m, n <= 40`
 *   * `1 <= k <= m * n`
 *   * `grid[i][j]` is either `0` **or** `1`.
 *   * `grid[0][0] == grid[m - 1][n - 1] == 0`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-path-in-a-grid-with-obstacles-elimination/
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
    int shortestPath(vector<vector<int>>& grid, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,0,0],[1,1,0],[0,0,0],[0,1,1],[0,0,0]]
    // Test case 2: 1
    // Test case 3: [[0,1,1],[1,1,1],[1,0,0]]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
