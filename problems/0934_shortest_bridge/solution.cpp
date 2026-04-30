// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 934: Shortest Bridge
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Array, Matrix
 *
 * Problem Description:
 * You are given an `n x n` binary matrix `grid` where `1` represents land and
 * `0`
 * represents water.
 * 
 * An **island** is a 4-directionally connected group of `1`'s not connected
 * to any
 * other `1`'s. There are **exactly two islands** in `grid`.
 * 
 * You may change `0`'s to `1`'s to connect the two islands to form **one
 * island**.
 * 
 * Return _the smallest number of_`0` _' s you must flip to connect the two
 * islands_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,1],[1,0]]
 *     **Output:** 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,1,0],[0,0,0],[0,0,1]]
 *     **Output:** 2
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 * **Input:** grid =
 * [[1,1,1,1,1],[1,0,0,0,1],[1,0,1,0,1],[1,0,0,0,1],[1,1,1,1,1]]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == grid.length == grid[i].length`
 *   * `2 <= n <= 100`
 *   * `grid[i][j]` is either `0` or `1`.
 *   * There are exactly two islands in `grid`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-bridge/
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
    int shortestBridge(vector<vector<int>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,1],[1,0]]
    // Test case 2: [[0,1,0],[0,0,0],[0,0,1]]
    // Test case 3: [[1,1,1,1,1],[1,0,0,0,1],[1,0,1,0,1],[1,0,0,0,1],[1,1,1,1,1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
