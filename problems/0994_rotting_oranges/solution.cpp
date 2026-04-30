// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 994: Rotting Oranges
 * Difficulty: Medium
 * Tags: Breadth-First Search, Array, Matrix
 *
 * Problem Description:
 * You are given an `m x n` `grid` where each cell can have one of three
 * values:
 * 
 *   * `0` representing an empty cell,
 *   * `1` representing a fresh orange, or
 *   * `2` representing a rotten orange.
 * 
 * Every minute, any fresh orange that is **4-directionally adjacent** to a
 * rotten
 * orange becomes rotten.
 * 
 * Return _the minimum number of minutes that must elapse until no cell has a
 * fresh
 * orange_. If _this is impossible, return_ `-1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** grid = [[2,1,1],[1,1,0],[0,1,1]]
 *     **Output:** 4
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** grid = [[2,1,1],[0,1,1],[1,0,1]]
 *     **Output:** -1
 * **Explanation:** The orange in the bottom left corner (row 2, column 0) is
 * never rotten, because rotting only happens 4-directionally.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** grid = [[0,2]]
 *     **Output:** 0
 * **Explanation:** Since there are already no fresh oranges at minute 0, the
 * answer is just 0.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == grid.length`
 *   * `n == grid[i].length`
 *   * `1 <= m, n <= 10`
 *   * `grid[i][j]` is `0`, `1`, or `2`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rotting-oranges/
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
    int orangesRotting(vector<vector<int>>& grid) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[2,1,1],[1,1,0],[0,1,1]]
    // Test case 2: [[2,1,1],[0,1,1],[1,0,1]]
    // Test case 3: [[0,2]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
